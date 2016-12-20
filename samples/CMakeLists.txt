project(tdrsamples CXX)
find_package(OpenGL REQUIRED)

option(ARCH_32 "Target 32-bit libraries." TRUE)

if(UNIX)
    find_package(glfw3 3.0 REQUIRED)
    message("GLFW3 library dir: ${GLFW3_LIBRARY_DIR}")
    message("GLFW3 include dir: ${GLFW3_INCLUDE_DIR}")
    message("GLFW3 library:     ${GLFW3_LIBRARY}")
elseif(MSYS)
    message("Manually specifying GLFW3 location for MSYS.")
    if(ARCH_32)
        set(GLFW3_LIBRARY_DIR "-L/mingw32/lib")
        set(GLFW3_INCLDUR_DIR "-I/mingw32/include")
        set(GLFW3_LIBRARY "-lglfw3 -lgdi32")
    else()
        set(GLFW3_LIBRARY_DIR "-L/mingw64/lib")
        set(GLFW3_INCLDUR_DIR "-I/mingw64/include")
        set(GLFW3_LIBRARY "-lglfw3 -lgdi32")
    endif()
endif()


subdirs(sample01)
subdirs(sample02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
