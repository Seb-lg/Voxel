//
// Created by seub on 05/03/2021.
//
#include <iostream>
#include "Chunk.hpp"
#include "../Core/Core.hpp"


Chunk::Chunk(sf::Vector2i chunkPos, siv::PerlinNoise perlin):
    pos(chunkPos), wireframe(sf::LineStrip, 4)
{
    // All std::Quads will be stored in a single sf::VertexArray so we can draw
    // the whole chunk in one pass
    // We set all the Vertex positions at init (here) and we'll only change colors
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(CHUNK_SIZE * CHUNK_SIZE * 4);
    // The particles "data" (position, type, life etc) is stored in a separate vector (pixels)
    pixels.resize(CHUNK_SIZE * CHUNK_SIZE);
    // It's IMPERATIVE to change BOTH the colors in "vertices" and the objects in "pixels"
    // EVERYTIME
    auto ptr = pixels.data();
    for (int y = 0; y < CHUNK_SIZE; ++y) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            // Create the Pixel object and data
            *ptr = createTileFromPerlin(
                sf::Vector2i(
                    chunkPos.x * CHUNK_SIZE + x,
                    chunkPos.y * CHUNK_SIZE + y
                ),
                perlin
            );
            // get a pointer to the current tile's sf::quad
            sf::Vertex* quad = &vertices[(x + y * CHUNK_SIZE) * 4];
            // define its 4 corners
            quad[0].position = sf::Vector2f((chunkPos.x * CHUNK_SIZE + x) * PIXEL_SIZE, (chunkPos.y * CHUNK_SIZE + y) * PIXEL_SIZE);
            quad[1].position = sf::Vector2f((chunkPos.x * CHUNK_SIZE + x+1) * PIXEL_SIZE, (chunkPos.y * CHUNK_SIZE + y) * PIXEL_SIZE);
            quad[2].position = sf::Vector2f((chunkPos.x * CHUNK_SIZE + x+1) * PIXEL_SIZE, (chunkPos.y * CHUNK_SIZE + y+1) * PIXEL_SIZE);
            quad[3].position = sf::Vector2f((chunkPos.x * CHUNK_SIZE + x) * PIXEL_SIZE, (chunkPos.y * CHUNK_SIZE + y+1) * PIXEL_SIZE);
            // Set color from the created Pixel object
            quad[0].color = (*ptr)->color;
            quad[1].color = (*ptr)->color;
            quad[2].color = (*ptr)->color;
            quad[3].color = (*ptr)->color;
            ++ptr;
        }
    }
    wireframe[0].position = sf::Vector2f(0, 0);
    wireframe[1].position = sf::Vector2f(CHUNK_SIZE * PIXEL_SIZE, 0);
    wireframe[2].position = sf::Vector2f(CHUNK_SIZE * PIXEL_SIZE, CHUNK_SIZE * PIXEL_SIZE);
    wireframe[3].position = sf::Vector2f(0, CHUNK_SIZE * PIXEL_SIZE);

    wireframe[0].color = sf::Color(199, 199, 199);
    wireframe[1].color = sf::Color(199, 199, 199);
    wireframe[2].color = sf::Color(199, 199, 199);
    wireframe[3].color = sf::Color(199, 199, 199);
}


