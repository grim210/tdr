include_directories("${tdrmain_SOURCE_DIR}/include/")
include_directories("${tdrrenderer_SOURCE_DIR}/include")
include_directories("${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR}")
add_library(tdrrenderer
    glad.cpp
    gltexture.cpp
    rendererutil.cpp
    shaderprogram.cpp
    window.cpp)
