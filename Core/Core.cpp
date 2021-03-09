//
// Created by seub on 03/03/2021.
//

#include <iostream>
#include <thread>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core(): perlin(rand_seed) {
    screen.create(
        sf::VideoMode(width, height, 32),
        "SandEngine",
        sf::Style::Titlebar | sf::Style::Close
    );
//    screen.setVerticalSyncEnabled(true);
    screen.setFramerateLimit(fps);
}

Core::~Core() noexcept = default;

void Core::initChunks() {
    // Create each chunks, which handle their own pixels creation
    for (int x = 0; x < nb_chunk; ++x) {
        for (int y = 0; y < nb_chunk; ++y) {
            float percentage = (float)(x*nb_chunk+y) / (nb_chunk*nb_chunk) * 100.0;
            std::cout << "Map init: " << percentage << "%\r" << std::flush;
            chunks[x][y] = std::make_shared<Chunk>(x, y);
        }
    }
    std::cout << "Map init: 100%" << std::endl;
}

bool Core::run() {
    auto now = getTime();
    sf::Event event{};
    // Check for quit event
    while (screen.pollEvent(event)) {
        if ((event.type == sf::Event::EventType::KeyPressed && event.key.code == 36)
            || event.type == sf::Event::Closed)
            return false;
    }
    if (mouse.isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2<int> mouse_pos = mouse.getPosition(screen);
        sf::Vector2<int> pixel_pos = mouse_pos / pixel_size;
        if (isInSim(pixel_pos.x, pixel_pos.y)) {
            std::cout << "===============" << '\n';
            std::cout << nb_chunk * chunk_size << '\n';
            printf("x=%d, y=%d\n", pixel_pos.x, pixel_pos.y);




            if (getTile(pixel_pos.x, pixel_pos.y).type() == PixelType::Sand) {
                }
        } else {
            std::cout << "not in sim" << '\n';
        }
    }

    screen.clear(sf::Color::Black);
    updateChunks();
    screen.display();

    std::cout << "fps : " << 1 / ((getTime() - now) / 1000.0) << "\r" << std::flush;
    return true;
}

int Core::isInSim(int x, int y) {
    static const int simSize = nb_chunk * chunk_size;
    if (x >= 0 && y >= 0 && x < simSize && y < simSize)
        return true;
    return false;
}

void Core::updateChunks() {
    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second)
                elem.second->update(chunks);
        }
    }
    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                for (auto &pixel : elem.second->pixels)
                    pixel->processed = false;
            }
        }
    }
}

std::shared_ptr<Pixel> Core::getTile(int x, int y) {
    // Called by the Chunk class constructor, will determine if the pixel exists
    // or not, based on the perlin noise
    static const double frequency = 8;
    static const int octaves = 10;
    static const double fx = (float)width / frequency;
    static const double fy = (float)height / frequency;

    // If tile already exists, return it
    auto itX = map.find(x);
    if (itX != map.end()) {
        auto itY = itX->second.find(y);
        if (itY != itX->second.end())
            return itY->second;
    }
    // Otherwise, create
    auto noise = static_cast<unsigned char>(
        perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves) * 255.0
    );
    // Fill "half the screen" with sand and the other half with Pixel (empty)
    return map[x][y] = noise > 255.0/2 ? std::make_shared<Pixel>(): std::make_shared<Sand>();
}
