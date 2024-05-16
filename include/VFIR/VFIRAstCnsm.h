#ifndef VFIRAstCnsm_H
#define VFIRAstCnsm_H

#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>
#include <filesystem>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Comment.h"
#include "clang/AST/CommentVisitor.h"


#include <fmt/core.h>

#include "VFIR/FnVst.h"
#include "base/Util.h"

using namespace llvm;
using namespace clang;

//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------


class VFIRAstCnsm : public ASTConsumer {
public:
    //Rewriter:3:  Action将Rewriter传递给Consumer
    explicit VFIRAstCnsm(CompilerInstance &_CI, const std::shared_ptr<Rewriter> _rewriter_ptr, ASTContext *_astContext,
                        SourceManager &_SM, LangOptions &_langOptions)
            //Rewriter:4:  Consumer将Rewriter传递给Visitor
            :
            CI(_CI),
            Ctx(*_astContext),
            SM(_SM),
            varOk(false),
            fnVst(_CI, _rewriter_ptr, _astContext, _SM, _langOptions)
            {
      //构造函数
//      _rewriter_ptr->overwriteChangedFiles();//C'正常.
    }

//    virtual bool HandleTopLevelDecl(DeclGroupRef DG) ;

    void HandleTranslationUnit(ASTContext &Ctx) override;


public:
    CompilerInstance &CI;
    ASTContext & Ctx;
//    FnVst insertVst;
//    FndVarFlagCmtHdl findTCCallROVisitor;
    SourceManager &SM;
    //两次HandleTranslationUnit的ASTConsumer只能每次新建，又期望第二次不要发生，只能让标志字段mainFileProcessed写成static
    static bool mainFileProcessed;

    //region 判断是否已经处理过了
    //花括号是否已插入
    bool varOk;
    //endregion

    //region 进行处理：插入花括号
    FnVst fnVst;
    //endregion

};


#endif