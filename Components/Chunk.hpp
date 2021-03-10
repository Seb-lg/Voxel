//
// Created by seub on 05/03/2021.
//

#pragma once


#include <memory>
#include <vector>
#include "../Perlin/PerlinNoise.hpp"
#include "Pixel.hpp"

class Chunk {
public:
    Chunk(sf::Vector2i chunkPos, siv::PerlinNoise perlin);

    void update(std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks);
    std::shared_ptr<Pixel> createTileFromPerlin(sf::Vector2i pos, siv::PerlinNoise perlin);
    TileResponse replaceTile(sf::Vector2<int> tilePos, std::shared_ptr<Pixel> newTile, bool override);
    bool initialised;

    std::vector<std::shared_ptr<Pixel>> pixels;
    sf::Vector2i pos;
    sf::VertexArray wireframe;

};
