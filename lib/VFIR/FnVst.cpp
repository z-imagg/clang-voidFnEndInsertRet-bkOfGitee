#include "VFIR/FnVst.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "base/Util.h"
#include <vector>

#include <fmt/core.h>
#include <iostream>
#include <clang/AST/ParentMapContext.h>

#include "base/MyAssert.h"

using namespace llvm;
using namespace clang;

//【术语】 cmpnd == compound, Brc == Brace == 花括号, LBrc == Left Brace == 左花括号, RBrc == Right Brace == 右花括号


bool FnVst::TraverseCompoundStmt(CompoundStmt *compoundStmt  ){
    //跳过非MainFile
    bool _LocFileIDEqMainFileID=Util::LocFileIDEqMainFileID(SM,compoundStmt->getBeginLoc());
    if(!_LocFileIDEqMainFileID){
        return false;
    }

//    Util::printStmt(CtxRef, CI, "进入TraverseCompoundStmt", "", compoundStmt, false);  //开发用打印
    //获取主文件ID,文件路径
    FileID mainFileId;
    std::string filePath;
    Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);

    DynTypedNode parent;
    ASTNodeKind parentNK;
    bool only1P = Util::only1ParentNodeKind(CI, CtxRef, compoundStmt, parent, parentNK);
    //参考 例子语法树 http://giteaz:3000/cl_plg/clang-voidFnEndInsertRet/src/branch/release/test_in/test_main.cpp.syntax_tree.txt
    bool parentNKIsFunctionDecl = ASTNodeKind::getFromNodeKind<FunctionDecl>().isSame(parentNK);
    bool parentNKIsCXXMethodDecl = ASTNodeKind::getFromNodeKind<CXXMethodDecl>().isSame(parentNK);
    bool parentNKIsCXXConstructorDecl = ASTNodeKind::getFromNodeKind<CXXConstructorDecl>().isSame(parentNK);
    bool parentNKIsCXXDestructorDecl = ASTNodeKind::getFromNodeKind<CXXDestructorDecl>().isSame(parentNK);

    //块语句直接父亲是FunctionDecl | CXXMethodDecl | CXXConstructorDecl | CXXDestructorDecl
    bool care=(only1P && (parentNKIsFunctionDecl || parentNKIsCXXMethodDecl || parentNKIsCXXConstructorDecl || parentNKIsCXXDestructorDecl));
    //若不关注 则直接返回
    if (!care){
        return true;
    }
/////////////////////对当前节点compoundStmt做 自定义处理
//  Util::printStmt(CtxRef,CI,"t1","",compoundStmt,true);
  const ASTNodeKind &parentNodeKind = parent.getNodeKind();
  //如果是当前块语句的父节点 是 FunctionDecl的子类
  //  FunctionDecl的子类 == 普通函数、c++成员函数、c++构造函数、c++析构函数
  if(ASTNodeKind::getFromNodeKind<FunctionDecl>() .isBaseOf(parentNodeKind)){
    const FunctionDecl *functionDecl = parent.get<FunctionDecl>();
    const QualType &declaredReturnType = functionDecl->getDeclaredReturnType();
    const Type *typePtr = declaredReturnType.getTypePtr();
    assert(typePtr!=NULL);
    bool isVoidType = typePtr->isVoidType();
    if(!isVoidType){
      //若本函数返回类型不是void, 则直接返回
      return true;
    }
  }
  ///1.2 return语句插入位置是 组合语句 右花括号} 前
  SourceLocation insertLoc=compoundStmt->getRBracLoc();

  Stmt *endStmt = compoundStmt->body_back();

  const SourceLocation &compoundStmtLBracLoc = compoundStmt->getLBracLoc();
  const SourceLocation &compoundStmtRBracLoc = compoundStmt->getRBracLoc();


  // 块尾语句是不是return
  bool compoundEndStmtIsReturn=Util::isReturnStmtClass(endStmt);

  //若 有 栈变量释放 且 未曾插入过 释放语句，则插入释放语句
  if(!compoundEndStmtIsReturn){

    bool parentKindIsCXXConstructorDecl= Util::parentKindIsSame(Ctx, compoundStmt, ASTNodeKind::getFromNodeKind<CXXConstructorDecl>());
    bool compoundStmtLRBrace_inSameLine=Util::isEqSrcLocLineNum(SM,compoundStmtLBracLoc,compoundStmtRBracLoc);
    //若是构造方法、且方法体左花括号和右花括号在同一行，则新插入的return语句前有换行
    bool withNewLinePrefix= parentKindIsCXXConstructorDecl && compoundStmtLRBrace_inSameLine;

    LocId compoundStmtRBracLocId=LocId::buildFor(filePath,  compoundStmtRBracLoc, SM);
  ///1.7  在上面算出的位置处, 插入释放语句
    bool insertResult=insertBefore_Return(
            compoundStmtRBracLocId,
            compoundStmtRBracLoc,
            withNewLinePrefix);

  }

  return true;
}


bool FnVst::insertBefore_Return(LocId cmpndStmRBrcLocId, SourceLocation cmpndStmRBrcLoc ,bool withNewLinePrefix ){
    std::string verbose="";
    //环境变量 clangPlgVerbose_voidFnEndInsertRet 控制 是否在注释中输出完整路径_行号_列号
    if(Util::envVarEq("clangPlgVerbose_voidFnEndInsertRet","true")){
        verbose=cmpndStmRBrcLocId.to_string();
    }

    //region 构造插入语句
    std::string cStr_destroy=fmt::format(
            "{}return; /* voidFnEndInsertRet: {}*/",
            withNewLinePrefix?"\n":"",
            verbose
    );
    llvm::StringRef strRef_destroy(cStr_destroy);
    bool insertResult_destroy=mRewriter_ptr->InsertTextBefore(cmpndStmRBrcLoc , strRef_destroy);
    //endregion

    //记录已插入语句的节点ID们以防重： 即使重复遍历了 但不会重复插入
    funcReturnLocIdSet.insert(cmpndStmRBrcLocId);
    return insertResult_destroy;
}