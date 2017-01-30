project(tdrmeshtool CXX)

set(TDRMESHTOOL_VERSION_MAJOR ${tdr_VERSION_MAJOR})
set(TDRMESHTOOL_VERSION_MINOR ${tdr_VERSION_MINOR})
set(TDRMESHTOOL_VERSION_PATCH ${tdr_VERSION_PATCH})

configure_file(
    "${tdrmeshtool_SOURCE_DIR}/include/config.h.in"
    "${tdrmeshtool_SOURCE_DIR}/include/config.h"
)

include_directories("${tdrmeshtool_SOURCE_DIR}/include/")
include_directories("${tdrmain_SOURCE_DIR}/include/")
link_directories("${tdrmain_SOURCE_DIR}/src/")

add_executable(tdrmeshtool main.cpp)
target_link_libraries(tdrmeshtool tdrmain)

