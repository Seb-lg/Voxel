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

std::shared_ptr<Pixel> Water::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) {
    auto density = (*surround.c)->density;
    if (surround.d && density > (*surround.d)->density) {
        return (*surround.d);
        //swapTiles(surround.c, surround.d, pos + sf::Vector2i(0, 1), chunk_pos);
    } else if (surround.dl && density > (*surround.dl)->density) {
        return (*surround.dl);
        //swapTiles(surround.c, surround.dl, pos + sf::Vector2i(-1, 1), chunk_pos);
    } else if (surround.dr && density > (*surround.dr)->density) {
        return (*surround.dr);
        //swapTiles(surround.c, surround.dr, pos + sf::Vector2i(1, 1), chunk_pos);
    } else if (surround.r && density > (*surround.r)->density) {
        return (*surround.r);
        //swapTiles(surround.c, surround.r, pos + sf::Vector2i(1, 0), chunk_pos);
    } else if (surround.l && density > (*surround.l)->density) {
        return (*surround.l);
        //swapTiles(surround.c, surround.l, pos + sf::Vector2i(-1, 0), chunk_pos);
    }
    return (*surround.c);
}

Sand::Sand(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Sand,
        sf::Color(194, 178, 128), 30,
        sf::Vector2i(59, 12)
    )
{};

std::shared_ptr<Pixel> Sand::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) {
    auto density = (*surround.c)->density;
    if (surround.d && density > (*surround.d)->density) {
        return (*surround.d);
        //swapTiles(surround.c, surround.d, pos + sf::Vector2i(0, 1), chunk_pos);
    } else if (surround.dl && density > (*surround.dl)->density) {
        return (*surround.dl);
        //swapTiles(surround.c, surround.dl, pos + sf::Vector2i(-1, 1), chunk_pos);
    } else if (surround.dr && density > (*surround.dr)->density) {
        return (*surround.dr);
        //swapTiles(surround.c, surround.dr, pos + sf::Vector2i(1, 1), chunk_pos);
    }
    return (*surround.c);
}
