//
// Created by seub on 03/03/2021.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <langinfo.h>
#include "Core.hpp"
#include "../configuration.hpp"
#include "Perlin/PerlinNoise.hpp"

Core::Core(): perlin([](){std::srand(std::time(nullptr));return std::rand();}()) {
    screen.create(sf::VideoMode(width, height, 32), "SandEngine", sf::Style::Titlebar | sf::Style::Close);
//    screen.setVerticalSyncEnabled(true);
    screen.setFramerateLimit(10);

    texture.create(width, height);
    pixels.resize(width * height * 4, 255);

    for (int y = 0 ; y < height / granularity ; ++y) {
        std::cout << "Map init: " << (float)y / (height / granularity) * 100.0 << "%\r" << std::flush;
        for (int x = 0 ; x < width / granularity; ++x) {
            getTile(x, y);
        }
    }
    std::cout << "Map init: 100%" << std::endl;
    texture.update(pixels.data());
    sprite.setTexture(texture, true);
    sprite.setPosition(0, 0);
}

Core::~Core() noexcept = default;

bool Core::run() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    sf::Event event{};

    while (screen.pollEvent(event)) {
        if ((event.type == sf::Event::EventType::KeyPressed && event.key.code == 36) || event.type == sf::Event::Closed)
            return false;
    }
    updateTexture();
    display();
    std::cout << "fps : " << 1/ ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - now) / 1000.0) << "\r" << std::flush;
    return true;
}

void Core::updateTexture() {
    auto rgba = pixels.data();
    for (int x = 0 ; x < width / granularity; ++x) {
        auto const xEnd = map[x].end();
        for (auto it = map[x].begin(); it != xEnd; ++it){
            if (!it->second)
                continue;
            it->second->update(map, x, it->first);
//            if (it->second == 0 && next->second == 255) {
//                it->second = 255;
//                next->second = 0;
//            }
//            if (next->second == 0 && it->second == 255) {
//                next->second = 255;
//                it->second = 0;
//            }
            int vara = x * granularity * 4;
            int varb = it->first * width * granularity * 4;
            drawSquare(rgba + vara + varb, granularity, it->second);
        }
    }
//    drawSquare(pixels.data(), granularity, 125);
}

void Core::drawSquare(unsigned char *pos, int size, std::shared_ptr<Pixel> color) {
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            *pos = color->r;
            ++pos;
            *pos = color->g;
            ++pos;
            *pos = color->b;
            ++pos;
            ++pos;
        }
        pos+= (width - size)*4;
    }
}

std::shared_ptr<Pixel> Core::getTile(int x, int y) {
    static const double frequency = 8*granularity;
    static const int octaves = 10;
    static const double fx = (float)width  / frequency;
    static const double fy = (float)height  / frequency;

    auto itX = map.find(x);
    if (itX != map.end()) {
        auto itY = itX->second.find(y);
        if (itY != itX->second.end())
            return itY->second;
    }

    auto noise = static_cast<unsigned char>(perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves) * 255.0);
    return map[x][y] = noise > 255.0/2 ? std::make_shared<Pixel>(): std::make_shared<Sand>();
}

void Core::display() {
    screen.clear(sf::Color::White);

    texture.update(pixels.data());
    screen.draw(sprite);

    screen.display();
}
