project(_sample_02 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
include_directories("${SDL2_INCLUDE_DIRS}")
link_directories("${tdrmain_SOURCE_DIRS}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")
link_directories("${SDL2_LIBRARY_DIRS}")

add_executable(_sample_02 sample02.cpp)
target_link_libraries(_sample_02 tdrmain tdrrenderer ${SDL2_LIBRARIES})

file(COPY cube.json DESTINATION .)
