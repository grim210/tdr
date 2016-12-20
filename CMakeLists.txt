cmake_minimum_required(VERSION 3.0)
project(tdr CXX)

# Check for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-std=c++11 SUPPORTS_CXX11)
if(NOT SUPPORTS_CXX11)
    message(FATAL_ERROR "Failed to find C++11 capable compiler.")
endif()

# Add the tdrmain directory
subdirs(main renderer samples)
