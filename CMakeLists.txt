cmake_minimum_required(VERSION 3.0)
project(tdr CXX)
enable_language(C)

# Set version
set(tdr_VERSION_MAJOR 0)
set(tdr_VERSION_MINOR 0)
set(tdr_VERSION_PATCH 2)
set(tdr_VERSION_STRING "0.0.2")

option(BUILD_SYSTEM_DEBUG "Output debug info from CMake as it configure."
    FALSE)

# Check for C++11 compiler
# TODO: This should be more robust.  The most popular compilers are GNU G++,
# Clang and Visual C++.  We should support these compilers at the very least.
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX)
    CHECK_CXX_COMPILER_FLAG(-std=c++11 SUPPORTS_CXX11)
else()
    # Assume the individual building the software knows what they're doing.
    set(SUPPORTS_CXX11 TRUE)
    message("** Warning: Only GNU's C++ compiler is officially supported.")
endif()

if(SUPPORTS_CXX11)
    # This is a GNU-ism..  Potentially works for Clang as well.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "!! Error: Failed to find C++11 capable compiler.")
endif()

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -DTDR_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(OpenGL)
find_package(PkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "Failed to find SDL2!")
else()
    if(BUILD_SYSTEM_DEBUG)
        message("SDL2 Libraries: ${SDL2_LIBRARIES}")
        message("SDL2 Library Dirs: ${SDL2_LIBRARY_DIRS}")
        message("SDL2 LDFLAGS: ${SDL2_LDFLAGS}")
        message("SDL2 LDFLAGS Other: ${SDL2_LDFLAGS_OTHER}")
        message("SDL2 Include Dirs: ${SDL2_INCLUDE_DIRS}")
        message("SDL2 CFLAGS: ${SDL2_CFLAGS}")
        message("SDL2 CFLAGS Other: ${SDL2_CLFAGS_OTHER}")
    endif()
endif()

# There needs to be a way to determine if the dependencies are satisified
# or not and then conditionally include that stuff.
subdirs(main renderer samples tests)
