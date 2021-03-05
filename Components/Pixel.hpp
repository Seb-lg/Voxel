//
// Created by seub on 04/03/2021.
//

#pragma once
#define uchar unsigned char

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include "../conf.hpp"

enum class PixelType {
    Air = 0,
    Sand = 1,
};

class Pixel {
public:
    Pixel(uchar r=0, uchar g=0, uchar b=0, uchar a=255):r(r), g(g), b(b), a(a), type(PixelType::Air), processed(false), sprite(sf::Quads, 4) {
    };

    virtual void
    update(std::map<int, std::map<int, std::shared_ptr<Pixel>>> &map, int x, int y, sf::RenderWindow &window) {
    };

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

    void update(std::map<int, std::map<int, std::shared_ptr<Pixel>>> &map, int x, int y, sf::RenderWindow &window) override {
        if (!processed && y != height -1) {
            if (map[x][y + 1]->type == PixelType::Air) {
                auto tmp = map[x][y];
                map[x][y] = map[x][y + 1];
                map[x][y + 1] = tmp;
                draw(window, x, y + 1);
            } else if (x>0&&map[x - 1][y + 1]->type == PixelType::Air&&map[x - 1][y]->type == PixelType::Air) {
                auto tmp = map[x][y];
                map[x][y] = map[x - 1][y + 1];
                map[x - 1][y + 1] = tmp;
                draw(window, x - 1, y + 1);
            } else if (x<width - 1&&map[x + 1][y + 1]->type == PixelType::Air&&map[x + 1][y]->type == PixelType::Air) {
                auto tmp = map[x][y];
                map[x][y] = map[x + 1][y + 1];
                map[x + 1][y + 1] = tmp;
                draw(window, x + 1, y + 1);
            }
        }
        draw(window, x, y);
        processed = true;
    }

};