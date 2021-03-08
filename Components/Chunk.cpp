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

void Chunk::update(std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks) {
    auto &scrn = Core::get().screen;

    /** update bottom row */
    auto ptr = pixels.data() + chunk_size * chunk_size - 1;
//    for (int y = 0; y < chunk_size; ++y) {
//        for (int x = 0; x < chunk_size; ++x) {
    for (int y = chunk_size - 1; y >= 0; --y) {
        for (int x = chunk_size - 1; x >= 0; --x) {
            Surrounding surround;

            if (y == chunk_size - 1) {
                if (x == 0) {
                    if (chunks[posX - 1][posY]) {
                        auto ptrLeft = chunks[posX - 1][posY]->pixels.data();
                        surround.ul = ptrLeft + (y - 1) * chunk_size;
                        surround.l = ptrLeft + (y - 0) * chunk_size;
                    }
                    if (chunks[posX - 1][posY + 1]) {
                        auto prtDLeft = chunks[posX - 1][posY + 1]->pixels.data();
                        surround.dl = prtDLeft + chunk_size;
                    }
                }else if (x == chunk_size - 1) {
                    if (chunks[posX + 1][posY]) {
                        auto ptrRight = chunks[posX + 1][posY]->pixels.data();
                        surround.ur = ptrRight + (chunk_size - 2) * chunk_size;
                        surround.r = ptrRight + (chunk_size - 1) * chunk_size;
                    }
                    if (chunks[posX + 1][posY + 1]) {
                        auto prtDRight = chunks[posX + 1][posY + 1]->pixels.data();
                        surround.dr = prtDRight + chunk_size - 1;
                    }
                    surround.ul = ptr - 1 - chunk_size;
                    surround.l = ptr - 1;
                    if (chunks[posX][posY + 1]) {
                        auto ptrDown = chunks[posX][posY + 1]->pixels.data();
                        surround.d = ptrDown + x;
                        surround.dl = ptrDown + x - 1;
                    }
                } else {
                    surround.ul = ptr - 1 - chunk_size;
                    surround.l = ptr - 1;
                    if (chunks[posX][posY + 1]) {
                        auto ptrDown = chunks[posX][posY + 1]->pixels.data();
                        surround.d = ptrDown + x;
                        surround.dl = ptrDown + x - 1;
                    }
                }
            } else {
                if (x == 0) {
                    if (chunks[posX - 1][posY]) {
                        auto ptrLeft = chunks[posX - 1][posY]->pixels.data();
                        surround.ul = ptrLeft + (y - 1) * chunk_size;
                        surround.l = ptrLeft + (y - 0) * chunk_size;
                        surround.dl = ptrLeft + (y + 1) * chunk_size;
                    }
                } else {
                    surround.ul = ptr - 1 - chunk_size;
                    surround.l = ptr - 1;
                    surround.dl = ptr - 1 + chunk_size;
                }
                surround.d = ptr + chunk_size;
            }
            surround.c = ptr;
            (*ptr)->update(surround, x, y, scrn, posX, posY);
            --ptr;
        }
    }
    /** update center rows */
    /** update up row */


//    for (auto const &elem : pixels)
//        elem->processed = false;
}
//void Chunk::update() {
//    auto ptr = pixels.data() + chunk_size * chunk_size - 1;
//    auto &scrn = Core::get().screen;
//    for (int i = chunk_size * chunk_size - 1; i >= 0 ; --i, --ptr)
//        (*ptr)->update(pixels.data(), i % chunk_size, i / chunk_size, scrn, posX, posY);
//    for (auto const &elem : pixels)
//        elem->processed = false;
//}
