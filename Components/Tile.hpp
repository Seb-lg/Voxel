//
// Created by seub on 04/03/2021.
//
#pragma once
#define uchar unsigned char

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <memory>
#include <random>
#include <iostream>
#include "../conf.hpp"
#include "../Globals.hpp"

struct PixelSwitch;
class Map;

class Tile {
public:
    Tile(
        sf::Vector2i globalIdx, PixelType type=PixelType::Air,
        sf::Color colorTarget=sf::Color(0, 0, 0), int colorVar=1,
        sf::Vector2i densityParams=sf::Vector2i(0, 1), uint life=0
    );

    virtual bool update(Map &map, PixelSwitch &nextPixelData);
    void draw(sf::RenderTexture &rawGameTexture);

public:
    int processed;

    PixelType type;
    sf::Color color;
    uint life;
    int density;
    sf::Vector2f velocity;
};

class Concrete: public Tile {
public:
    Concrete(sf::Vector2i globalIdx);
};


class Water: public Tile {
public:
    Water(sf::Vector2i globalIdx);
    bool update(Map &map, PixelSwitch &nextPixelData) override;
};

class Sand: public Tile {
public:
    Sand(sf::Vector2i globalIdx);
    bool update(Map &map, PixelSwitch &nextPixelData) override;
};

// Utils
struct PixelSwitch {
    uint pixel1Idx;
    std::shared_ptr<Tile> *pixel1 = nullptr;
    // 2 = Wanted pos
    sf::Vector2i chunk2Pos;
    uint pixel2Idx;
    std::shared_ptr<Tile> *pixel2 = nullptr;
    // 1 = actual pos
    sf::Vector2i chunk1Pos;
};

inline const char* pixelTypeToString(PixelType v)
{
    switch (v)
    {
        case PixelType::Air:        return "Air";
        case PixelType::Sand:       return "Sand";
        case PixelType::Concrete:   return "Concrete";
        case PixelType::Water:      return "Water";
        default:                    return "[Unknown PixelType]";
    }
}
