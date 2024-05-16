#include "VFIR/FnVst.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "base/Util.h"
#include "VFIR/RangeHasMacroAstVst.h"
#include <vector>

#include <fmt/core.h>
#include <iostream>
#include <clang/AST/ParentMapContext.h>

#include "base/MyAssert.h"

using namespace llvm;
using namespace clang;



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
  //region 0.准备、开发用语句
//  int64_t compoundStmtID = compoundStmt->getID(*Ctx);
//  const Stmt::child_range &subStmtLs = compoundStmt->children();

//  const std::string &compoundStmtText = Util::getSourceTextBySourceRange(compoundStmt->getSourceRange(), SM, CI.getLangOpts());
//
//  std::vector<clang::Stmt*> subStmtVec(subStmtLs.begin(), subStmtLs.end());
//  unsigned long subStmtCnt = subStmtVec.size();


  ///1.2 块尾释放语句默认插入位置是 组合语句 右花括号} 前
  SourceLocation insertLoc=compoundStmt->getRBracLoc();

  Stmt *endStmt = compoundStmt->body_back();

    const SourceLocation &compoundStmtRBracLoc = compoundStmt->getRBracLoc();

  // 块尾语句是不是return
  bool compoundEndStmtIsReturn=Util::isReturnStmtClass(endStmt);

  //若 有 栈变量释放 且 未曾插入过 释放语句，则插入释放语句
  if(!compoundEndStmtIsReturn){

      LocId compoundStmtRBracLocId=LocId::buildFor(filePath, "", compoundStmtRBracLoc, SM);
  ///1.7  在上面算出的位置处, 插入释放语句
    bool insertResult=insertBefore_Return(
            compoundStmtRBracLocId,
            compoundStmtRBracLoc);
//    std::string title=fmt::format("插入结果:{},RwtPtr:{:x}",    insertResult,reinterpret_cast<uintptr_t> (mRewriter_ptr.get() ) );
//    Util::printStmt(*Ctx, CI, "TraverseCompoundStmt插入块释放", title, compoundStmt, false);  //开发用打印

  }
  //endregion


  return true;
}

bool FnVst::insertBefore_Return(LocId cmpndStmRBrcLocId, SourceLocation cmpndStmRBrcLoc  ){
    //region 构造插入语句
    std::string cStr_destroy=fmt::format(
            "return; /* voidFnEndInsertRet: {}*/",
            cmpndStmRBrcLocId.filePath,
            cmpndStmRBrcLocId.funcName,
            cmpndStmRBrcLocId.to_string()
    );
    llvm::StringRef strRef_destroy(cStr_destroy);
    bool insertResult_destroy=mRewriter_ptr->InsertTextBefore(cmpndStmRBrcLoc , strRef_destroy);
    //endregion

    //记录已插入语句的节点ID们以防重： 即使重复遍历了 但不会重复插入
    funcReturnLocIdSet.insert(cmpndStmRBrcLocId);
    return insertResult_destroy;
}