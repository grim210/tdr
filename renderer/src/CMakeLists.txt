include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include")
include_directories("${GLFW3_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR}")
add_library(tdrrenderer
    glad.cpp
    gltexture.cpp
    rendererutil.cpp
    shaderprogram.cpp
    window.cpp)
