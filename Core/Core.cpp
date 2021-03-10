//
// Created by seub on 03/03/2021.
//

#include <iostream>
#include <thread>
#include <list>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core()
    : perlin(rand_seed), activeMaterialIdx(0)
{
    std::cout << "Seed: " << rand_seed << std::endl;
    screen.create(
        sf::VideoMode(width, height, 32),
        "SandEngine",
        sf::Style::Titlebar | sf::Style::Close
    );
    screen.setFramerateLimit(fps);
    materialList.push_back(std::make_shared<Sand>());
    materialList.push_back(std::make_shared<Concrete>());
    materialList.push_back(std::make_shared<Water>());
}

Core::~Core() noexcept = default;

void Core::initChunks() {
    // Create each chunks, which handle their own pixels creation (based on perlin noise)
    auto tmp = chunk_size * pixel_size;
    auto chunk_width_nbr = width / tmp;
    auto chunk_height_nbr = height / tmp;
    if (width % tmp > 0)
        chunk_width_nbr++;
    if (height % tmp > 0)
        chunk_height_nbr++;
    for (int x = 0; x < chunk_width_nbr; ++x) {
        for (int y = 0; y < chunk_height_nbr; ++y) {
            float percentage = (float)(x*chunk_width_nbr+y) / (chunk_width_nbr*chunk_height_nbr) * 100.0;
            std::cout << "Map init: " << percentage << "\r" << std::flush;
            chunks[x][y] = std::make_shared<Chunk>(x, y);
        }
    }
    printf(
        "Map init: 100%\n%d (%d/%d) Chunks loaded\n",
        chunk_width_nbr * chunk_height_nbr,
        chunk_width_nbr, chunk_height_nbr
    );
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        activeMaterialIdx++;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        activeMaterialIdx--;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        dynamicTileDrawing(materialList[activeMaterialIdx % materialList.size()], false);
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        dynamicTileDrawing(std::make_shared<Pixel>(), true);

    screen.clear(sf::Color::Black);
    updateChunks();
    screen.display();

    std::cout << "fps : " << 1 / ((getTime() - now) / 1000.0) << "\r" << std::flush;
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

void Core::updateChunks() {
    //TODO: thread line by line -> maybe hard af and render threading useless
    std::map<int, std::list<s<Chunk>>, std::greater<int>> sortedChunkList;
    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                sortedChunkList[elem.second->posY].emplace_back(elem.second);
            }
        }
    }

    std::list<std::thread> threads;
    for( auto const &list : sortedChunkList) {
        for (auto &elem : list.second) {
            if (elem->posX % 2) {
                threads.emplace_back([&](){elem->update(chunks);});
            }
        }
        for (auto &thread : threads) {
            thread.join();
        }
        threads.clear();
        for (auto &elem : list.second) {
            if (!(elem->posX % 2)) {
                threads.emplace_back([&](){elem->update(chunks);});
            }
        }
        for (auto &thread : threads) {
            thread.join();
        }
        threads.clear();
    }

    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
#ifdef DEBUG
                sf::Transform pos;
                pos.translate(elem.second->posX * chunk_size * pixel_size, elem.second->posY * chunk_size * pixel_size);
                screen.draw(elem.second->wireframe, pos);
#endif
                for (auto &pixel : elem.second->pixels) {
                    pixel->draw(screen);
                    pixel->processed = false;
                }
            }
        }
    }
}

std::shared_ptr<Chunk> Core::getChunk(sf::Vector2<int> chunk_idxes) {
    // Return the chunk at the specified indexes, create it if needed
    auto itX = chunks.find(chunk_idxes.x);
    if (itX != chunks.end()) {
        auto itY = itX->second.find(chunk_idxes.y);
        if (itY != itX->second.end())
            return itY->second;
    }
    std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(chunk_idxes.x, chunk_idxes.y);
    chunks[chunk_idxes.x][chunk_idxes.y] = newChunk;
    return newChunk;
}

std::shared_ptr<Pixel> Core::createTileFromPerlin(int x, int y) {
    // Called by the Chunk class constructor, will determine which pixel type
    // is created, based on the perlin noise
    static const double frequency = 50;
    static const int octaves = 20;

    auto noise = perlin.accumulatedOctaveNoise2D_0_1(x / frequency, y / frequency, octaves);
    if (noise < 0.3)
        return std::make_shared<Concrete>();
    if (noise < 0.4)
        return std::make_shared<Sand>();
    if (noise < 0.5)
    // if (noise < 0.3)
        return std::make_shared<Water>();
    return std::make_shared<Pixel>();
}
