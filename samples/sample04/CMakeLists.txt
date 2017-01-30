project(sample_04 CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include/")
link_directories("${tdrmain_SOURCE_DIR}/src")
link_directories("${tdrrenderer_SOURCE_DIR}/src")

add_executable(sample_04 sample04.cpp)
target_include_directories(sample_04 PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(sample_04 tdrrenderer tdrmain
    ${SDL2_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${CMAKE_DL_LIBS}
)

file(COPY shaders DESTINATION .)
file(COPY textures DESTINATION .)
file(COPY cube.json DESTINATION .)
