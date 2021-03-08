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

struct Surrounding;

class Pixel {
public:
    Pixel(uchar r=0, uchar g=0, uchar b=0, uchar a=255):r(r), g(g), b(b), a(a), type(PixelType::Air), processed(false), sprite(sf::Quads, 4) {
    };

    virtual void
    update(Surrounding surrounding, int x, int y, sf::RenderWindow &window, int cx, int cy);

    void draw(sf::RenderWindow &window, int x, int y) {
        sf::Transform jej;
        jej.translate(x * pixel_size, y * pixel_size);
        window.draw(sprite, jej);
    }

public:
    uchar r;
    uchar g;
    uchar b;
    uchar a;

    PixelType type;
    bool processed;
    sf::VertexArray sprite;
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
//        ul = nullptr;
//        u = nullptr;
//        ur = nullptr;
//        l = nullptr;
//        c = nullptr;
//        r = nullptr;
//        dl = nullptr;
//        d = nullptr;
//        dr = nullptr;
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


class Sand: public Pixel {
public:
    Sand(): Pixel(255, 255, 0) {
        type = PixelType::Sand;
        sprite[0].position = sf::Vector2f(0, 0);
        sprite[1].position = sf::Vector2f(pixel_size, 0);
        sprite[2].position = sf::Vector2f(pixel_size, pixel_size);
        sprite[3].position = sf::Vector2f(0, pixel_size);

//        sprite[0].color = sf::Color(r, g, b);
//        sprite[1].color = sf::Color(r, g, b);
//        sprite[2].color = sf::Color(r, g, b);
//        sprite[3].color = sf::Color(r, g, b);
        sprite[0].color = sf::Color(std::rand()%255, std::rand()%255, std::rand()%255);
        sprite[1].color = sf::Color(std::rand()%255, std::rand()%255, std::rand()%255);
        sprite[2].color = sf::Color(std::rand()%255, std::rand()%255, std::rand()%255);
        sprite[3].color = sf::Color(std::rand()%255, std::rand()%255, std::rand()%255);
    };

    /*void update(std::shared_ptr<Pixel> *map, int x, int y, sf::RenderWindow &window, int cx, int cy) override {
        if (!processed && y < chunk_size - 1) {
            if (map[x+ (chunk_size *(y + 1))]->type == PixelType::Air) {
                auto tmp = map[x+ (chunk_size *y)];
                map[x+ (chunk_size *y)] = map[x+ (chunk_size *(y + 1))];
                map[x+ (chunk_size *(y + 1))] = tmp;
                draw(window, cx * chunk_size + x, cy * chunk_size + y + 1);
            } else if (x>0 &&
                        map[x - 1+ (chunk_size *(y + 1))]->type == PixelType::Air &&
                        map[x - 1+ (chunk_size *y)]->type == PixelType::Air) {
                auto tmp = map[x+ (chunk_size *y)];
                map[x+ (chunk_size *y)] = map[x - 1+ (chunk_size *(y + 1))];
                map[x - 1+ (chunk_size *(y + 1))] = tmp;
                draw(window, cx * chunk_size + x - 1, cy * chunk_size + y + 1);
            } else if (x<chunk_size - 1 &&
                        map[x + 1+ (chunk_size *(y + 1))]->type == PixelType::Air &&
                        map[x + 1+ (chunk_size *y)]->type == PixelType::Air) {
                auto tmp = map[x+ (chunk_size *y)];
                map[x+ (chunk_size *y)] = map[x + 1+ (chunk_size *(y + 1))];
                map[x + 1+ (chunk_size *(y + 1))] = tmp;
                draw(window, cx * chunk_size + x + 1, cy * chunk_size + y + 1);
            }
        }
        draw(window, cx * chunk_size + x, cy * chunk_size + y);
        processed = true;
    }*/
    void update(Surrounding surround, int x, int y, sf::RenderWindow &window, int cx, int cy) override {
        if (!processed) {
            if (surround.d && (*surround.d)->type == PixelType::Air) {
                auto tmp = (*surround.c);
                (*surround.c) = (*surround.d);
                (*surround.d) = tmp;
                draw(window, cx * chunk_size + x, cy * chunk_size + y + 1);
            } else if (surround.l && (*surround.l)->type == PixelType::Air && surround.dl && (*surround.dl)->type == PixelType::Air) {
                auto tmp = (*surround.c);
                (*surround.c) = (*surround.dl);
                (*surround.dl) = tmp;
                draw(window, cx * chunk_size + x - 1, cy * chunk_size + y + 1);
            } else if (surround.r && (*surround.r)->type == PixelType::Air && surround.dr && (*surround.dr)->type == PixelType::Air) {
                auto tmp = (*surround.c);
                (*surround.c) = (*surround.dr);
                (*surround.dr) = tmp;
                draw(window, cx * chunk_size + x - 1, cy * chunk_size + y + 1);
            }
                /*else if (x>0 &&
                        map[x - 1+ (chunk_size *(y + 1))]->type == PixelType::Air &&
                        map[x - 1+ (chunk_size *y)]->type == PixelType::Air) {
                auto tmp = map[x+ (chunk_size *y)];
                map[x+ (chunk_size *y)] = map[x - 1+ (chunk_size *(y + 1))];
                map[x - 1+ (chunk_size *(y + 1))] = tmp;
                draw(window, cx * chunk_size + x - 1, cy * chunk_size + y + 1);
            } else if (x<chunk_size - 1 &&
                        map[x + 1+ (chunk_size *(y + 1))]->type == PixelType::Air &&
                        map[x + 1+ (chunk_size *y)]->type == PixelType::Air) {
                auto tmp = map[x+ (chunk_size *y)];
                map[x+ (chunk_size *y)] = map[x + 1+ (chunk_size *(y + 1))];
                map[x + 1+ (chunk_size *(y + 1))] = tmp;
                draw(window, cx * chunk_size + x + 1, cy * chunk_size + y + 1);
            }*/
            else {
                draw(window, cx * chunk_size + x, cy * chunk_size + y);
            }
            processed = true;
        }
    }

};

