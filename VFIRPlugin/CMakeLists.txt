# THE LIST OF PLUGINS AND THE CORRESPONDING SOURCE FILES
# ======================================================


set(VFIR_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VFIRAstAct.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/Util.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../base_home/impl/base/LocId.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/FnVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/PPCb.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/VFIR/VFIRAstCnsm.cpp"
)


    # Create a library corresponding to 'plugin'
    add_library(
            VFIRPlugin
      SHARED
      ${VFIR_SOURCES}
      )

target_link_libraries(VFIRPlugin
        "${LIBFMT_STATIC}"   #/pubx/fmt/build/libfmt.a
        )

    # Configure include directories for 'plugin'
    target_include_directories(VFIRPlugin
      PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../base_home/include"
      "${LIBFMT_INCLUDE}"
    )

