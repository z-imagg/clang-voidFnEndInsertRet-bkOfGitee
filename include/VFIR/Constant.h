
#ifndef VFIR_CONSTANT_H
#define VFIR_CONSTANT_H
#include <string>
#include <fmt/core.h>


class Constant{
public:
    const   std::string PrgMsgStmt_funcIdAsmIns = "#pragma message(\"VFIR_inserted\")\n";
    const   std::string Include_RuntimeCxx = "#include \"runtime_cpp__vars_fn.h\"\n";
    const   std::string Include_RuntimeC00 = "#include \"runtime_c__vars_fn.h\"\n";
    // 开头语句文本 c++
    const   std::string PrgMsg_IncRuntime_Cxx = fmt::format("%s %s",PrgMsgStmt_funcIdAsmIns,Include_RuntimeCxx);
    // 开头语句文本 c
    const   std::string PrgMsg_IncRuntime_C00 = fmt::format("%s %s",PrgMsgStmt_funcIdAsmIns,Include_RuntimeC00);

    //PPCb::pragmaMsgFull决定了Constant::NameSpace_funcIdAsmIns的值的样式是 "命名空间:pragmaMessgae"
    const   std::string NameSpace_funcIdAsmIns = ":VFIR_inserted";

};
#endif //VFIR_CONSTANT_H
