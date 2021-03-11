//
// Created by seub on 05/03/2021.
//
#include <iostream>
#include "Chunk.hpp"
#include "../Core/Core.hpp"


Chunk::Chunk(sf::Vector2i chunkPos, siv::PerlinNoise perlin):
    pos(chunkPos), wireframe(sf::LineStrip, 4)
{
    pixels.resize(chunk_size*chunk_size);
    auto ptr = pixels.data();
    for (int y = 0; y < chunk_size; ++y) {
        for (int x = 0; x < chunk_size; ++x) {
            *ptr = createTileFromPerlin(
                sf::Vector2i(
                    chunkPos.x * chunk_size + x,
                    chunkPos.y * chunk_size + y
                ),
                perlin
            );
            ++ptr;
        }
    }
    wireframe[0].position = sf::Vector2f(0, 0);
    wireframe[1].position = sf::Vector2f(chunk_size * pixel_size, 0);
    wireframe[2].position = sf::Vector2f(chunk_size * pixel_size, chunk_size * pixel_size);
    wireframe[3].position = sf::Vector2f(0, chunk_size * pixel_size);

    wireframe[0].color = sf::Color(199, 199, 199);
    wireframe[1].color = sf::Color(199, 199, 199);
    wireframe[2].color = sf::Color(199, 199, 199);
    wireframe[3].color = sf::Color(199, 199, 199);
}

std::shared_ptr<Pixel> Chunk::createTileFromPerlin(sf::Vector2i pos, siv::PerlinNoise perlin) {
    // Called by the Chunk class constructor, will determine which pixel type
    // is created, based on the perlin noise
    static const double frequency = 50;
    static const int octaves = 20;

    auto noise = perlin.accumulatedOctaveNoise2D_0_1(
        pos.x / frequency,
        pos.y / frequency,
        octaves
    );
    if (noise < 0.3)
        return std::make_shared<Concrete>(pos);
    if (noise < 0.4)
        return std::make_shared<Sand>(pos);
    if (noise > 0.8)
        return std::make_shared<Water>(pos);
    return std::make_shared<Pixel>(pos);
}

TileResponse Chunk::replaceTile(sf::Vector2<int> tilePos, std::shared_ptr<Pixel> newTile, bool override) {
    int idx = tilePos.y * chunk_size + tilePos.x;
    if (idx > chunk_size * chunk_size || idx < 0)
        return TileResponse::OOB;
    if (pixels[idx]->type == newTile->type)
        return TileResponse::ALREADY_CREATED;
    if (!override && pixels[idx]->type != PixelType::Air)
        return TileResponse::NOT_EMPTY;
    pixels[idx] = newTile;
    return TileResponse::CREATED;
}

