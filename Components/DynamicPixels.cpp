//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Tile.hpp"
#include "../Core/Map.hpp"


Water::Water(sf::Vector2i globalIdx):
        Tile(
        globalIdx, PixelType::Water,
        sf::Color(108, 177, 202), 5,
        sf::Vector2i(50, 10)
    )
{};

bool Water::update(Map &map, PixelSwitch &nextPixelData) {
    bool forceUpdate = std::rand()%2;
    // if (processed && forceUpdate)
    if (processed)
        return false;
    // D
    if (map.lookup(sf::Vector2i(0, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    // DL
    if (map.lookup(sf::Vector2i(-1, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    // DR
    if (map.lookup(sf::Vector2i(1, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    for (int i = 4 ; i > 0 ; i--) {  // R
        if (map.lookup(sf::Vector2i(i, 0), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
            return true;
    }
    for (int i = 4 ; i > 0 ; i--) {  // R
        if (map.lookup(sf::Vector2i(-i, 0), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
            return true;
    }
    return false;
}

Sand::Sand(sf::Vector2i globalIdx):
        Tile(
        globalIdx, PixelType::Sand,
        sf::Color(194, 178, 128), 30,
        sf::Vector2i(59, 12)
    )
{};

bool Sand::update(Map &map, PixelSwitch &nextPixelData) {
    bool forceUpdate = std::rand()%2;
    if (processed && forceUpdate)
        return false;
    // D
    if (map.lookup(sf::Vector2i(0, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    // DL
    if (map.lookup(sf::Vector2i(-1, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    // DR
    if (map.lookup(sf::Vector2i(1, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    return false;
}
