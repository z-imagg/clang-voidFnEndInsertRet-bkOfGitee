#### 术语
- VFIR == Void Function Insert Return == 'void函数末尾无return时候，补充return语句'

#### 插件运行举例

```shell
LLVM15HOME=/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4
export PATH=$LLVM15HOME/bin:$PATH
test_main_cpp_F=/fridaAnlzAp/clang-voidFnEndInsertRet/test_in/test_main.cpp
clgPlg_so_F=/fridaAnlzAp/clang-voidFnEndInsertRet/build/lib/libVFIRPlugin.so
clgPlg_name=VFIRPlugin
```

##### clang插件主要运行方法

###### 一般运行插件
-  `-add-plugin` 尝试 编译、本插件， 本插件 正常修改源文件， 正常编译 输出 test_main.o

       不确定 编译、本插件 之间的先后次序
  ```shell
  clang++  -Xclang   -load -Xclang $clgPlg_so_F  -Xclang   -add-plugin -Xclang  $clgPlg_name  -c  $test_main_cpp_F 
  ```
参考: https://www.ibm.com/docs/en/xl-c-and-cpp-linux/16.1.0?topic=cla-running-user-defined-actions-by-using-clang-plug-ins

###### 指定环境变量运行插件
 指定环境变量 `clangPlgVerbose_voidFnEndInsertRet=true|false|空即false` 控制 是否在注释中输出完整路径_行号_列号

  ```shell
  clangPlgVerbose_voidFnEndInsertRet=true clang++  -Xclang   -load -Xclang $clgPlg_so_F  -Xclang   -add-plugin -Xclang  $clgPlg_name  -c  $test_main_cpp_F 
  ```

##### clang插件其他运行方法
- `-add-plugin` 尝试 编译、链接、本插件， 本插件 正常修改源文件,  正常编译链接 输出 test_main.elf

      不确定 编译、链接、本插件 之间的先后次序
  ```shell
  clang++  -Xclang   -load -Xclang $clgPlg_so_F  -Xclang   -add-plugin -Xclang  $clgPlg_name   $test_main_cpp_F  -o test_main.elf -mllvm -print-after-all 2>&1 | gedit -
  ```
  
- `-plugin` 尝试 cc1、本插件， 本插件 未运行， 未编译

      不确定 cc1、本插件 之间的先后次序
  ```shell
  clang++ -cc1  -load $clgPlg_so_F   -plugin $clgPlg_name   $test_main_cpp_F
  ```

- `-plugin` 尝试 编译、本插件， 本插件 未运行， 未编译

      不确定 编译、本插件 之间的先后次序
  ```shell
  clang++  -Xclang   -load -Xclang $clgPlg_so_F  -Xclang   -plugin -Xclang  $clgPlg_name  -c  $test_main_cpp_F
  ```
      参考: https://releases.llvm.org/8.0.0/tools/clang/docs/ClangPlugins.html



##### 未能给出clang中各pass执行过程

- `-mllvm -print-after-all` 给出llvm中各pass执行结果?
```shell
clang++           -Xclang    -load -Xclang $clgPlg_so_F  -Xclang   -add-plugin -Xclang  $clgPlg_name   $test_main_cpp_F  -o test_main  -mllvm -print-after-all 2>&1 | gedit -
```

##### `-ccc-print-phases` 打印编译的粗略步骤
```shell
clang++    -ccc-print-phases         $test_main_cpp_F  -o test_main
#            +- 0: input, "/fridaAnlzAp/clang-voidFnEndInsertRet/test_in/test_main.cpp", c++
#         +- 1: preprocessor, {0}, c++-cpp-output
#      +- 2: compiler, {1}, ir
#   +- 3: backend, {2}, assembler
#+- 4: assembler, {3}, object
#5: linker, {4}, image

```