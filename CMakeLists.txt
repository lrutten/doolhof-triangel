cmake_minimum_required(VERSION 3.17)
project(doolhof_triangle)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-g")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(doolhof_triangle src/main.cpp src/maze.cpp src/point.cpp src/triangle.cpp src/wall.cpp)

target_link_libraries(doolhof_triangle sfml-graphics sfml-window sfml-system)


