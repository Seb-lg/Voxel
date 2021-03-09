//
// Created by seub on 04/03/2021.
//

#pragma once
#define uchar unsigned char

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <memory>
#include "../conf.hpp"

enum class PixelType {
    Air = 0,
    Sand = 1,
};

enum class TileResponse {
    OOB = -1,
    ALREADY_CREATED = 0,
    CREATED = 1,
};

struct Surrounding;

class Pixel {
public:
    Pixel(uchar r=0, uchar g=0, uchar b=0, uchar a=255);
    virtual void update(Surrounding surrounding, int x, int y, sf::RenderWindow &window, int cx, int cy);
    virtual std::shared_ptr<Pixel> clone() { return std::make_shared<Pixel>(); }
    void draw(sf::RenderWindow &window, int x, int y);

public:
    uchar r;
    uchar g;
    uchar b;
    uchar a;

    PixelType type;
    bool processed;
    sf::VertexArray sprite;
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
    std::shared_ptr<Pixel> *ul = nullptr;
    std::shared_ptr<Pixel> *u = nullptr;
    std::shared_ptr<Pixel> *ur = nullptr;
    std::shared_ptr<Pixel> *l = nullptr;
    std::shared_ptr<Pixel> *c = nullptr;
    std::shared_ptr<Pixel> *r = nullptr;
    std::shared_ptr<Pixel> *dl = nullptr;
    std::shared_ptr<Pixel> *d = nullptr;
    std::shared_ptr<Pixel> *dr = nullptr;
};
