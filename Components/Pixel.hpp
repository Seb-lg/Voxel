//
// Created by seub on 04/03/2021.
//

#pragma once
#define uchar unsigned char
#include "../configuration.hpp"

enum class PixelType {
    Air = 0,
    Sand = 1,
};

class Pixel {
public:
    Pixel(uchar r=255, uchar g=255, uchar b=255, uchar a=255):r(r), g(g), b(b), a(a), type(PixelType::Air) {
    };

    virtual void update(std::map<int, std::map<int, std::shared_ptr<Pixel>>> &map, int x, int y) {
    };

public:
    uchar r;
    uchar g;
    uchar b;
    uchar a;

    PixelType type;
};

class Sand: public Pixel {
public:
    Sand(): Pixel(0, 0, 0) {
        type = PixelType::Sand;
    };

    void update(std::map<int, std::map<int, std::shared_ptr<Pixel>>> &map, int x, int y) override {
        if (y != height / granularity -1 && map[x][y + 1]->type == PixelType::Air) {
            auto tmp = map[x][y + 1];
            map[x][y + 1] = map[x][y];
            map[x][y] = tmp;
        }
    }

};