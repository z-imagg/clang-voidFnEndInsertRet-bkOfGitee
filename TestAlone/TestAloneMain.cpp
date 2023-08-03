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

using namespace clang;


class Util {
public:
    static std::tuple<int,int> extractLineAndColumn(const clang::SourceManager& SM, const clang::SourceLocation& sourceLocation ) {
      clang::PresumedLoc presumedLoc = SM.getPresumedLoc(sourceLocation);
      int line = presumedLoc.getLine();
      int column = presumedLoc.getColumn();
      return std::tuple<int,int>(line,column);

    }
    static SourceLocation getStmtEndSemicolonLocation(const Stmt *S, const SourceManager &SM,bool& endIsSemicolon) {

      const LangOptions &LO = LangOptions();
      Token JTok;

      // 获取Stmt的结束位置
      SourceLocation JLoc = S->getEndLoc();
//      Lexer::getRawToken(EndLoc, JTok, SM, LO);
      const std::tuple<int, int> &JLocLC = extractLineAndColumn(SM, JLoc);//开发看行号

//      SourceLocation JLoc = Lexer::getLocForEndOfToken(EndLoc, 0, SM, LO); 循环变量初值 不能是此行
//      SourceLocation JLoc = EndLoc; //有可能EndLoc就是语句末尾分号, 即 循环变量初值 应该是 EndLoc


      const std::tuple<int, int> &initNextTokenEndLocLC = extractLineAndColumn(SM, JLoc);//开发看行号

      std::tuple<int, int> NextTokenEndLocLC;//开发看行号

      // 查找下一个分号
      do{

        Lexer::getRawToken(JLoc, JTok, SM, LO);
        NextTokenEndLocLC = extractLineAndColumn(SM, JLoc);//开发看行号
        JLoc = Lexer::getLocForEndOfToken(JTok.getLocation(), 0, SM, LO);

        //region 开发打印日志
//        std::string str=JLoc.printToString(SM);
//        std::cout<< str <<std::endl;
        //endregion
      }while (JTok.isNot(tok::semi)
//      && JTok.isNot(tok::eof)//这里不太确定
              && JLoc.isInvalid() //若没有此条件则当invalid时陷入死循环
              );


      // 获取分号的结束位置
      SourceLocation SemicolonEndLoc = Lexer::getLocForEndOfToken(JTok.getLocation(), 0, SM, LO);//还有问题：当出来时候，JTok的下一个居然是分号，却没注意到 比如 return a+b
      const std::tuple<int, int> &SemicolonEndLocLC = extractLineAndColumn(SM, SemicolonEndLoc);//开发看行号

      endIsSemicolon=JTok.is(tok::semi);
      return SemicolonEndLoc;
//      return JTok.getLocation();
    }
};

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
    CompilerInstance& CI;

    explicit MyASTVisitor(CompilerInstance &_CI, const std::shared_ptr<Rewriter> _rewriter_ptr ) : CI(_CI)  {

    }
    bool VisitStmt(clang::Stmt *stmt) {
      bool endIsSemicolon=false;
      SourceManager &SM = CI.getSourceManager();
      LangOptions &LO = CI.getLangOpts();

      const SourceRange &sourceRange = stmt->getSourceRange();
      CharSourceRange charSourceRange=CharSourceRange::getCharRange(sourceRange);
      std::string strSourceText=Lexer::getSourceText(charSourceRange, SM, LO).str();

      bool eq=(strSourceText=="float fff = a + b / 10  + MyClass::ZERO     ");//此文本是从程序输出拿到的
      const SourceLocation &semicolonLoc = Util::getStmtEndSemicolonLocation(stmt, SM,endIsSemicolon);//条件断点 eq为真
      const std::string &semicolonLocStr = semicolonLoc.printToString(SM);
      llvm::outs() << "VisitStmt: " << stmt->getStmtClassName()  << ": 【" << strSourceText  << "】,结尾是否分号:"<<endIsSemicolon<<",semicolonLocStr: " << semicolonLocStr << "\n";

      return true;
    }
