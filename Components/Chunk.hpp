//
// Created by seub on 05/03/2021.
//

#pragma once


#include <memory>
#include <vector>
#include "Pixel.hpp"

class Chunk {
public:
    Chunk(int x, int y);

    void update(std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks);
    TileResponse replaceTile(sf::Vector2<int> tilePos, std::shared_ptr<Pixel> newTile);
    bool initialised;

    std::vector<std::shared_ptr<Pixel>> pixels;
    int posX;
    int posY;
private:

    sf::VertexArray wireframe;
};
