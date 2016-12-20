project(tdrsamples CXX)
subdirs(sample01)
subdirs(sample02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
