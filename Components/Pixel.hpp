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
        case PixelType::Air:   return "Air";
        case PixelType::Sand:   return "Sand";
        case PixelType::Concrete: return "Concrete";
        case PixelType::Water: return "Water";
        default:      return "[Unknown PixelType]";
    }
}

enum class TileResponse {
    OOB = -1,
    ALREADY_CREATED = 0,
    CREATED = 1,
    NOT_EMPTY = 2,
};

struct Surrounding;

class Pixel {
public:
    Pixel(
        sf::Vector2i globalIdx, PixelType type=PixelType::Air,
        sf::Color color=sf::Color(255, 105, 180), int var=1,
        int density=0, uint life=0
    );

    virtual void update(Surrounding surrounding, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos);
    void draw(sf::RenderTexture &rawGameTexture);
    void swapTiles(
        std::shared_ptr<Pixel> *fst, std::shared_ptr<Pixel> *snd,
        sf::Vector2<int> pos, sf::Vector2<int> chunk_pos
    );

public:
    uchar r;
    uchar g;
    uchar b;
    uchar a;

    PixelType type;
    int processed;
    sf::VertexArray sprite;
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
    void update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) override;
};

class Sand: public Pixel {
public:
    Sand(sf::Vector2i globalIdx);
    void update(Surrounding surround, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos) override;
};


struct Surrounding{
    Surrounding(std::shared_ptr<Pixel>* ptr) {
        ul = ptr - 1 - chunk_size;
        u = ptr - chunk_size;
        ur = ptr + 1 - chunk_size;
        l = ptr - 1;
        c = ptr;
        r = ptr + 1;
        dl = ptr - 1 + chunk_size;
        d = ptr + chunk_size;
        dr = ptr + 1 + chunk_size;
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
