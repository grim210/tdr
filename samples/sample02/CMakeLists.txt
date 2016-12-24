project(sample_02 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
include_directories("${GLFW3_INCLUDE_DIR}")
link_directories("${tdrmain_SOURCE_DIR}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")
link_directories("${GLFW3_LIBRARY_DIR}")

add_executable(sample_02 sample02.cpp)
target_link_libraries(sample_02 tdrmain tdrrenderer)

file(COPY cube.json DESTINATION .)
