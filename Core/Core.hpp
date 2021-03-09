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
#include "../Components/Chunk.hpp"

class Core {
public:
    static Core &get() {
        static Core core;
        return core;
    }
    void initChunks();
    bool run();
    std::shared_ptr<Pixel> getTile(int x, int y);

private:
    Core();
    ~Core();
    void updateChunks();
    int isInSim(int x, int y);

public:
    sf::RenderWindow screen;
    sf::Mouse mouse;

private:
    sf::Vector2i position;

    siv::PerlinNoise perlin;
    std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks;
    std::map<int, std::map<int, std::shared_ptr<Pixel>>> map;
};
