cmake_minimum_required(VERSION 3.0)
project(tdr CXX)
enable_language(C)

set(tdr_VERSION_MAJOR 0)
set(tdr_VERSION_MINOR 0)
set(tdr_VERSION_PATCH 2)

# Check for C++11 compiler
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX)
    CHECK_CXX_COMPILER_FLAG(-std=c++11 SUPPORTS_CXX11)
else()
    # Assume the individual building the software knows what they're doing.
    set(SUPPORTS_CXX11 TRUE)
    message("** Warning: Only GNU's C++ compiler is officially supported.")
endif()

if(SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "!! Error: Failed to find C++11 capable compiler.")
endif()

# Find OpenGL, necessary for tdrrenderer, and the samples
find_package(OpenGL)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -DTDR_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(PkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

if(SDL2_FOUND)
    message("SDL2 Libraries: ${SDL2_LIBRARIES}")
    message("SDL2 Library Dirs: ${SDL2_LIBRARY_DIRS}")
    message("SDL2 LDFLAGS: ${SDL2_LDFLAGS}")
    message("SDL2 LDFLAGS Other: ${SDL2_LDFLAGS_OTHER}")
    message("SDL2 Include Dirs: ${SDL2_INCLUDE_DIRS}")
    message("SDL2 CFLAGS: ${SDL2_CFLAGS}")
    message("SDL2 CFLAGS Other: ${SDL2_CLFAGS_OTHER}")
else()
    message(FATAL_ERROR "Failed to find SDL2!")
endif()


# There needs to be a way to determine if the dependencies are satisified
# or not and then conditionally include that stuff.
subdirs(main renderer samples)
