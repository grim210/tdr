project(sample_02 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
link_directories("${tdrmain_SOURCE_DIR}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")

message("tdrmain_SOURCE_DIR: ${tdrmain_SOURCE_DIR}")

add_executable(sample_02 sample02.cpp)
target_include_directories(sample_02 PUBLIC ${GLFW3_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
target_link_libraries(sample_02 tdrrenderer tdrmain ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY} ${CMAKE_DL_LIBS})

file(COPY shaders DESTINATION .)
file(COPY textures DESTINATION .)
