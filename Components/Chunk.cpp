//
// Created by seub on 05/03/2021.
//

#include <iostream>
#include "Chunk.hpp"
#include "../Core/Core.hpp"

Chunk::Chunk(int cX, int cY): posX(cX), posY(cY) {
    pixels.resize(chunk_size*chunk_size);
    auto ptr = pixels.data();
    auto &core = Core::get();
    for (int y = 0; y < chunk_size; ++y) {
        for (int x = 0; x < chunk_size; ++x) {
            *ptr = core.getTile(cX*chunk_size+x, cY*chunk_size+y);
            ++ptr;
        }
    }
}

void Chunk::update() {
    auto ptr = pixels.data() + chunk_size * chunk_size - 1;
    auto &scrn = Core::get().screen;
    for (int i = chunk_size * chunk_size - 1; i >= 0 ; --i, --ptr)
        (*ptr)->update(pixels.data(), i % chunk_size, i / chunk_size, scrn, posX, posY);
    for (auto const &elem : pixels)
        elem->processed = false;
}
