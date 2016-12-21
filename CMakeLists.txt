cmake_minimum_required(VERSION 3.0)
project(tdr CXX)

set(tdr_VERSION_MAJOR 0)
set(tdr_VERSION_MINOR 0)
set(tdr_VERSION_PATCH 1)

message("System: ${CMAKE_SYSTEM_NAME}")

# Check for C++11 compiler
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-std=c++11 SUPPORTS_CXX11)
if(NOT SUPPORTS_CXX11)
    message(FATAL_ERROR "Failed to find C++11 capable compiler.")
endif()

# There needs to be a better way of doing this..
option(ARCH_32 "Target 32-bit libraries." TRUE)

# Find OpenGL, necessary for tdrrenderer, and the samples
find_package(OpenGL)

# Find GLFW3.. which seems to be pretty frustrating.  Necessary
# for tdrrenderer and the samples.
if(UNIX)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
        find_package(PkgConfig)
        pkg_search_module(GLFW REQUIRED glfw3)
        set(GLFW3_LIBRARY_DIR "${GLFW_LIBRARY_DIRS}")
        set(GLFW3_LIBRARY "${GLFW_LIBRARIES}")
        set(GLFW3_INCLUDE_DIR "${GLFW_INCLUDE_DIRS}")
    else()
        find_package(glfw3 3.0 REQUIRED)
    endif()
elseif(MSYS)
    if(ARCH_32)
        set(GLFW3_LIBRARY_DIR "-L/mingw32/lib")
        set(GLFW3_INCLUDE_DIR "-I/mingw32/include")
        set(GLFW3_LIBRARY "-lglfw3 -lgdi32")
    else()
        set(GLFW3_LIBRARY_DIR "-L/mingw64/lib")
        set(GLFW3_INCLUDE_DIR "-I/mingw64/include")
        set(GLFW3_LIBRARY "-lglfw -lgdi32")
    endif()
endif()

# There needs to be a way to determine if the dependencies are satisified
# or not and then conditionally include that stuff.
subdirs(main renderer samples)
