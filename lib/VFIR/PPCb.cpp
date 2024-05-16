#include "VFIR/PPCb.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>


#include "base/Util.h"


using namespace clang;

    //预处理回调收集#includee 以判断case起止范围内 有无#i
    void PPCb::InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange, Optional<FileEntryRef> File, StringRef SearchPath,
                            StringRef RelativePath, const Module *Imported,
                            SrcMgr::CharacteristicKind FileType)   {
      //region 方便变量
      SourceManager &SM = CI.getSourceManager();
      //endregion

      //region 获取主文件ID,文件路径
      FileID mainFileId;
      std::string filePath;
      Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);
      //endregion

      //region 跳过非主文件
      if(!SM.isWrittenInMainFile(HashLoc)){
        return;
      }
      //endregion

      //region 收集 #include指令 位置
      InclusionDirectiveLocSet.insert(LocId::buildFor(filePath ,HashLoc,SM));
//      std::cout << "Include指令:" << FileName.str() << std::endl;//开发打印日志
      //endregion
    }

    //预处理回调收集#define 以判断case起止范围内 有无#d
    void PPCb::MacroDefined(const clang::Token& MacroNameTok,
                              const clang::MacroDirective* MD)   {
      //region 方便变量
      const clang::MacroInfo* MI = MD->getMacroInfo();
      SourceManager &SM = CI.getSourceManager();
      //endregion

      //region 获取主文件ID,文件路径
      FileID mainFileId;
      std::string filePath;
      Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);
      //endregion

      //region 跳过非主文件
      SourceLocation macroNameTkLoc = MacroNameTok.getLocation();
      if(!SM.isWrittenInMainFile(macroNameTkLoc)){
        return;
      }
      //endregion

      //region 收集 #define定义 位置
      MacroDefinedLocSet.insert(LocId::buildFor(filePath, macroNameTkLoc,SM));
//      std::cout << "宏定义:" << MacroNameTok.getIdentifierInfo()->getName().str() << std::endl;//开发打印日志
      //endregion
    }

    //判断给定起止范围内 有无#include
    bool PPCb::hasInclusionDirective(SourceManager& SM, SourceRange range){
      bool hasIncDInRange=std::any_of(
              PPCb::InclusionDirectiveLocSet.begin(),
              PPCb::InclusionDirectiveLocSet.end(),
              [&range,&SM](LocId locIdK){
                  return locIdK.containedByRange(SM,range);
              }
      );
      return hasIncDInRange;
    }

    //判断给定起止范围内 有无#define
    bool PPCb::hasMacroDefined(SourceManager& SM, SourceRange range){
      bool hasMacroDInRange=std::any_of(
              PPCb::MacroDefinedLocSet.begin(),
              PPCb::MacroDefinedLocSet.end(),
              [&range,&SM](LocId locIdK){
                  return locIdK.containedByRange(SM,range);
              }
      );
      return hasMacroDInRange;
    }

std::unordered_set<LocId,LocId> PPCb::InclusionDirectiveLocSet;
std::unordered_set<LocId,LocId> PPCb::MacroDefinedLocSet;


////program

//如果本源文件中根本没有#pragma ，则方法PragmaMessage不会被调用
    void PPCb::PragmaMessage(SourceLocation Loc, StringRef namespaceSR, PPCallbacks::PragmaMessageKind msgKind, StringRef msgSR) {
        //region 方便变量
        SourceManager &SM = CI.getSourceManager();
        //endregion

        //region 获取主文件ID,文件路径
        FileID mainFileId;
        std::string filePath;
        Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);
        //endregion

        //region 跳过非主文件
        if(!SM.isWrittenInMainFile(Loc)){
            return;
        }
        //endregion

        if(PPCallbacks::PragmaMessageKind::PMK_Message != msgKind){
            return;
        }

        //region 收集  #pragma message
        auto msg=msgSR.str();
        auto namespac=namespaceSR.str();

        auto msgFull=PPCb::pragmaMsgFull(namespac,msg);

        pragma_message_set.insert(msgFull);
        std::cout << fmt::format("namespaceSR:{} , msgSR:{}, msgKind:{}, msgFull:{}\n", namespaceSR.str(), msgSR.str() , (int)msgKind, msgFull) ;

        //endregion
    }


std::set<std::string> PPCb::pragma_message_set;