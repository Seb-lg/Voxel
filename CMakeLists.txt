cmake_minimum_required(VERSION 3.18)
project(Voxel)

set(CMAKE_CXX_STANDARD 20)

add_executable(Voxel main.cpp
        Core/Core.cpp
        Core/Core.hpp configuration.hpp Core/Perlin/PerlinNoise.hpp Components/Pixel.hpp Components/Chunk.cpp Components/Chunk.hpp)

find_package(SFML 2.5 REQUIRED audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(Voxel sfml-graphics sfml-audio sfml-system sfml-window)