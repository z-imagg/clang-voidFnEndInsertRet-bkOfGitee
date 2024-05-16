

MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#CMAKE_SOURCE_DIR: /pubx/clang-var/

MESSAGE("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#CMAKE_CURRENT_SOURCE_DIR: /pubx/clang-var/tools

set(VFIRAlone_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/ActMain.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../VFIRAlone/VarMain.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/Util.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/LocId.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/FnVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VarDeclVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/RangeHasMacroAstVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/CollectIncMacro_PPCb.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VFIRAstCnsm.cpp"
)

    # Create a library corresponding to 'plugin'
    add_executable(
        VFIRAlone
      ${VFIRAlone_SOURCES}
      )

    # Configure include directories for 'tool'
    target_include_directories(
    VFIRAlone
      PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/include"
    "${LIBFMT_INCLUDE}"
    )

    # Link in the required libraries
    target_link_libraries(
    VFIRAlone
      "clangTooling"
   "${LIBFMT_STATIC}"  #/pubx/fmt/build/libfmt.a
    )
