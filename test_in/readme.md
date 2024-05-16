```shell
LLVM15_HOME=/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4
export PATH=$LLVM15_HOME/bin:$PATH
#显示源文件test_main.cpp的语法树
#  彩色输出，方便人工查看
clang -Xclang -ast-dump -fsyntax-only   /fridaAnlzAp/clang-voidFnEndInsertRet/test_in/test_main.cpp
#  文本输出，方便保存
clang -Xclang -ast-dump -fsyntax-only  -fno-color-diagnostics /fridaAnlzAp/clang-voidFnEndInsertRet/test_in/test_main.cpp   | tee /fridaAnlzAp/clang-voidFnEndInsertRet/test_in/test_main.cpp.syntax_tree.txt
```