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
#include <bits/stdc++.h>
#include "../conf.hpp"

class PixelSwitch;
enum class TileResponse;
enum class PixelType;

class Pixel {
public:
    Pixel(
        sf::Vector2i globalIdx, PixelType type=PixelType::Air,
        sf::Color colorTarget=sf::Color(0, 0, 0), int colorVar=1,
        sf::Vector2i densityParams=sf::Vector2i(0, 1), uint life=0
    );

    virtual std::shared_ptr<PixelSwitch> update(Map map, PixelSwitch &nextPixelData);
    void draw(sf::RenderTexture &rawGameTexture);

public:
    int processed;

    PixelType type;
    sf::Color color;
    uint life;
    int density;
};

class Concrete: public Pixel {
public:
    Concrete(sf::Vector2i globalIdx);
};


class Water: public Pixel {
public:
    Water(sf::Vector2i globalIdx);
    std::shared_ptr<PixelSwitch> update(Map map, PixelSwitch &nextPixelData) override;
};

class Sand: public Pixel {
public:
    Sand(sf::Vector2i globalIdx);
    std::shared_ptr<PixelSwitch> update(Map map, PixelSwitch &nextPixelData) override;
};

// Utils
class PixelSwitch {
    // 1 = actual pos
    sf::Vector2i chunk1Pos;
    uint pixel1Idx;
    std::shared_ptr<Pixel> *pixel1;
    // 2 = Wanted pos
    sf::Vector2i chunk2Pos;
    uint pixel2Idx;
    std::shared_ptr<Pixel> *pixel2;
};

enum class TileResponse {
    OOB = -1,
    ALREADY_CREATED = 0,
    CREATED = 1,
    NOT_EMPTY = 2,
};

enum class PixelType {
    Air = 0,
    Sand = 1,
    Concrete = 2,
    Water = 3,
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
