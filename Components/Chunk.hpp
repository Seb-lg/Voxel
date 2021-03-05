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

    void update();
private:

    std::vector<std::shared_ptr<Pixel>> pixels;
    int posX;
    int posY;
};


