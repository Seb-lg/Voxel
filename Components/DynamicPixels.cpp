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
{
    velocity.x = std::rand() % 2 ? -1 : 1;
    velocity.y = 1;
}

bool Water::update(Map &map, PixelSwitch &nextPixelData) {
    bool forceUpdate = std::rand()%2;
    // if (processed && forceUpdate)
    if (processed)
        return false;
    for (int i = 3 ; i > 0 ; i--) {  // D
        if (map.lookup(sf::Vector2i(0, i), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
            return true;
    }
    // DL
    if (map.lookup(sf::Vector2i(-1, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    // DR
    if (map.lookup(sf::Vector2i(1, 1), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
        return true;
    if (velocity.x > 0){
        for (int i = 4 ; i > 0 ; i--) {  // R
            if (map.lookup(sf::Vector2i(i, 0), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) &&
                density > (*nextPixelData.pixel2)->density)
                return true;
        }
        nextPixelData.pixel1->get()->velocity.x = -nextPixelData.pixel1->get()->velocity.x;// - BOUNCE_COST *  relativeTestPos.x);
        nextPixelData.pixel1->get()->velocity.y = -nextPixelData.pixel1->get()->velocity.y;// - BOUNCE_COST *  relativeTestPos.y);
    }
    else if (velocity.x < 0) {
        for (int i = 4 ; i > 0 ; i--) {  // L
            if (map.lookup(sf::Vector2i(-i, 0), nextPixelData) && ((*nextPixelData.pixel2)->processed || forceUpdate) && density > (*nextPixelData.pixel2)->density)
                return true;
        }
        nextPixelData.pixel1->get()->velocity.x = -nextPixelData.pixel1->get()->velocity.x;// - BOUNCE_COST *  relativeTestPos.x);
        nextPixelData.pixel1->get()->velocity.y = -nextPixelData.pixel1->get()->velocity.y;// - BOUNCE_COST *  relativeTestPos.y);
    }
    else
        std::cout << "jej" << std::endl;
    return false;
}

Sand::Sand(sf::Vector2i globalIdx):
        Tile(
        globalIdx, PixelType::Sand,
        sf::Color(194, 178, 128), 30,
        sf::Vector2i(59, 12)
    )
{
}

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
