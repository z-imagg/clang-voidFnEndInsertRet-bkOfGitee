
#ifndef VFIR_CONSTANT_H
#define VFIR_CONSTANT_H
#include <string>


class Constant{
public:
    const   std::string PrgMsgStmt_funcIdAsmIns = "#pragma message(\"VFIR_inserted\")\n";

    //PPCb::pragmaMsgFull决定了Constant::NameSpace_funcIdAsmIns的值的样式是 "命名空间:pragmaMessgae"
    const   std::string NameSpace_funcIdAsmIns = ":VFIR_inserted";

};
#endif //VFIR_CONSTANT_H
