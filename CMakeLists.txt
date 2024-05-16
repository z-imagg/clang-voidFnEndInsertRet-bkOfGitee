#参考: http://giteaz:3000/anzc/clang-varBE/src/commit/cd31e1039a63444e42d78f2bf9b59eee3be45a95/CMakeLists.txt

cmake_minimum_required(VERSION 3.13.4)

#Makefile繁琐模式
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

#项目名称
project(clangPlugin-FuncStartInsertAsm)

#确保project在find_package之前,
#  否则 有警告："ADD_LIBRARY called with SHARED option but the target platform does not".
#    备注：ClAsCxx.cmake内有find_package。
include("${CMAKE_CURRENT_SOURCE_DIR}/ClAsCxx.cmake")

#添加依赖 libfmt
include("${CMAKE_CURRENT_SOURCE_DIR}/libfmt.cmake")

#添加依赖 libncurses
include("${CMAKE_CURRENT_SOURCE_DIR}/libncurses.cmake")

# 拒绝非15版本的LLVM
if(NOT LLVM_VERSION_MAJOR STREQUAL "15" )
  message(FATAL_ERROR "LLVM版本必须是15,但找到的LLVM版本是${LLVM_VERSION_MAJOR}")
endif()

#使用c++17
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
#只加上一行 编译命令中 无"-std=..."
#加上以下两行 编译命令中 才会出现"-std=gnu++17"
set(CMAKE_CXX_EXTENSIONS ON)
add_compile_options(-std=gnu++17)
#Util.cpp需要c++17,否则其编译会报错:error: no member named 'filesystem' in namespace 'std'

#打印clang的繁琐日志 供调试
#add_compile_options(-v)


#默认构建调试版本
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "描述CMAKE_BUILD_TYPE" FORCE)
endif()

#向CXX_FLAGS添加-Wall ： 启用所有常规警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#向CXX_FLAGS添加-fdiagnostics-color=always ：启用编译器输出中的彩色标记，以提高可读性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
#向CXX_FLAGS添加选项-fno-rtti：clang插件貌似必须无rtti 否则编译报错
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
#clang的内敛函数默认可见
# gcc 内敛函数设置为可见 -fvisibility-inlines-visible, 但 clang的内敛函数默认可见 故而 无需设置
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-visible")  #此行是针对gcc的 , clang无选项 -fvisibility-inlines-visible

# 编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# 添加子模块
add_subdirectory(VFIRPlugin)
add_subdirectory(VFIRAlone)
add_subdirectory(TestAlone)
add_subdirectory(test_in)