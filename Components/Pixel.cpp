//
// Created by seub on 07/03/2021.
//

#include "Pixel.hpp"

Pixel::Pixel(uchar r, uchar g, uchar b, uchar a):
    r(r), g(g), b(b), a(a), type(PixelType::Air), processed(false), sprite(sf::Quads, 4)
{};

void Pixel::update(Surrounding surrounding, int x, int y, sf::RenderWindow &window, int cx, int cy) {}

void Pixel::draw(sf::RenderWindow &window) {
    sf::Transform jej;
    jej.translate(drawX * pixel_size, drawY * pixel_size);
    window.draw(sprite, jej);
}


Sand::Sand(): Pixel(255, 255, 0) {
    type = PixelType::Sand;
    sprite[0].position = sf::Vector2f(0, 0);
    sprite[1].position = sf::Vector2f(pixel_size, 0);
    sprite[2].position = sf::Vector2f(pixel_size, pixel_size);
    sprite[3].position = sf::Vector2f(0, pixel_size);
    int max = 10;
    int min = 10;
    int var = std::rand() % (max + min);
    auto rd_color1 = 194 + var;
    auto rd_color2 = 178 + var;
    auto rd_color3 = 128 + var;
    sprite[0].color = sf::Color(rd_color1, rd_color2, rd_color3);
    sprite[1].color = sf::Color(rd_color1, rd_color2, rd_color3);
    sprite[2].color = sf::Color(rd_color1, rd_color2, rd_color3);
    sprite[3].color = sf::Color(rd_color1, rd_color2, rd_color3);
};

void Sand::update(Surrounding surround, int x, int y, sf::RenderWindow &window, int cx, int cy) {
    if (!processed) {
        if (surround.d && (*surround.d)->type == PixelType::Air) {
            auto tmp = (*surround.c);
            (*surround.c) = (*surround.d);
            (*surround.d) = tmp;
            drawX = cx * chunk_size + x;
            drawY = cy * chunk_size + y + 1;
        } else if (surround.l && (*surround.l)->type == PixelType::Air && surround.dl && (*surround.dl)->type == PixelType::Air) {
            auto tmp = (*surround.c);
            (*surround.c) = (*surround.dl);
            (*surround.dl) = tmp;
            drawX = cx * chunk_size + x - 1;
            drawY = cy * chunk_size + y + 1;
        } else if (surround.r && (*surround.r)->type == PixelType::Air && surround.dr && (*surround.dr)->type == PixelType::Air) {
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
}
