project(sample_01 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
include_directories("${GLFW3_INCLUDE_DIR}")
link_directories("${tdrmain_SOURCE_DIR}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")
link_directories("${GLFW3_LIBRARY_DIR}")

add_executable(sample_01 sample01.cpp)
target_link_libraries(sample_01 tdrmain tdrrenderer)
