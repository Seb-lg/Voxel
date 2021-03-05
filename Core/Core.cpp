//
// Created by seub on 03/03/2021.
//

#include <iostream>
#include <thread>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core(): perlin(6969420) {
    screen.create(sf::VideoMode(1920, 1080, 32), "SandEngine", sf::Style::Titlebar | sf::Style::Close);
//    screen.setVerticalSyncEnabled(true);
    screen.setFramerateLimit(fps);
}

Core::~Core() noexcept = default;

void Core::initChunks() {
    int size = 2;
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            std::cout << "Map init: " << (float)(x*size+y) / (size*size) * 100.0 << "%\r" << std::flush;
            chunks[x][y] = std::make_shared<Chunk>(x,y);
        }
    }
    std::cout << "Map init: 100%" << std::endl;
}

bool Core::run() {
    auto now = getTime();
    sf::Event event{};

    while (screen.pollEvent(event)) {
        if ((event.type == sf::Event::EventType::KeyPressed && event.key.code == 36) || event.type == sf::Event::Closed)
            return false;
    }

    screen.clear(sf::Color::Black);
    updateTexture();
    screen.display();
    std::cout << "fps : " << 1/ ((getTime() - now) / 1000.0) << "\r" << std::flush;
    return true;
}

void Core::updateTexture() {
    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            elem.second->update();
        }
    }
}

std::shared_ptr<Pixel> Core::getTile(int x, int y) {
    static const double frequency = 8;
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
