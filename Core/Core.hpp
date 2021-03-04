//
// Created by seub on 03/03/2021.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include "Perlin/PerlinNoise.hpp"
#include "../Components/Pixel.hpp"

class Core {
public:
    static Core &get() {
        static Core core;
        return core;
    }

    bool run();

private:
    Core();
    ~Core();

    std::shared_ptr<Pixel> getTile(int x, int y);

    void updateTexture();

private:
    sf::RenderWindow screen;
    sf::Vector2i position;


    siv::PerlinNoise perlin;
    std::map<int, std::map<int, std::shared_ptr<Pixel>>> map;
};
