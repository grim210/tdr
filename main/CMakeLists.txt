project(tdrmain CXX)
subdirs(src)

set(TDRMAIN_VERSION_MAJOR ${tdr_VERSION_MAJOR})
set(TDRMAIN_VERSION_MINOR ${tdr_VERSION_MINOR})
set(TDRMAIN_VERSION_PATCH ${tdr_VERSION_PATCH})

configure_file(
    "${tdrmain_SOURCE_DIR}/include/config.h.in"
    "${tdrmain_SOURCE_DIR}/include/config.h"
)
