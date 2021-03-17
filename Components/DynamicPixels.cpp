//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Pixel.hpp"
#include "../Core/Map.hpp"


Water::Water(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Water,
        sf::Color(108, 177, 202), 5,
        sf::Vector2i(50, 10)
    )
{};

bool Water::update(Map map, PixelSwitch &nextPixelData) {
    // DO a:
    // for delta in (list of delta positions):
    // if ...

    // D
    map.lookup(sf::Vector2i(0, 1), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    // DL
    map.lookup(sf::Vector2i(-1, 1), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    // DR
    map.lookup(sf::Vector2i(1, 1), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    // R
    map.lookup(sf::Vector2i(1, 0), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    // L
    map.lookup(sf::Vector2i(-1, 0), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    return false;
}

Sand::Sand(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Sand,
        sf::Color(194, 178, 128), 30,
        sf::Vector2i(59, 12)
    )
{};

bool Sand::update(Map map, PixelSwitch &nextPixelData) {
    // D
    map.lookup(sf::Vector2i(0, 1), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    // DL
    map.lookup(sf::Vector2i(-1, 1), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    // DR
    map.lookup(sf::Vector2i(1, 1), nextPixelData);
    if (nextPixelData.pixel2 && density > (*nextPixelData.pixel2)->density)
        return true;
    return false;
}
