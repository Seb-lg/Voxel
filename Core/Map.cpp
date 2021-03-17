//
// Created by seub on 3/17/21.
//

#include "Map.hpp"

Map::Map():perlin(RANDOM_SEED) {

}

void Map::swapPixels(PixelSwitch &nextPixelData) {
    sf::VertexArray chunk1Vertices = chunks[nextPixelData.chunk1Pos.x][nextPixelData.chunk1Pos.y];
    sf::VertexArray chunk2Vertices = chunks[nextPixelData.chunk2Pos.x][nextPixelData.chunk2Pos.y];
    // First swap the colors of the vertices
    // in the vertices VertexArray buffer
    sf::Vertex* quad1 = &vertices[chunk1Vertices * 4];
    sf::Color tmp = quad1[0].color; // All vertices have the same color
    sf::Vertex* quad2 = &vertices[chunk2Vertices * 4];
    quad1[0].color = quad2[0].color;
    quad1[1].color = quad2[1].color;
    quad1[2].color = quad2[2].color;
    quad1[3].color = quad2[3].color;

    quad2[0].color = tmp;
    quad2[1].color = tmp;
    quad2[2].color = tmp;
    quad2[3].color = tmp;
    // Now swap the Pixel themselves

    auto tmp = *pixel1;
    *pixel = *pixel2;
    *pixel2 = tmp;
}
