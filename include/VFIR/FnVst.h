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

    bool insertBefore_Return(LocId cmpndStmRBrcLocId, SourceLocation cmpndStmRBrcLoc  );
    bool TraverseCompoundStmt(CompoundStmt *compoundStmt  );




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