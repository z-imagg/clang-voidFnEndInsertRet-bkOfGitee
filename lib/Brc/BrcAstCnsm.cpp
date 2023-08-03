#include "Brc/BrcAstCnsm.h"



bool BrcAstCnsm::mainFileProcessed=false;


std::string BrcAstCnsm::BrcOkFlagText="__BrcOkFlagText";

 /*bool BrcAstCnsm::HandleTopLevelDecl(DeclGroupRef DG) {


  return true;
}*/

 void BrcAstCnsm::HandleTranslationUnit(ASTContext &Ctx) {
//  ASTConsumer::HandleTranslationUnit(Ctx);

   //translationUnitDecl中同时包含 非MainFile中的Decl、MainFile中的Decl
   //  因此不能用translationUnitDecl的位置 判断当前是否在MainFile中
   TranslationUnitDecl *translationUnitDecl = Ctx.getTranslationUnitDecl();

   bool inMainFile=SM.isWrittenInMainFile(translationUnitDecl->getBeginLoc());

   //跳过非MainFile
//  if(!Util::isDeclInMainFile(SM,translationUnitDecl)){
//    return;
//  }

   const DeclContext::decl_range &Decls = translationUnitDecl->decls();
   std::vector<Decl*> declVec(Decls.begin(), Decls.end());
//  inMainFile=SM.isWrittenInMainFile( translationUnitDecl->getBody()->getBeginLoc() );
   inMainFile=SM.isWrittenInMainFile( declVec[declVec.size()-1]->getBeginLoc() );
   inMainFile=SM.isWrittenInMainFile(declVec[0]->getBeginLoc());

   //region 获取主文件ID，文件路径
   FileID mainFileId;
   std::string filePath;
   Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);
   //endregion

   //region 声明组转为声明vector
//  std::vector<Decl*> declVec(DG.begin(),DG.end());
   //endregion

   //region 1.若本文件已处理，则直接返回。
   if(BrcAstCnsm::isProcessed(CI,SM,Ctx,brcOk,declVec)){
     return ;
   }
   //endregion

   //region 2. 插入花括号
   unsigned long declCnt = declVec.size();
   for(int i=0; i<declCnt; i++) {
     Decl *D = declVec[i];
     this->brcVst.TraverseDecl(D);
   }
   //endregion

   //region 3. 插入已处理标记
   bool insertResult;
   //插入的注释语句不要带换行,这样不破坏原行号
   //  必须插入此样式/** */ 才能被再次读出来， 而/* */读不出来
   const std::string brcOkFlagComment = fmt::format("/**{}*/", BrcOkFlagText);
   Decl* firstDeclInMainFile=Util::firstDeclInMainFile(SM,declVec);
   if(firstDeclInMainFile){
     Util::insertCommentBeforeLoc(brcOkFlagComment, firstDeclInMainFile->getBeginLoc(),  brcVst.mRewriter_ptr, insertResult);
   }

   //endregion

   //region 4. 应用修改到源文件
   brcVst.mRewriter_ptr->overwriteChangedFiles();
//   DiagnosticsEngine &de = SM.getDiagnostics();//de是空的，没有DiagnosticsEngine?
   DiagnosticsEngine &de = CI.getDiagnostics();//
   DiagnosticsEngine &Diags = CI.getDiagnostics();
   int error=Diags.getNumErrors();
   bool hasErrorOccurred = Diags.hasErrorOccurred();
   bool hasFatalErrorOccurred = Diags.hasFatalErrorOccurred();
   bool hasUncompilableErrorOccurred = Diags.hasUncompilableErrorOccurred();
   bool hasUnrecoverableErrorOccurred = Diags.hasUnrecoverableErrorOccurred();
   //endregion
 }

//region 判断是否已经处理过了
bool BrcAstCnsm::isProcessed(CompilerInstance& CI,SourceManager&SM, ASTContext& Ctx,  bool& _brcOk, std::vector<Decl*> declVec){
  unsigned long declCnt = declVec.size();
   for(int i=0; i<declCnt; i++){
     Decl* D=declVec[i];

     //忽略非主文件中的声明
     if(!Util::isDeclInMainFile(SM,D)){
       continue;
     }

//     Util::printDecl(Ctx,CI,"查看声明","",D,true);
     RawComment *rc = Ctx.getRawCommentForDeclNoCache(D);
     //Ctx.getRawCommentForDeclNoCache(D) 获得的注释是完整的
     BrcAstCnsm::__visitRawComment(CI,SM,  rc, _brcOk);
     if(_brcOk){
       std::cout<<"已有标记(已插入花括号),不再处理"<<std::endl;
       return true;
     }
   }
   return false;
}
void BrcAstCnsm::__visitRawComment(CompilerInstance& CI,SourceManager&SM, const RawComment *C, bool & _brcOk) {
  if(!C){
    return;
  }
  const SourceRange &sourceRange = C->getSourceRange();
//  Util::printSourceRangeSimple(CI,"查看RawComment","",sourceRange, true);

  if(_brcOk){
    return;
  }

  LangOptions &langOpts = CI.getLangOpts();
  std::string sourceText = Util::getSourceTextBySourceRange(sourceRange, SM, langOpts);
//      brcOk= (sourceText==BrcOkFlagText);//由于取出来的可能是多个块注释，导致不能用相等判断，只能用下面的包含判断
  std::string::size_type index = sourceText.find(BrcOkFlagText);
  _brcOk= (index != std::string::npos);

}

//endregion
