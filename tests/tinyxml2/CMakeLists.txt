project(xmltest CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
link_directories("${tdrmain_SOURCE_DIR}/src/")

add_executable(xmltest xmltest.cpp)
target_link_libraries(xmltest tdrmain ${SDL2_LIBRARIES})

file(COPY resources DESTINATION .)
