//
// Created by seub on 3/12/21.
//

#pragma once
#include "Pixel.hpp"
#include "Chunk.hpp"

#define cptr std::shared_ptr<Pixel>*

namespace ChunkHelper {
    cptr LookBasedOnVelocity(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos, std::map<int, std::map<int, std::shared_ptr<Chunk>>> &map);
}
