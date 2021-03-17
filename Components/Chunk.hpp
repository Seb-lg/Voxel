//
// Created by seub on 05/03/2021.
//
#pragma once

#include <memory>
#include <vector>
#include "../Perlin/PerlinNoise.hpp"
#include "../Globals.hpp"
#include "Tile.hpp"

class Map;

class Chunk {
public:
    Chunk(sf::Vector2i chunkPos, siv::PerlinNoise perlin);

    void update(Map &map);
    TileResponse replaceTile(sf::Vector2<int> tilePos, std::shared_ptr<Tile> newTile, bool override);

    bool initialised;
    sf::Vector2i pos;
    sf::VertexArray wireframe;

    std::vector<std::shared_ptr<Tile>> pixels;
    sf::VertexArray vertices;

private:

    std::shared_ptr<Tile> createTileFromPerlin(sf::Vector2i pos, siv::PerlinNoise perlin);
};
