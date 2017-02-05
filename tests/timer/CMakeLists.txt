project(timer CXX)

include_directories("${tdrmain_SOURCE_DIR}/include/")
link_directories("${tdrmain_SOURCE_DIR}/src/")

add_executable(timer
    main.cpp
)

target_link_libraries(timer
    tdrmain
)
