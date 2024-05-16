#!/bin/bash

#【描述】
#【依赖】
#【术语】
#【备注】

source /app/bash-simplify/_importBSFn.sh
_importBSFn "assertFileExisted.sh"
_importBSFn "cdCurScriptDir.sh"
_importBSFn "git_Clone_SwitchTag.sh"
_importBSFn "cmakeInstall.sh"
_importBSFn "link_CppPrj_includeDir_to.sh"

cat /etc/issue
#Ubuntu 22.04.4 LTS \n \l

#解决make时报错: /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/include/llvm/Support/SwapByteOrder.h:17:10: fatal error: 'cstddef' file not found
sudo apt install libstdc++-12-dev -y

LLVM15Home=/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4

#断言llvm15存在
assertFileExisted $LLVM15Home/bin/clang-15 "请手工下载、解压LLVM15到'$LLVM15Home' " || exit $?

REPO_HOME=/fridaAnlzAp/clang-var

#到当前目录
cdCurScriptDir


cmakeInstall || exit $?

#克隆本仓库
git_Clone_SwitchTag http://giteaz:3000/frida_analyze_app_src/clang-var.git tag__release $REPO_HOME

#构建libfmt
bash /app/fmtlib-fmt/build-libfmt.sh && \

link_CppPrj_includeDir_to http://giteaz:3000/util/nlohmann--json.git   tag__v3.11.3_fix   "/app/nlohmann--json"   include/nlohmann/   "/fridaAnlzAp/clang-var/include/nlohmann" || exit $?
##   "/fridaAnlzAp/clang-var/include/nlohmann" --> "/app/nlohmann--json/include/nlohmann/"

link_CppPrj_includeDir_to http://giteaz:3000/util/yhirose--cpp-httplib.git   tag__v0.14.2a   "/app/yhirose--cpp-httplib"   ./   "/fridaAnlzAp/clang-var/include/cpp_httplib" || exit $?
##   "/fridaAnlzAp/clang-var/include/cpp_httplib" --> "/app/yhirose--cpp-httplib"

BUILD_HOME=$REPO_HOME/build/
( mkdir -p $BUILD_HOME && rm -fr $BUILD_HOME/* ;) || exit $?

{
#构建的Release版本 : build_release_0.sh
# LIB_VFIR_CXX_FLAGS="  "
# -rwxrwxr-x 1 zz zz 874K  7月 24 13:13 /bal/clang-add-funcIdAsm/build/lib/libVFIR.so

#构建的Release版本,带调试信息(变量名称和行号),  当出现问题方便排查: build_release_1.sh
LIB_VFIR_CXX_FLAGS=" -fno-omit-frame-pointer -Wall   -O0    -g1"
# -rwxrwxr-x 1 zz zz 4.6M  7月 24 14:50 lib/libVFIR.so

#构建的Release版本,带调试信息(变量名称和行号、局部变量),  当出现问题方便排查:  build_release_2.sh
# LIB_VFIR_CXX_FLAGS=" -fno-omit-frame-pointer -Wall   -O0    -g2"
# -rwxrwxr-x 1 zz zz 27M  7月 24 15:03 /bal/clang-add-funcIdAsm/build/lib/libVFIR.so

cmake -S $REPO_HOME -B $BUILD_HOME  -DCMAKE_BUILD_TYPE=Release   -DCMAKE_CXX_FLAGS="$LIB_VFIR_CXX_FLAGS"   -DCMAKE_C_FLAGS="$LIB_VFIR_CXX_FLAGS"    && \
#-DCMAKE_VERBOSE_MAKEFILE=True
( cd $BUILD_HOME && make -j8 ;) && \
ls -lh $BUILD_HOME/lib/lib*.so ;}

$BUILD_HOME/bin/VFIRAlone $REPO_HOME/test_in/test_main.cpp  && echo "TEST_OK"

## 构建Release版本且有带局部变量名字的调试信息 以定位问题
#> GCC  调试信息不同级别选项 ：
#- `-g1`：生成最基本的调试信息，包括变量名称和行号信息。
#- `-g2`：除了基本的调试信息外，还包括局部变量的类型信息。
#- `-g3`：除了 `-g2` 提供的信息外，还包括宏定义和宏展开的内容。
