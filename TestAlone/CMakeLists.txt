

MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#CMAKE_SOURCE_DIR: /pubx/clang-voidFnEndInsertRet/

MESSAGE("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#CMAKE_CURRENT_SOURCE_DIR: /pubx/clang-voidFnEndInsertRet/tools

set(TestAlone_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../TestAlone/TestAloneMain.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/RangeHasMacroAstVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/PPCb.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/Util.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/LocId.cpp"

)

    # Create a library corresponding to 'plugin'
    add_executable(
        TestAlone
      ${TestAlone_SOURCES}
      )

    # Configure include directories for 'tool'
    target_include_directories(
    TestAlone
      PRIVATE
    "${LIBFMT_INCLUDE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"  # == /fridaAnlzAp/clang-voidFnEndInsertRet/include
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/include/"
    )

    # Link in the required libraries
    target_link_libraries(
    TestAlone
      "clangTooling"
   "${LIBFMT_STATIC}"  #/pubx/fmt/build/libfmt.a
    )
