project(tdrrenderer CXX)
subdirs(src)

set(TDRRENDERER_VERSION_MAJOR ${tdr_VERSION_MAJOR})
set(TDRRENDERER_VERSION_MINOR ${tdr_VERSION_MINOR})
set(TDRRENDERER_VERSION_PATCH ${tdr_VERSION_PATCH})
set(TDRRENDERER_VERSION_STRING
    "${tdr_VERSION_MAJOR}.${tdr_VERSION_MINOR}.${tdr_VERSION_PATCH}")

configure_file(
    "${tdrrenderer_SOURCE_DIR}/include/config.h.in"
    "${tdrrenderer_SOURCE_DIR}/include/config.h"
)
