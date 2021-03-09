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

enum class PixelType {
    Air = 0,
    Sand = 1,
    Concrete = 2,
};

enum class TileResponse {
    OOB = -1,
    ALREADY_CREATED = 0,
    CREATED = 1,
    NOT_EMPTY = 2,
};

struct Surrounding;

class Pixel {
public:
    Pixel(uchar r=0, uchar g=0, uchar b=0, uchar a=255);
    virtual void update(Surrounding surrounding, int x, int y, sf::RenderWindow &window, int cx, int cy);
    virtual std::shared_ptr<Pixel> clone() { return std::make_shared<Pixel>(); }
    void draw(sf::RenderWindow &window);

public:
    uchar r;
    uchar g;
    uchar b;
    uchar a;

    PixelType type;
    bool processed;
    sf::VertexArray sprite;
    int drawX;
    int drawY;
};


class Sand: public Pixel {
public:
    Sand();
    std::shared_ptr<Pixel> clone() override { return std::make_shared<Sand>(); }
    void update(
        Surrounding surround,
        int x, int y,
        sf::RenderWindow &window,
        int cx, int cy
    ) override;
};

class Concrete: public Pixel {
public:
    Concrete();
    std::shared_ptr<Pixel> clone() override { return std::make_shared<Concrete>(); }
    void update(
        Surrounding surround,
        int x, int y,
        sf::RenderWindow &window,
        int cx, int cy
    ) override;
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
