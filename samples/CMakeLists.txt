project(tdrsamples CXX)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.0 REQUIRED)

message("GLFW3 library dir: ${GLFW3_LIBRARY_DIR}")
message("GLFW3 include dir: ${GLFW3_INCLUDE_DIR}")
message("GLFW3 library:     ${GLFW3_LIBRARY}")

subdirs(sample01)
subdirs(sample02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
