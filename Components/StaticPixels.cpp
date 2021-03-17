//
// Created by seub on 07/03/2021.
//

#include "Tile.hpp"
#include "../Core/Map.hpp"
#include "Chunk.hpp"

Tile::Tile(
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

bool Tile::update(Map &map, PixelSwitch &nextPixelData)
{
    return false;
}

Concrete::Concrete(sf::Vector2i globalIdx):
        Tile(
        globalIdx, PixelType::Concrete,
        sf::Color(100, 100, 100), 10,
        sf::Vector2i(5000, 20)
    )
{};
