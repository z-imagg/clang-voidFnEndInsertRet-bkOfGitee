# THE LIST OF PLUGINS AND THE CORRESPONDING SOURCE FILES
# ======================================================


set(VFIR_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VarAstAct.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/Util.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/LocId.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/FnVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VarDeclVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/RangeHasMacroAstVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/CollectIncMacro_PPCb.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VarAstCnsm.cpp"
)


    # Create a library corresponding to 'plugin'
    add_library(
            VarPlugin
      SHARED
      ${VFIR_SOURCES}
      )

target_link_libraries(VarPlugin
        "${LIBFMT_STATIC}"   #/pubx/fmt/build/libfmt.a
        )

    # Configure include directories for 'plugin'
    target_include_directories(VarPlugin
      PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/include"
      "${LIBFMT_INCLUDE}"
    )

