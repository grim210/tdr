project(sample_02 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
include_directories("${SDL2_INCLUDE_DIRS}")
link_directories("${tdrmain_SOURCE_DIRS}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")
link_directories("${SDL2_LIBRARY_DIRS}")

add_executable(sample_02 sample02.cpp)
target_link_libraries(sample_02 tdrmain tdrrenderer ${SDL2_LIBRARIES})

file(COPY scene.xml DESTINATION .)
file(COPY cube.colors.bin DESTINATION .)
file(COPY cube.verts.bin DESTINATION .)
file(COPY vshader.vs DESTINATION .)
file(COPY fshader.fs DESTINATION .)
