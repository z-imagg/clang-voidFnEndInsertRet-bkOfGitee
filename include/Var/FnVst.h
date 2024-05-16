#ifndef FnVst_H
#define FnVst_H


#include <clang/Rewrite/Core/Rewriter.h>
#include <set>
#include <clang/Frontend/CompilerInstance.h>
#include <unordered_set>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "base/LocId.h"

using namespace llvm;
using namespace clang;

/**
 * 插入花括号Visitor
 */
class FnVst
        : public RecursiveASTVisitor<FnVst> {
public:
public:
    //Rewriter:4:  Consumer将Rewriter传递给Visitor
    explicit FnVst(CompilerInstance &CI, const std::shared_ptr<Rewriter> rewriter_ptr, ASTContext *Ctx, SourceManager& SM, LangOptions &_langOptions)
    //Rewriter:5:  Consumer将Rewriter传递给Visitor, 并由Visitor.mRewriter接收
    : mRewriter_ptr(rewriter_ptr),
    Ctx(Ctx),CtxRef(*Ctx),
    CI(CI),
    SM(SM)
    {

    }

    bool insertBefore_X__funcReturn(LocId cmpndStmRBrcLocId, SourceLocation cmpndStmRBrcLoc  );
    bool insertAfterFnLeftBrace__insertBeforeFnRightBrace(LocId fnBdLBrcLocId, SourceLocation funcBodyLBraceLoc , SourceLocation funcBodyRBraceLoc );
    bool TraverseCompoundStmt(CompoundStmt *compoundStmt  );
    virtual bool TraverseFunctionDecl(FunctionDecl* funcDecl);
    bool TraverseCXXConstructorDecl(CXXConstructorDecl* cxxCnstrDecl);
    bool TraverseCXXMethodDecl(CXXMethodDecl* cxxMethDecl);
    bool TraverseCXXConversionDecl(CXXConversionDecl * cxxCnvDecl);
    bool TraverseCXXDestructorDecl(CXXDestructorDecl * cxxDestructorDecl);
    bool I__TraverseCXXMethodDecl(CXXMethodDecl* cxxMethDecl,const char* who);
    bool TraverseLambdaExpr(LambdaExpr *lambdaExpr);
    bool _Traverse_Func(
//            bool funcIsStatic,
//            bool funcIsInline,
            QualType funcReturnType,
            bool isaCXXConstructorDecl,
            Stmt *endStmtOfFuncBody,
            SourceLocation funcBodyLBraceLoc,
            SourceLocation funcBodyRBraceLoc,
            LocId funcBodyLBraceLocId,
            LocId funcBodyRBraceLocId,
            CompoundStmt* compoundStmt,
            std::string funcName
    );

    bool TraverseReturnStmt(ReturnStmt *returnStmt);




public:
    const std::shared_ptr<Rewriter> mRewriter_ptr;

public:
    ASTContext *Ctx;
    ASTContext& CtxRef;
    CompilerInstance& CI;
    SourceManager& SM;


    std::unordered_set<LocId,LocId> funcReturnLocIdSet;
    std::unordered_set<LocId,LocId> funcEnterLocIdSet;
};


#endif