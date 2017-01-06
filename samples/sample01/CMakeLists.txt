project(sample_01 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
include_directories("${SDL2_INCLUDE_DIRS}")

link_directories("${tdrmain_SOURCE_DIR}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")
link_directories("${SDL2_LIBRARY_DIRS}")

add_executable(sample_01 sample01.cpp)
target_link_libraries(sample_01 tdrmain tdrrenderer ${SDL2_LIBRARIES})
