//
// Created by seub on 05/03/2021.
//

#include "Chunk.hpp"
#include "../Core/Core.hpp"

Chunk::Chunk(int cX, int cY): pixels(chunk_size * chunk_size) {
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

}
