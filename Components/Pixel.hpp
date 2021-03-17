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

enum class TileResponse {
    OOB = -1,
    ALREADY_CREATED = 0,
    CREATED = 1,
    NOT_EMPTY = 2,
};

enum class NeighborsIdxDeltas {
    ul = -CHUNK_SIZE - 1,
    u = -CHUNK_SIZE,
    ur = -CHUNK_SIZE + 1,
    l = -1,
    c = 0,
    r = +1,
    dl = CHUNK_SIZE - 1,
    d = CHUNK_SIZE,
    dr = CHUNK_SIZE + 1,
};

struct Surrounding;

class Pixel {
public:
    Pixel(
        sf::Vector2i globalIdx, PixelType type=PixelType::Air,
        sf::Color colorTarget=sf::Color(0, 0, 0), int colorVar=1,
        sf::Vector2i densityParams=sf::Vector2i(0, 1), uint life=0
    );

    virtual std::shared_ptr<Pixel> update(
        Surrounding surround,
        sf::Vector2<int> pos,
        sf::Vector2<int> chunk_pos
    );
    // virtual sf::Vector2i getNextPos(Surrounding surrounding, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos);
    void draw(sf::RenderTexture &rawGameTexture);
    // void swapTiles(
    //     std::shared_ptr<Pixel> *fst, std::shared_ptr<Pixel> *snd,
    //     sf::Vector2<int> pos, sf::Vector2<int> chunk_pos
    // );

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
    std::shared_ptr<Pixel> update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) override;
};

class Sand: public Pixel {
public:
    Sand(sf::Vector2i globalIdx);
    std::shared_ptr<Pixel> update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) override;
};


struct Surrounding{
    Surrounding(std::shared_ptr<Pixel>* ptr) {
        ul = ptr - 1 - CHUNK_SIZE;
        u = ptr - CHUNK_SIZE;
        ur = ptr + 1 - CHUNK_SIZE;
        l = ptr - 1;
        c = ptr;
        r = ptr + 1;
        dl = ptr - 1 + CHUNK_SIZE;
        d = ptr + CHUNK_SIZE;
        dr = ptr + 1 + CHUNK_SIZE;
    }
    std::shared_ptr<Pixel> *ul;
    std::shared_ptr<Pixel> *u;
    std::shared_ptr<Pixel> *ur;
    std::shared_ptr<Pixel> *l;
    std::shared_ptr<Pixel> *c;
    std::shared_ptr<Pixel> *r;
    std::shared_ptr<Pixel> *dl;
    std::shared_ptr<Pixel> *d;
    std::shared_ptr<Pixel> *dr;
};
