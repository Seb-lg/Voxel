//
// Created by seub on 07/03/2021.
//

#include "Pixel.hpp"

Pixel::Pixel(int density, uint life):
    type(PixelType::Air), processed(false),
    sprite(sf::Quads, 4), density(density), life(life)
{};

void Pixel::update(Surrounding surrounding, int x, int y, sf::RenderWindow &window, int cx, int cy) {}

void Pixel::draw(sf::RenderWindow &window) {
    sf::Transform jej;
    jej.translate(drawX * pixel_size, drawY * pixel_size);
    window.draw(sprite, jej);
}

void Pixel::swapTiles(
        std::shared_ptr<Pixel> *fst, std::shared_ptr<Pixel> *snd,
        sf::Vector2<int> pos, sf::Vector2<int> chunk_pos
) {
    auto tmp = (*fst);
    (*fst) = (*snd);
    (*snd) = tmp;
    drawX = chunk_pos.x * chunk_size + pos.x;
    drawY = chunk_pos.y * chunk_size + pos.y + 1;
}


Concrete::Concrete(): Pixel(INT_MAX) {
    type = PixelType::Concrete;
    sprite[0].position = sf::Vector2f(0, 0);
    sprite[1].position = sf::Vector2f(pixel_size, 0);
    sprite[2].position = sf::Vector2f(pixel_size, pixel_size);
    sprite[3].position = sf::Vector2f(0, pixel_size);
    int var = std::rand() % 10 - 5;
    auto pixel_color = sf::Color(100 + var, 100 + var, 100 + var);
    sprite[0].color = pixel_color;
    sprite[1].color = pixel_color;
    sprite[2].color = pixel_color;
    sprite[3].color = pixel_color;
};

void Concrete::update(Surrounding surround, int x, int y, sf::RenderWindow &window, int cx, int cy) {
    // Do nothing, just set the draw at the same position
    drawX = cx * chunk_size + x;
    drawY = cy * chunk_size + y;
}
