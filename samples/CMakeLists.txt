project(tdrsamples CXX)
find_package(OpenGL REQUIRED)

if(UNIX)
    find_package(glfw3 3.0 REQUIRED)
    message("GLFW3 library dir: ${GLFW3_LIBRARY_DIR}")
    message("GLFW3 include dir: ${GLFW3_INCLUDE_DIR}")
    message("GLFW3 library:     ${GLFW3_LIBRARY}")
elseif(WIN32)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    message("GLFW3 library dir: ${GLFW_LIBRARIES}")
    message("GLFW3 includr dir: ${GLFW_INCLUDE_DIRS}")
endif()


subdirs(sample01)
subdirs(sample02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
