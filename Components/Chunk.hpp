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
    bool initialised;

    std::vector<std::shared_ptr<Pixel>> pixels;
private:
    int posX;
    int posY;
};


