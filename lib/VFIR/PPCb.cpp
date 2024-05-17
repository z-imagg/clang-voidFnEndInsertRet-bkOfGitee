#include "VFIR/PPCb.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>


#include "base/Util.h"


using namespace clang;



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