TileResponse Chunk::replaceTile(sf::Vector2<int> tilePos, std::shared_ptr<Pixel> newTile, bool override) {
    int idx = tilePos.y * CHUNK_SIZE + tilePos.x;
    if (idx > CHUNK_SIZE * CHUNK_SIZE || idx < 0)
        return TileResponse::OOB;
    if (pixels[idx]->type == newTile->type)
        return TileResponse::ALREADY_CREATED;
    if (!override && pixels[idx]->type != PixelType::Air)
        return TileResponse::NOT_EMPTY;
    pixels[idx] = newTile;
    // get a pointer to the current tile's sf::quad
    sf::Vertex* quad = &vertices[idx * 4];
    // We don't change the previous position, only the color, to match the one of the new tile
    quad[0].color = newTile->color;
    quad[1].color = newTile->color;
    quad[2].color = newTile->color;
    quad[3].color = newTile->color;

    return TileResponse::CREATED;
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


void Chunk::update(std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks) {
    auto &scrn = Core::get().screen;
    /** update bottom row */
    auto ptr = pixels.data() + CHUNK_SIZE * CHUNK_SIZE - 1;
    for (int y = CHUNK_SIZE - 1; y >= 0; --y) {
        for (int x = CHUNK_SIZE - 1; x >= 0; --x) {
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
                        surround.l = left + CHUNK_SIZE - 1;
                        surround.dl = left + 2 * CHUNK_SIZE - 1;
                    } else {
                        surround.l = nullptr;
                        surround.dl = nullptr;
                    }
                    // up Left
                    if (chunks[pos.x - 1][pos.y - 1]) {
                        auto up = chunks[pos.x - 1][pos.y - 1]->pixels.data();
                        surround.ul = up + CHUNK_SIZE* CHUNK_SIZE - 1;
                    } else {
                        surround.ul = nullptr;
                    }
                    // up
                    if (chunks[pos.x][pos.y - 1]) {
                        auto up = chunks[pos.x][pos.y - 1]->pixels.data();
                        surround.u = up + x + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                        surround.ur = up + x + 1 + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                    } else {
                        surround.u = nullptr;
                        surround.ur = nullptr;
                    }
                }
                /** Last column */
                else if (x == CHUNK_SIZE - 1) {
                    // right
                    if (chunks[pos.x + 1][pos.y]) {
                        auto right = chunks[pos.x + 1][pos.y]->pixels.data();
                        surround.r = right;
                        surround.dr = right + CHUNK_SIZE;
                    } else {
                        surround.r = nullptr;
                        surround.dr = nullptr;
                    }
                    // right up
                    if (chunks[pos.x + 1][pos.y - 1]) {
                        auto up = chunks[pos.x + 1][pos.y - 1]->pixels.data();
                        surround.ur = up + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                    } else {
                        surround.ur = nullptr;
                    }
                    // up
                    if (chunks[pos.x][pos.y - 1]) {
                        auto up = chunks[pos.x][pos.y - 1]->pixels.data();
                        surround.ul = up + x - 1 + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                        surround.u = up + x + (CHUNK_SIZE - 1) * CHUNK_SIZE;
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
                        surround.ul = up + x - 1 + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                        surround.u = up + x + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                        surround.ur = up + x + 1 + (CHUNK_SIZE - 1) * CHUNK_SIZE;
                    } else {
                        surround.ul = nullptr;
                        surround.u = nullptr;
                        surround.ur = nullptr;
                    }
                }
            }
            /** Last row */
            else if (y == CHUNK_SIZE - 1) {
                /** Checking X axis */
                /** First column */
                if (x == 0) {
                    // left
                    if (chunks[pos.x - 1][pos.y]) {
                        auto left = chunks[pos.x-1][pos.y]->pixels.data();
                        surround.ul = left + (y - 1) * CHUNK_SIZE - 1;
                        surround.l = left + y * CHUNK_SIZE - 1;
                    } else {
                        surround.ul = nullptr;
                        surround.l = nullptr;
                    }
                    // down Left
                    if (chunks[pos.x - 1][pos.y + 1]) {
                        auto down = chunks[pos.x - 1][pos.y + 1]->pixels.data();
                        surround.dl = down + CHUNK_SIZE - 1;
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
                else if (x == CHUNK_SIZE - 1) {
                    // right
                    if (chunks[pos.x + 1][pos.y]) {
                        auto right = chunks[pos.x + 1][pos.y]->pixels.data();
                        surround.r = right + (CHUNK_SIZE - 1) * CHUNK_SIZE;
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
                        surround.ul = left + (y - 0) * CHUNK_SIZE - 1;
                        surround.l = left + (y+1) * CHUNK_SIZE - 1;
                        surround.dl = left + (y + 2) * CHUNK_SIZE - 1;
                    } else {
                        surround.ul = nullptr;
                        surround.l = nullptr;
                        surround.dl = nullptr;
                    }
                }
                    /** First column */
                else if (x == CHUNK_SIZE - 1) {
                    if (chunks[pos.x + 1][pos.y]) {
                        auto right = chunks[pos.x + 1][pos.y]->pixels.data();
                        surround.ur = right + CHUNK_SIZE * (y - 1);
                        surround.r = right + CHUNK_SIZE * (y);
                        surround.dr = right + CHUNK_SIZE * (y + 1);
                    } else {
                        surround.ur = nullptr;
                        surround.r = nullptr;
                        surround.dr = nullptr;
                    }
                }
                /** Intermediate column handled by the constructor */
            }

            surround.c = ptr;
            if (!(*ptr)->processed) {
                sf::Vector2i pos = sf::Vector2i(x, y);
                // We should not use a std::shared_ptr<Pixel> here,
                // as when we'll implement velocity we wont be able
                // to use the Surrounding class anymore
                // (cuz we'll need more than just the immediate neighbors)
                std::shared_ptr<Pixel> nextPosPixel = (*ptr)->update(
                    surround,
                    sf::Vector2i(x, y),
                    sf::Vector2i(pos.x, pos.y)
                );
                (*ptr)->processed = true;
                // Now swap the colors of the vertices
                // in the vertices VertexArray buffer

                // get a pointer to the current tile's sf::quad
                sf::Vertex* quad = &vertices[(x + y * CHUNK_SIZE) * 4];
                sf::Color tmp = quad[0].color;
                quad[0].color = (*ptr)->color;
                quad[1].color = (*ptr)->color;
                quad[2].color = (*ptr)->color;
                quad[3].color = (*ptr)->color;



            }
            --ptr;
        }
    }
}
