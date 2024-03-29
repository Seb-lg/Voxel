cmake_minimum_required(VERSION 3.18)
project(Voxel)

set(CMAKE_CXX_STANDARD 20)

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

add_executable(Voxel
        main.cpp

        Core/Core.hpp
        Core/Core.cpp
        Core/inputs.cpp
        Core/chunkHandling.cpp
        Core/shaders.cpp

        Perlin/PerlinNoise.hpp

        conf.hpp

        Components/DynamicPixels.cpp
        Components/StaticPixels.cpp
        Components/Tile.hpp
        Components/Chunk.cpp
        Components/Chunk.hpp
        Components/ChunkHelper.cpp Components/ChunkHelper.hpp Core/Map.cpp Core/Map.hpp Globals.hpp utils/ThreadPool.cpp utils/ThreadPool.hpp)

find_package(Threads)
target_link_libraries(Voxel ${CMAKE_THREAD_LIBS_INIT})

find_package(SFML 2.5 REQUIRED audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(Voxel sfml-graphics sfml-audio sfml-system sfml-window)
