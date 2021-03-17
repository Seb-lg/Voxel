//
// Created by seub on 3/17/21.
//

#pragma once

#include <map>
#include <memory>
#include "../Perlin/PerlinNoise.hpp"
#include "../Components/Chunk.hpp"

class Map {
public:
    Map();

    siv::PerlinNoise perlin;
    std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks;

    void swapPixels(PixelSwitch &nextPixelData);
};
