//
// Created by seub on 07/03/2021.
//

#include "Pixel.hpp"

Pixel::Pixel(
    sf::Vector2i globalIdx, PixelType type,
    sf::Color color, int var, int density, uint life
):
    type(type), processed(false),
    sprite(sf::Quads, 4), density(density), life(life)
{
    // Set vertices color
    var = std::rand() % var - int(var / 2);
    auto pixel_color = sf::Color(color.r + var, color.g + var, color.b + var);
    sprite[0].color = pixel_color;
    sprite[1].color = pixel_color;
    sprite[2].color = pixel_color;
    sprite[3].color = pixel_color;
    // Apply position in VertexArray
    auto drawIdx = globalIdx * pixel_size;
    sprite[0].position = sf::Vector2f(drawIdx.x, drawIdx.y);
    sprite[1].position = sf::Vector2f(drawIdx.x + pixel_size, drawIdx.y);
    sprite[2].position = sf::Vector2f(drawIdx.x + pixel_size, drawIdx.y + pixel_size);
    sprite[3].position = sf::Vector2f(drawIdx.x, drawIdx.y + pixel_size);
};

void Pixel::update(Surrounding surrounding, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos)
{
    auto tmpX = (chunk_pos.x * chunk_size + pos.x) * pixel_size;
    auto tmpY = (chunk_pos.y * chunk_size + pos.y) * pixel_size;
    // Apply position in VertexArray
    sprite[0].position = sf::Vector2f(tmpX, tmpY);
    sprite[1].position = sf::Vector2f(tmpX + pixel_size, tmpY);
    sprite[2].position = sf::Vector2f(tmpX + pixel_size, tmpY + pixel_size);
    sprite[3].position = sf::Vector2f(tmpX, tmpY + pixel_size);
}

void Pixel::draw(sf::RenderTexture &rawGameTexture) {
    if (type != PixelType::Air)
        rawGameTexture.draw(sprite);
}

void Pixel::swapTiles(
        std::shared_ptr<Pixel> *fst, std::shared_ptr<Pixel> *snd,
        sf::Vector2<int> pos, sf::Vector2<int> chunk_pos
) {
    auto tmp = (*fst);
    (*fst) = (*snd);
    (*snd) = tmp;
    auto tmpX = (chunk_pos.x * chunk_size + pos.x) * pixel_size;
    auto tmpY = (chunk_pos.y * chunk_size + pos.y) * pixel_size;
    // Apply position in VertexArray
    sprite[0].position = sf::Vector2f(tmpX, tmpY);
    sprite[1].position = sf::Vector2f(tmpX + pixel_size, tmpY);
    sprite[2].position = sf::Vector2f(tmpX + pixel_size, tmpY + pixel_size);
    sprite[3].position = sf::Vector2f(tmpX, tmpY + pixel_size);
}

Concrete::Concrete(sf::Vector2i globalIdx):
    Pixel(globalIdx, PixelType::Concrete, sf::Color(100, 100, 100), 10, INT_MAX)
{};
