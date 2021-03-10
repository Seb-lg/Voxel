//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Pixel.hpp"


Water::Water(): Pixel(255, 255, 0) {
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

void Water::update(Surrounding surround, int x, int y, sf::RenderWindow &window, int cx, int cy) {
    if (processed)
        return;
    if (surround.d && (*surround.d)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.d);
        (*surround.d) = tmp;
        drawX = cx * chunk_size + x;
        drawY = cy * chunk_size + y + 1;
    } else if (surround.dl && (*surround.dl)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.dl);
        (*surround.dl) = tmp;
        drawX = cx * chunk_size + x - 1;
        drawY = cy * chunk_size + y + 1;
    } else if (surround.dr && (*surround.dr)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.dr);
        (*surround.dr) = tmp;
        drawX = cx * chunk_size + x + 1;
        drawY = cy * chunk_size + y + 1;
    // Check left and right now, as it's water
    } else if (surround.r && (*surround.r)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.r);
        (*surround.r) = tmp;
        drawX = cx * chunk_size + x + 1;
        drawY = cy * chunk_size + y + 1;
    } else if (surround.l && (*surround.l)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.l);
        (*surround.l) = tmp;
        drawX = cx * chunk_size + x + 1;
        drawY = cy * chunk_size + y + 1;
    } else {
        drawX = cx * chunk_size + x;
        drawY = cy * chunk_size + y;
    }
    processed = true;
}

Sand::Sand(): Pixel(255, 255, 0) {
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

void Sand::update(Surrounding surround, int x, int y, sf::RenderWindow &window, int cx, int cy) {
    if (processed)
        return;
    if (surround.d && (*surround.d)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.d);
        (*surround.d) = tmp;
        drawX = cx * chunk_size + x;
        drawY = cy * chunk_size + y + 1;
    } else if (surround.dl && (*surround.dl)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.dl);
        (*surround.dl) = tmp;
        drawX = cx * chunk_size + x - 1;
        drawY = cy * chunk_size + y + 1;
    } else if (surround.dr && (*surround.dr)->type == PixelType::Air) {
        auto tmp = (*surround.c);
        (*surround.c) = (*surround.dr);
        (*surround.dr) = tmp;
        drawX = cx * chunk_size + x + 1;
        drawY = cy * chunk_size + y + 1;
    } else {
        drawX = cx * chunk_size + x;
        drawY = cy * chunk_size + y;
    }
    processed = true;
}
