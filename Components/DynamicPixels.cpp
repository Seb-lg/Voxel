//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Pixel.hpp"


Water::Water(): Pixel(10) {
    type = PixelType::Water;
    sprite[0].position = sf::Vector2f(0, 0);
    sprite[1].position = sf::Vector2f(pixel_size, 0);
    sprite[2].position = sf::Vector2f(pixel_size, pixel_size);
    sprite[3].position = sf::Vector2f(0, pixel_size);
    int var = std::rand() % 2 - 1;
    auto pixel_color = sf::Color(128 + var, 197 + var, 222 + var);
    sprite[0].color = pixel_color;
    sprite[1].color = pixel_color;
    sprite[2].color = pixel_color;
    sprite[3].color = pixel_color;
};

void Water::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) {
    if (processed)
        return;
    auto density = (*surround.c)->density;
    if (surround.d && density > (*surround.d)->density) {
        swapTiles(surround.c, surround.d, pos + sf::Vector2i(0, 1), chunk_pos);
    } else if (surround.dl && density > (*surround.dl)->density) {
        swapTiles(surround.c, surround.dl, pos + sf::Vector2i(-1, 1), chunk_pos);
    } else if (surround.dr && density > (*surround.dr)->density) {
        swapTiles(surround.c, surround.dr, pos + sf::Vector2i(1, 1), chunk_pos);
    } else if (surround.r && density > (*surround.r)->density) {
        swapTiles(surround.c, surround.r, pos + sf::Vector2i(1, 0), chunk_pos);
    } else if (surround.l && density > (*surround.l)->density) {
        swapTiles(surround.c, surround.l, pos + sf::Vector2i(-1, 0), chunk_pos);
    } else {  // Do nothing, just set the draw at the same position
        drawX = chunk_pos.x * chunk_size + pos.x;
        drawY = chunk_pos.y * chunk_size + pos.y;
    }
    processed = true;
}

Sand::Sand(): Pixel(20) {
    type = PixelType::Sand;
    sprite[0].position = sf::Vector2f(0, 0);
    sprite[1].position = sf::Vector2f(pixel_size, 0);
    sprite[2].position = sf::Vector2f(pixel_size, pixel_size);
    sprite[3].position = sf::Vector2f(0, pixel_size);
    int var = std::rand() % 30 - 15;
    auto pixel_color = sf::Color(194 + var, 178 + var, 128 + var);
    sprite[0].color = pixel_color;
    sprite[1].color = pixel_color;
    sprite[2].color = pixel_color;
    sprite[3].color = pixel_color;
};

void Sand::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) {
    if (processed)
        return;
    auto density = (*surround.c)->density;
    if (surround.d && density > (*surround.d)->density) {
        swapTiles(surround.c, surround.d, pos + sf::Vector2i(0, 1), chunk_pos);
    } else if (surround.dl && density > (*surround.dl)->density) {
        swapTiles(surround.c, surround.dl, pos + sf::Vector2i(-1, 1), chunk_pos);
    } else if (surround.dr && density > (*surround.dr)->density) {
        swapTiles(surround.c, surround.dr, pos + sf::Vector2i(1, 1), chunk_pos);
    } else {  // Do nothing, just set the draw at the same position
        drawX = chunk_pos.x * chunk_size + pos.x;
        drawY = chunk_pos.y * chunk_size + pos.y;
    }
    processed = true;
}
