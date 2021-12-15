cmake_minimum_required(VERSION 3.17)
project(doolhof_triangle)

set(CMAKE_CXX_STANDARD 17)

add_executable(doolhof_triangle src/main.cpp src/maze.cpp src/point.cpp src/triangle.cpp src/wall.cpp)

target_link_libraries(doolhof_triangle sfml-graphics sfml-window sfml-system)


