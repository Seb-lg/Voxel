//
// Created by seub on 05/03/2021.
//
#include <iostream>
#include "Chunk.hpp"
#include "Pixel.hpp"
#include "../Core/Core.hpp"
#include "../Core/Map.hpp"


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
        return (TileResponse)TileResponse::OOB;
    if (pixels[idx]->type == newTile->type)
        return (TileResponse)TileResponse::ALREADY_CREATED;
    if (!override && pixels[idx]->type != PixelType::Air)
        return (TileResponse)TileResponse::NOT_EMPTY;
    pixels[idx] = newTile;
    // get a pointer to the current tile's sf::quad
    sf::Vertex* quad = &vertices[idx * 4];
    // We don't change the previous position, only the color, to match the one of the new tile
    quad[0].color = newTile->color;
    quad[1].color = newTile->color;
    quad[2].color = newTile->color;
    quad[3].color = newTile->color;

    return (TileResponse)TileResponse::CREATED;
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


void Chunk::update(Map &map) {
    auto &scrn = Core::get().screen;
    /** update bottom row */
    auto ptr = pixels.data() + CHUNK_SIZE * CHUNK_SIZE - 1;
    for (int y = CHUNK_SIZE - 1; y >= 0; --y) {
        for (int x = CHUNK_SIZE - 1; x >= 0; --x) {
            if (ptr->get()->type == PixelType::Air) {
                --ptr;
                continue;
            }
            if (!(*ptr)->processed) {
                PixelSwitch nextPixelData;
                nextPixelData.chunk1Pos = sf::Vector2i(x, y);
                nextPixelData.pixel1Idx = x + y * CHUNK_SIZE;
                nextPixelData.pixel1 = ptr;

                if ((*ptr)->update(map, nextPixelData)) {
                    (*ptr)->processed = true;
                    map.swapPixels(&nextPixelData);
                }
            }
            --ptr;
        }
    }
}
