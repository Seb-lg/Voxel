//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Pixel.hpp"


Water::Water(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Water,
        sf::Color(108, 177, 202), 5,
        sf::Vector2i(50, 10)
    )
{};

std::shared_ptr<PixelSwitch> Water::update(Map map, PixelSwitch &nextPixelData) {
    // DO a:
    // for delta in (list of delta positions):
    // if ...


    (*ptr)->processed = true;
    // D
    std::shared_ptr<PixelSwitch> nextPixelData;
    nextPixelData = map.lookup(sf::Vector2i(0, 1), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    // DL
    nextPixelData = map.lookup(sf::Vector2i(-1, 1), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    // DR
    nextPixelData = map.lookup(sf::Vector2i(1, 1), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    // R
    nextPixelData = map.lookup(sf::Vector2i(1, 0), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    // L
    nextPixelData = map.lookup(sf::Vector2i(-1, 0), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    (*ptr)->processed = false;
    return std::make_shared<PixelSwitch>(nullptr);
}

Sand::Sand(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Sand,
        sf::Color(194, 178, 128), 30,
        sf::Vector2i(59, 12)
    )
{};

std::shared_ptr<PixelSwitch> Sand::update(Map map, PixelSwitch &nextPixelData) {
    (*ptr)->processed = true;
    // D
    std::shared_ptr<PixelSwitch> nextPixelData;
    nextPixelData = map.lookup(sf::Vector2i(0, 1), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    // DL
    nextPixelData = map.lookup(sf::Vector2i(-1, 1), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    // DR
    nextPixelData = map.lookup(sf::Vector2i(1, 1), nextPixelData);
    if (nextPixelData && density > nextPixelData->nextPixel->density)
        return nextPixelData;
    (*ptr)->processed = false;
    return std::make_shared<PixelSwitch>(nullptr);
}