void Chunk::update(std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks) {
    auto &scrn = Core::get().screen;
    /** update bottom row */
    auto ptr = pixels.data() + chunk_size * chunk_size - 1;
    for (int y = chunk_size - 1; y >= 0; --y) {
        for (int x = chunk_size - 1; x >= 0; --x) {
            if (ptr->get()->type == PixelType::Air) {
                --ptr;
                continue;
            }
            Surrounding surround(ptr);

            /** Checking Y axis first */
            /** First row */
            if (y == 0) {
                /** Checking X axis */
                /** First column */
                if (x == 0) {
                    // left
                    if (chunks[pos.x - 1][pos.y]) {
                        auto left = chunks[pos.x - 1][pos.y]->pixels.data();
                        surround.l = left + chunk_size - 1;
                        surround.dl = left + 2 * chunk_size - 1;
                    } else {
                        surround.l = nullptr;
                        surround.dl = nullptr;
                    }
                    // up Left
                    if (chunks[pos.x - 1][pos.y - 1]) {
                        auto up = chunks[pos.x - 1][pos.y - 1]->pixels.data();
                        surround.ul = up + chunk_size* chunk_size - 1;
                    } else {
                        surround.ul = nullptr;
                    }
                    // up
                    if (chunks[pos.x][pos.y - 1]) {
                        auto up = chunks[pos.x][pos.y - 1]->pixels.data();
                        surround.u = up + x + (chunk_size - 1) * chunk_size;
                        surround.ur = up + x + 1 + (chunk_size - 1) * chunk_size;
                    } else {
                        surround.u = nullptr;
                        surround.ur = nullptr;
                    }
                }
                /** Last column */
                else if (x == chunk_size - 1) {
                    // right
                    if (chunks[pos.x + 1][pos.y]) {
                        auto right = chunks[pos.x + 1][pos.y]->pixels.data();
                        surround.r = right;
                        surround.dr = right + chunk_size;
                    } else {
                        surround.r = nullptr;
                        surround.dr = nullptr;
                    }
                    // right up
                    if (chunks[pos.x + 1][pos.y - 1]) {
                        auto up = chunks[pos.x + 1][pos.y - 1]->pixels.data();
                        surround.ur = up + (chunk_size - 1) * chunk_size;
                    } else {
                        surround.ur = nullptr;
                    }
                    // up
                    if (chunks[pos.x][pos.y - 1]) {
                        auto up = chunks[pos.x][pos.y - 1]->pixels.data();
                        surround.ul = up + x - 1 + (chunk_size - 1) * chunk_size;
                        surround.u = up + x + (chunk_size - 1) * chunk_size;
                    } else {
                        surround.ul = nullptr;
                        surround.u = nullptr;
                    }
                }
                /** Intermediate column */
                else {
                    // up
                    if (chunks[pos.x][pos.y - 1]) {
                        auto up = chunks[pos.x][pos.y - 1]->pixels.data();
                        surround.ul = up + x - 1 + (chunk_size - 1) * chunk_size;
                        surround.u = up + x + (chunk_size - 1) * chunk_size;
                        surround.ur = up + x + 1 + (chunk_size - 1) * chunk_size;
                    } else {
                        surround.ul = nullptr;
                        surround.u = nullptr;
                        surround.ur = nullptr;
                    }
                }
            }
            /** Last row */
            else if (y == chunk_size - 1) {
                /** Checking X axis */
                /** First column */
                if (x == 0) {
                    // left
                    if (chunks[pos.x - 1][pos.y]) {
                        auto left = chunks[pos.x-1][pos.y]->pixels.data();
                        surround.ul = left + (y - 1) * chunk_size - 1;
                        surround.l = left + y * chunk_size - 1;
                    } else {
                        surround.ul = nullptr;
                        surround.l = nullptr;
                    }
                    // down Left
                    if (chunks[pos.x - 1][pos.y + 1]) {
                        auto down = chunks[pos.x - 1][pos.y + 1]->pixels.data();
                        surround.dl = down + chunk_size - 1;
                    } else {
                        surround.dl = nullptr;
                    }
                    // down
                    if (chunks[pos.x][pos.y + 1]) {
                        auto down = chunks[pos.x][pos.y + 1]->pixels.data();
                        surround.d = down + x;
                        surround.dr = down + x + 1;
                    } else {
                        surround.d = nullptr;
                        surround.dr = nullptr;
                    }
                }
                    /** Last column */
                else if (x == chunk_size - 1) {
                    // right
                    if (chunks[pos.x + 1][pos.y]) {
                        auto right = chunks[pos.x + 1][pos.y]->pixels.data();
                        surround.r = right + (chunk_size - 1) * chunk_size;
                    } else {
                        surround.r = nullptr;
                    }
                    // right down
                    if (chunks[pos.x + 1][pos.y + 1]) {
                        auto down = chunks[pos.x + 1][pos.y + 1]->pixels.data();
                        surround.dr = down;
                    } else {
                        surround.dr = nullptr;
                    }
                    // down
                    if (chunks[pos.x][pos.y + 1]) {
                        auto up = chunks[pos.x][pos.y + 1]->pixels.data();
                        surround.d = up + x;
                        surround.dl = up + x - 1;
                    } else {
                        surround.d = nullptr;
                        surround.dl = nullptr;
                    }
                }
                    /** Intermediate column */
                else {
                    // down
                    if (chunks[pos.x][pos.y + 1]) {
                        auto down = chunks[pos.x][pos.y + 1]->pixels.data();
                        surround.dl = down + x - 1;
                        surround.d = down + x;
                        surround.dr = down + x + 1;
                    } else {
                        surround.dl = nullptr;
                        surround.d = nullptr;
                        surround.dr = nullptr;
                    }
                }
            }
            /** Intermediate row */
            else {
                /** Checking X axis */
                /** First column */
                if (x == 0) {
                    if (chunks[pos.x - 1][pos.y]) {
                        auto left = chunks[pos.x - 1][pos.y]->pixels.data();
                        surround.ul = left + (y - 0) * chunk_size - 1;
                        surround.l = left + (y+1) * chunk_size - 1;
                        surround.dl = left + (y + 2) * chunk_size - 1;
                    } else {
                        surround.ul = nullptr;
                        surround.l = nullptr;
                        surround.dl = nullptr;
                    }
                }
                    /** First column */
                else if (x == chunk_size - 1) {
                    if (chunks[pos.x + 1][pos.y]) {
                        auto right = chunks[pos.x + 1][pos.y]->pixels.data();
                        surround.ur = right + chunk_size * (y - 1);
                        surround.r = right + chunk_size * (y);
                        surround.dr = right + chunk_size * (y + 1);
                    } else {
                        surround.ur = nullptr;
                        surround.r = nullptr;
                        surround.dr = nullptr;
                    }
                }
                /** Intermediate column handled by the constructor */
            }

            surround.c = ptr;
            (*ptr)->update(surround, sf::Vector2i(x, y), sf::Vector2i(pos.x, pos.y));
            --ptr;
        }
    }
}
