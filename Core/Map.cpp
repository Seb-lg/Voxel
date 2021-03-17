//
// Created by seub on 3/17/21.
//

#include "Map.hpp"

Map::Map():perlin(RANDOM_SEED) {

}

void Map::swapPixels(PixelSwitch &nextPixelData) {
    sf::VertexArray chunk1Vertices = chunks[nextPixelData.chunk1Pos.x][nextPixelData.chunk1Pos.y]->vertices;
    sf::VertexArray chunk2Vertices = chunks[nextPixelData.chunk2Pos.x][nextPixelData.chunk2Pos.y]->vertices;
    // First swap the colors of the vertices
    // in the vertices VertexArray buffer
    sf::Vertex *quad1 = &chunk1Vertices[nextPixelData.pixel1Idx * 4];
    sf::Color tmp = quad1[0].color; // All vertices have the same color
    sf::Vertex *quad2 = &chunk2Vertices[nextPixelData.pixel2Idx * 4];
    quad1[0].color = quad2[0].color;
    quad1[1].color = quad2[1].color;
    quad1[2].color = quad2[2].color;
    quad1[3].color = quad2[3].color;

    quad2[0].color = tmp;
    quad2[1].color = tmp;
    quad2[2].color = tmp;
    quad2[3].color = tmp;
    // Now swap the Pixel themselves

    auto jej = *nextPixelData.pixel1;
    *nextPixelData.pixel1 = *nextPixelData.pixel2;
    *nextPixelData.pixel2 = jej;
}

bool Map::lookup(sf::Vector2i relativeTestPos, PixelSwitch &data) {
    auto x = data.pixel1Idx % CHUNK_SIZE + relativeTestPos.x;
    auto y = data.pixel1Idx / CHUNK_SIZE + relativeTestPos.y;

    data.chunk2Pos = data.chunk1Pos;

    /** Check if the position is out of chunk bound */
    if (x < 0) {
        data.chunk2Pos.x--;
        x = CHUNK_SIZE - 1;
    } else if (x >= CHUNK_SIZE) {
        data.chunk2Pos.x++;
        x = 0;
    }

    if (y < 0) {
        data.chunk2Pos.y--;
        y = CHUNK_SIZE - 1;
    } else if (y >= CHUNK_SIZE) {
        data.chunk2Pos.y++;
        y = 0;
    }

    data.pixel2Idx = y * CHUNK_SIZE + x;

    if (data.chunk2Pos == data.chunk1Pos) {
        data.pixel2 = data.pixel1 + relativeTestPos.x + relativeTestPos.y * CHUNK_SIZE;
        return true;
    } else if (chunks[data.chunk2Pos.x][data.chunk2Pos.y]) {
        data.pixel2 = chunks[data.chunk2Pos.x][data.chunk2Pos.y]->pixels.data() + data.pixel2Idx;
        return true;
    }
    return false;
}
