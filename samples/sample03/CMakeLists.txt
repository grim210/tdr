project(sample_03 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
link_directories("${tdrmain_SOURCE_DIR}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")

add_executable(sample_03 sample03.cpp)
target_include_directories(sample_03 PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(sample_03 tdrrenderer tdrmain
    ${SDL2_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${CMAKE_DL_LIBS}
)

file(COPY shaders DESTINATION .)
file(COPY textures DESTINATION .)
