//
// Created by seub on 03/03/2021.
//
#include <iostream>
#include <thread>
#include <list>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core()
    : perlin(rand_seed), activeMaterial(std::make_shared<Sand>())
{
    std::cout << "Seed: " << rand_seed << std::endl;
    screen.create(
        sf::VideoMode(width, height, 32),
        "SandEngine",
        sf::Style::Titlebar | sf::Style::Close
    );
    screen.setFramerateLimit(fps);
    materialsMapping[sf::Keyboard::Num1] = std::make_shared<Sand>();
    materialsMapping[sf::Keyboard::Num2] = std::make_shared<Concrete>();
    materialsMapping[sf::Keyboard::Num3] = std::make_shared<Water>();

    initChunks();
}

Core::~Core() noexcept = default;

bool Core::run() {
    auto now = getTime();
    sf::Event event{};
    // Check for quit event
    while (screen.pollEvent(event)) {
        if ((event.type == sf::Event::EventType::KeyPressed && event.key.code == 36)
            || event.type == sf::Event::Closed)
            return false;
    }

    handleInputs();
    screen.clear(sf::Color::Black);
    updateChunks();
    screen.display();

    std::cout << "fps : " << 1 / ((getTime() - now) / 1000.0) << "\r" << std::flush;

    /*static int oui = width/2;
    auto pos = sf::Vector2f(oui, height/2);
    auto size = sf::Vector2f(width, height);
    oui++;
    sf::View jej(pos, size);
    this->screen.setView(jej);*/
    return true;
}

void Core::dynamicTileDrawing(std::shared_ptr<Pixel> newTile, bool override) {
    // Used to replace a tile with another (used when mouse drawing)
    sf::Vector2<int> centerPos = sf::Mouse::getPosition(screen) / pixel_size;
    if (centerPos.x < 0 || centerPos.y < 0)
        return;
    std::vector<sf::Vector2<int>> pixelsPoses = { centerPos };
    for (int x = 0 ; x < 20 ; x++)
        pixelsPoses.push_back(centerPos + getRandomPosition(-5, 10));
    for (int x = 0 ; x < 10 ; x++)
        pixelsPoses.push_back(centerPos + getRandomPosition(-10, 20));
    for (auto pixelPos : pixelsPoses)
        replaceTile(newTile->clone(), pixelPos, override);
}

sf::Vector2<int> Core::getRandomPosition(int min, int max) {
    return sf::Vector2i(rand() % max + min, rand() % max + min);
}

void Core::replaceTile(std::shared_ptr<Pixel> newTile, sf::Vector2<int> pixelPos, bool override) {
    // TMP HACK, AS IT ASSUMES CHUNK (0,0) IS AT TOP LEFT
    // WILL BREAK WITH PLAYER MOVEMENT IMPLEMENTATION
    std::shared_ptr<Chunk> chunk = getChunk(pixelPos / chunk_size);
    sf::Vector2<int> offset = sf::Vector2i(pixelPos.x % chunk_size, pixelPos.y % chunk_size);
    TileResponse flag = chunk->replaceTile(offset, newTile, override);
    // OutOfBounds (TileResponse::OOB) will be triggered by negative positions
    // of mouse cursor (Above and left of the window)
    // as we don't handle those for now (see function docstring)
}
