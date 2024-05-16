
#ifndef RangeHasMacroAstVst_H
#define RangeHasMacroAstVst_H


#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Basic/TargetInfo.h"
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>

#include "base/Util.h"

using namespace clang;


class RangeHasMacroAstVst : public RecursiveASTVisitor<RangeHasMacroAstVst> {
public:
    CompilerInstance& CI;
    //限定位置范围，即 caseK起至范围 ： 即case[K]的冒号 到 case[K+1]的'case'前
    SourceRange caseKSrcRange;
    bool hasMacro;
    //caseK的子语句个数
    int caseKSubStmtCnt;
    //直接写在'case'内的变量声明语句个数
    //有两种情况:
    //  1. 直接写在'case'内，其父亲是case语句的
    //  1. 直接写在'case'内, 但是其父亲是switch块的
    //注意: 直接写的'case {'内的 、 直接写在'case'内  不是一回事，而是差别很大。
    int VFIRDeclDirectlyInCaseKCnt;

    explicit RangeHasMacroAstVst(CompilerInstance &_CI , SourceRange _sourceRange )
            :
            CI(_CI),
            caseKSrcRange(_sourceRange),
            hasMacro(false),
            caseKSubStmtCnt(0),
            VFIRDeclDirectlyInCaseKCnt(0)
    {

    }
    bool VisitStmt(clang::Stmt *stmt) ;
};


#endif //RangeHasMacroAstVst_H
