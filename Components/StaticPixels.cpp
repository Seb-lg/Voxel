//
// Created by seub on 07/03/2021.
//

#include "Pixel.hpp"

Pixel::Pixel(
    sf::Vector2i globalIdx, PixelType type,
    sf::Color colorTarget, int colorVar, sf::Vector2i densityParams, uint life
):
    processed(false), type(type), life(life)
{
    // Set color
    colorVar = std::rand() % colorVar - int(colorVar / 2);
    color = sf::Color(colorTarget.r + colorVar, colorTarget.g + colorVar, colorTarget.b + colorVar);
    // Apply some randomness to the density as well
    density = densityParams.x + (std::rand() % densityParams.y - int(densityParams.y / 2));
};

std::shared_ptr<Pixel> Pixel::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos)
{
    return (*surround.c);
}

Concrete::Concrete(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Concrete,
        sf::Color(100, 100, 100), 10,
        sf::Vector2i(5000, 20)
    )
{};
