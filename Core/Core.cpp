//
// Created by seub on 03/03/2021.
//
#include <iostream>
#include <thread>
#include <list>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core()
    : threadPool(), lastSimTime(0), lastFrameDrawTime(0),
    activeMaterial(PixelType::Sand)
{
    std::cout << "Seed: " << RANDOM_SEED << std::endl;
    screen.create(
        sf::VideoMode(WIDTH, HEIGHT, 32),
        "SandEngine",
        sf::Style::Titlebar | sf::Style::Close
    );
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

    threadPool.map = &map;
}

bool Core::run() {
    auto startTime = getTime();
    sf::Event event{};
    // Check for quit event
    while (screen.pollEvent(event)) {
        if ((event.type == sf::Event::EventType::KeyPressed && event.key.code == 36)
            || event.type == sf::Event::Closed)
            return false;
    }
    auto now = getTime();
    if (now - lastSimTime > (NS_PER_SEC / SPS)) {
        std::cout << "Steps/s : " << NS_PER_SEC / (now - lastSimTime) << "\r" << std::flush;
        handleInputs();
        updateChunks();
        lastSimTime = now;
    }
    now = getTime();
    if (now - lastFrameDrawTime > (NS_PER_SEC / FPS)) {
        draw();
        // std::cout << "FPS : " << NS_PER_SEC / (now - lastFrameDrawTime) << std::endl;
        lastFrameDrawTime = now;
    }
    // Simulation frequency limiter
    auto elapsed = getTime() - startTime;
    auto nsToNextSimStep = ((NS_PER_SEC / SPS) - elapsed);
    // std::cout << "-> " << nsToNextSimStep << " ns" << std::endl;
    if (nsToNextSimStep > 0) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nsToNextSimStep));
    }
    else if (nsToNextSimStep < -20000000) {  // If more than 2 milliseconds
        std::cout << "Lagging behind by " << -nsToNextSimStep / 1000000 << " ms" << std::endl;
    }
    return true;
}

void Core::draw() {
    // First render the pixels to the rawGameTexture, not to the screen
    // this way, we can apply the shaders at the end
    rawGameTexture.clear();
    // Draw each chunk
    for (auto xChunks: map.chunks) {
        for (auto chunk: xChunks.second)
            if (chunk.second)
                rawGameTexture.draw(chunk.second->vertices);
    }
    // Update the texture with the draws
    rawGameTexture.display();
    // Create a sprite with the rawGameTexture
    // (in order to able to apply the shaders)
    sf::Sprite finalSprite;
    if (USE_FRAGMENT_SHADERS)
        finalSprite = applyShaders(rawGameTexture);
    else
        finalSprite = sf::Sprite(rawGameTexture.getTexture());
    // Now do the final draw on the window, with the vertex shaders if needed
    screen.clear(sf::Color::Black);
    if (USE_VERTEX_SHADERS)
        screen.draw(finalSprite, &pixelate_shader);
    else
        screen.draw(finalSprite);
    if (DRAW_TILE_DEBUG)
        screen.draw(debugText);
    // Final draw to screen call
    screen.display();
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
        flag = chunk->replaceTile(offset, std::make_shared<Tile>(pixelPos), override);
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
