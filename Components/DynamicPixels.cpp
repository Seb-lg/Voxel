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

void Water::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) {
    bool forceUpdate = std::rand()%2;
    if (processed && forceUpdate)
        return;
    auto density = (*surround.c)->density;
    if (surround.d && (!(*surround.d)->processed || forceUpdate) && (*surround.d)->type != type && density > (*surround.d)->density) {
        (*surround.d)->processed = true;
        processed = true;
        swapTiles(surround.c, surround.d, pos + sf::Vector2i(0, 1), chunk_pos);
    } else if (surround.dl && (!(*surround.dl)->processed || forceUpdate) && (*surround.dl)->type != type && density > (*surround.dl)->density) {
        (*surround.dl)->processed = true;
        processed = true;
        swapTiles(surround.c, surround.dl, pos + sf::Vector2i(-1, 1), chunk_pos);
    } else if (surround.dr && (!(*surround.dr)->processed || forceUpdate) && (*surround.dr)->type != type && density > (*surround.dr)->density) {
        (*surround.dr)->processed = true;
        processed = true;
        swapTiles(surround.c, surround.dr, pos + sf::Vector2i(1, 1), chunk_pos);
    } else if (surround.r && (!(*surround.r)->processed || forceUpdate) && (*surround.r)->type != type && density > (*surround.r)->density) {
        (*surround.r)->processed = true;
        processed = true;
        swapTiles(surround.c, surround.r, pos + sf::Vector2i(1, 0), chunk_pos);
    } else if (surround.l && (!(*surround.l)->processed || forceUpdate) && (*surround.l)->type != type && density > (*surround.l)->density) {
        (*surround.l)->processed = true;
        processed = true;
        swapTiles(surround.c, surround.l, pos + sf::Vector2i(-1, 0), chunk_pos);
    }
//    processed = true;
}

Sand::Sand(sf::Vector2i globalIdx):
    Pixel(
        globalIdx, PixelType::Sand,
        sf::Color(194, 178, 128), 30,
        sf::Vector2i(59, 12)
    )
{};

void Sand::update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) {
    if (processed)
        return;
    auto density = (*surround.c)->density;
    if (surround.d && density > (*surround.d)->density) {
        swapTiles(surround.c, surround.d, pos + sf::Vector2i(0, 1), chunk_pos);
    } else if (surround.dl && density > (*surround.dl)->density) {
        swapTiles(surround.c, surround.dl, pos + sf::Vector2i(-1, 1), chunk_pos);
    } else if (surround.dr && density > (*surround.dr)->density) {
        swapTiles(surround.c, surround.dr, pos + sf::Vector2i(1, 1), chunk_pos);
    }
    processed = true;
}
