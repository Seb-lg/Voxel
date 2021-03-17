//
// Created by seub on 03/03/2021.
//
#include <iostream>
#include <thread>
#include <list>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core()
    : perlin(rand_seed), activeMaterial(PixelType::Sand)
{
    std::cout << "Seed: " << rand_seed << std::endl;
    screen.create(
        sf::VideoMode(width, height, 32),
        "SandEngine",
        sf::Style::Titlebar | sf::Style::Close
    );
    screen.setFramerateLimit(fps);
    if (!font.loadFromFile("./Assets/CozetteVector.ttf"))
        std::cout << "loading font error" << std::endl;
    debugText.setFont(font); // font is a sf::Font
    debugText.setCharacterSize(24); // in pixels, not points!
    debugText.setFillColor(sf::Color::White);

    loadShaders();
    materialsMapping[sf::Keyboard::Num1] = PixelType::Sand;
    materialsMapping[sf::Keyboard::Num2] = PixelType::Concrete;
    materialsMapping[sf::Keyboard::Num3] = PixelType::Water;

    initChunks();
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
    // Perform computations
    handleInputs();
    updateChunks();
    // Perform drawing
    rawGameTexture.clear();
    // for each chunk call draw()
    for (auto xChunks: chunks) {
        for (auto chunk: xChunks.second)
            screen.draw(chunk.second->vertices);
    }
    rawGameTexture.display();

    // Apply all the fragment shaders
    sf::Sprite finalSprite;
    if (useFragmentShaders)
        finalSprite = applyShaders(rawGameTexture);
    else
        finalSprite = sf::Sprite(rawGameTexture.getTexture());
    // Now do the final draw on the window
    screen.clear(sf::Color::Black);
    if (useVertexShaders)
      screen.draw(finalSprite, &pixelate_shader);
    else
      screen.draw(finalSprite);
    if (drawTileDebug) {
        screen.draw(debugText);
    }
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

void Core::dynamicTileDrawing(PixelType newTileType, bool override) {
    // Used to replace a tile with another (used when mouse drawing)
    sf::Vector2<int> centerPos = sf::Mouse::getPosition(screen) / PIXEL_SIZE;
    if (centerPos.x < 0 || centerPos.y < 0)
        return;
    std::vector<sf::Vector2<int>> pixelsPoses = { centerPos };
    for (int x = 0 ; x < 20 ; x++)
        pixelsPoses.push_back(centerPos + getRandomPosition(-5, 10));
    for (int x = 0 ; x < 10 ; x++)
        pixelsPoses.push_back(centerPos + getRandomPosition(-10, 20));
    for (auto pixelPos : pixelsPoses)
            replaceTile(newTileType, pixelPos, override);
}

void Core::replaceTile(PixelType newTileType, sf::Vector2<int> pixelPos, bool override) {
    // TMP HACK, AS IT ASSUMES CHUNK (0,0) IS AT TOP LEFT
    // WILL BREAK WITH PLAYER MOVEMENT IMPLEMENTATION
    std::shared_ptr<Chunk> chunk = getChunk(pixelPos / CHUNK_SIZE);
    sf::Vector2<int> offset = sf::Vector2i(pixelPos.x % CHUNK_SIZE, pixelPos.y % CHUNK_SIZE);
    TileResponse flag;
    if (newTileType == PixelType::Air)
        flag = chunk->replaceTile(offset, std::make_shared<Pixel>(pixelPos), override);
    if (newTileType == PixelType::Sand)
        flag = chunk->replaceTile(offset, std::make_shared<Sand>(pixelPos), override);
    if (newTileType == PixelType::Water)
        flag = chunk->replaceTile(offset, std::make_shared<Water>(pixelPos), override);
    if (newTileType == PixelType::Concrete)
        flag = chunk->replaceTile(offset, std::make_shared<Concrete>(pixelPos), override);
    // OutOfBounds (TileResponse::OOB) will be triggered by negative positions
    // of mouse cursor (Above and left of the window)
    // as we don't handle those for now (see function docstring)
}

sf::Vector2<int> Core::getRandomPosition(int min, int max) {
    return sf::Vector2i(rand() % max + min, rand() % max + min);
}