/*输出:
TraverseStmt: ImplicitCastExpr
TraverseStmt: FloatingLiteral
TraverseStmt: CompoundStmt
TraverseStmt: ReturnStmt
TraverseStmt: BinaryOperator
TraverseStmt: ImplicitCastExpr
TraverseStmt: DeclRefExpr
TraverseStmt: ImplicitCastExpr
TraverseStmt: DeclRefExpr
 */
};

class MyASTConsumer : public clang::ASTConsumer {
public:
    CompilerInstance &CI;
    MyASTVisitor visitor;
    explicit MyASTConsumer(CompilerInstance &_CI, const std::shared_ptr<Rewriter> _rewriter_ptr)  : CI(_CI) ,visitor(_CI,_rewriter_ptr)  {

    }

    void HandleTranslationUnit(clang::ASTContext &Context) override {
      for (Decl *D : Context.getTranslationUnitDecl()->decls()) {
        visitor.TraverseDecl(D);
      }


//      TranslationUnitDecl *translationUnitDecl = Context.getTranslationUnitDecl();
    }
};

class MyASTFrontendAction : public clang::ASTFrontendAction {
public:
    const std::shared_ptr<Rewriter> mRewriter_ptr=std::make_shared<Rewriter>();//这里是插件Act中的Rewriter，是源头，理应构造Rewriter.


    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {

      SourceManager &SM = CI.getSourceManager();
      LangOptions &langOptions = CI.getLangOpts();
      ASTContext &astContext = CI.getASTContext();

      CI.getDiagnostics().setSourceManager(&SM);

      mRewriter_ptr->setSourceMgr(SM, langOptions);


      return std::make_unique<MyASTConsumer>(CI,mRewriter_ptr);
    }
};

int main() {
  // 创建 Clang 编译实例
  clang::CompilerInstance CI;

  CI.createDiagnostics();
  CI.createFileManager();
  CI.createSourceManager(CI.getFileManager());

  CI.getLangOpts().CPlusPlus = true;

  CI.getTargetOpts().Triple= "x86_64-pc-linux-gnu";

  llvm::Triple triple("x86_64-pc-linux-gnu");
  std::shared_ptr<clang::TargetOptions> targetOpts=std::make_shared<clang::TargetOptions>();
  targetOpts->Triple=triple.str();
  TargetInfo* targetInfo=  TargetInfo::CreateTargetInfo(CI.getDiagnostics(), targetOpts) ;
  CI.setTarget(targetInfo);

  CI.createPreprocessor(clang::TU_Complete);
  CI.getPreprocessor().Initialize(*targetInfo);


  SourceManager& SM=CI.getSourceManager();
  LangOptions &LO = CI.getLangOpts();
  Preprocessor &PP = CI.getPreprocessor();

  //添加诊断
  llvm::raw_ostream &OS = llvm::outs();
  DiagnosticOptions *diagnosticOptions = new clang::DiagnosticOptions();
  clang::TextDiagnosticPrinter *TextDiag = new TextDiagnosticPrinter(OS, diagnosticOptions);
  TextDiag->BeginSourceFile(LO,&PP);
//  TextDiag->EndSourceFile();
  CI.getDiagnostics().setClient(TextDiag);

  // 创建 ASTFrontendAction 实例
  clang::FrontendAction* Action = new MyASTFrontendAction();

  // 设置输入文件
  CI.getFrontendOpts().Inputs.push_back(clang::FrontendInputFile("/pubx/clang-brc/test_in/test_main.cpp", clang::InputKind(clang::Language::CXX)));

  // 运行 Clang 编译
  if (!CI.ExecuteAction(*Action)) {
    llvm::errs() << "Clang compilation failed\n";
    return 1;
  }

  return 0;
}