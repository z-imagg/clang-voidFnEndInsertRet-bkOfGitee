
#ifndef PPCb_H
#define PPCb_H
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "base/LocId.h"


#include <unordered_set>

using namespace clang;

class PPCb : public clang::PPCallbacks {
public:
    CompilerInstance &CI;


    explicit PPCb(CompilerInstance &_CI) : CI(_CI) {

    }


	////program
	
	void PragmaMessage(SourceLocation Loc, StringRef namespaceSR, PPCallbacks::PragmaMessageKind msgKind, StringRef msgSR) ;

    /** pragma_message_set 是 方法PragmaMessage 被回调后的结果
     */
    static std::set<std::string> pragma_message_set;

    /**
     * pragmaMsgFull决定了Constant::NameSpace_funcIdAsmIns的值的样式是 "命名空间:pragmaMessgae"
     * @param namespac
     * @param pragmaMessage
     * @return
     */
    static std::string pragmaMsgFull(std::string namespac, std::string pragmaMessage){
        auto msgFull=fmt::format("{}:{}",namespac,pragmaMessage);
        return msgFull;
    }

};


#endif //PPCb_H
