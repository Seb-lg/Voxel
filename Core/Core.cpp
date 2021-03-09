//
// Created by seub on 03/03/2021.
//

#include <iostream>
#include <thread>
#include <list>
#include "Core.hpp"
#include "../conf.hpp"


Core::Core(): perlin(rand_seed) {
    std::cout << "Seed: " << rand_seed << std::endl;
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
        dynamicTileDrawing(std::make_shared<Sand>());
    } else if (mouse.isButtonPressed(sf::Mouse::Button::Right)) {
        dynamicTileDrawing(std::make_shared<Pixel>());
    }

    screen.clear(sf::Color::Black);
    updateChunks();
    screen.display();

    std::cout << "fps : " << 1 / ((getTime() - now) / 1000.0) << "\r" << std::flush;
    return true;
}

void Core::dynamicTileDrawing(std::shared_ptr<Pixel> newTile) {
    sf::Vector2<int> centerPos = mouse.getPosition(screen) / pixel_size;

    std::vector<sf::Vector2<int>> pixelsPoses = {
        centerPos + sf::Vector2i(-1, -1),
        centerPos + sf::Vector2i(0, -1),
        centerPos + sf::Vector2i(1, -1),

        centerPos + sf::Vector2i(-1, 0),
        centerPos,
        centerPos + sf::Vector2i(1, 0),

        centerPos + sf::Vector2i(-1, 1),
        centerPos + sf::Vector2i(0, 1),
        centerPos + sf::Vector2i(1, 1),
    };
    for (auto pixelPos : pixelsPoses) {
        replaceTile(newTile->clone(), pixelPos);
    }
}

void Core::replaceTile(std::shared_ptr<Pixel> newTile, sf::Vector2<int> pixelPos) {
    // TMP HACK, AS IT ASSUMES CHUNK (0,0) IS AT TOP LEFT
    // WILL BREAK WITH PLAYER MOVEMENT IMPLEMENTATION
    std::shared_ptr<Chunk> chunk = getChunk(pixelPos / chunk_size);
    sf::Vector2<int> offset = sf::Vector2i(pixelPos.x % chunk_size, pixelPos.y % chunk_size);
    TileResponse flag = chunk->replaceTile(offset, newTile);
    // OutOfBounds can only be reached for negative positions, as we don't handle those for now
    // See function docstring
    if (flag == TileResponse::OOB) {
        std::cout << "Tile not in sim" << std::endl;
        return;
    }
}

void Core::updateChunks() {
    std::vector<std::list<std::shared_ptr<Chunk>>> threadChunkList(4);

    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                auto tmp = elem.second;
                if (tmp->posY % 2 == 0 && tmp->posX % 2 == 0)
                    threadChunkList[0].push_back(elem.second);
                else if (tmp->posY % 2 == 0 && (tmp->posX - 1) % 2 == 0)
                    threadChunkList[1].push_back(elem.second);
                else if (tmp->posY % 2 && tmp->posX % 2 == 0)
                    threadChunkList[2].push_back(elem.second);
                else
                    threadChunkList[3].push_back(elem.second);
            }
        }
    }

    for (auto &chunkList: threadChunkList) {
        std::list<std::thread> threads;
        for (auto &elem : chunkList)
            threads.emplace_back([&elem, this](){elem->update(chunks);});
        for (auto &thread : threads)
            thread.join();
    }
    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                if (elem.second->posX < 3 && elem.second->posY < 3)
                    for (auto &pixel : elem.second->pixels) {
                        pixel->draw(screen);
                        pixel->processed = false;
                    }
                else
                    for (auto &pixel : elem.second->pixels) {
                        pixel->processed = false;
                    }
            }
        }
    }
}

std::shared_ptr<Chunk> Core::getChunk(sf::Vector2<int> chunk_idxes) {
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
    // Called by the Chunk class constructor, will determine if the pixel exists
    // or not, based on the perlin noise
    static const double frequency = 1440 / 8;
    static const int octaves = 10;

    auto noise = static_cast<unsigned char>(perlin.accumulatedOctaveNoise2D_0_1(x / frequency, y / frequency, octaves) * 255.0);
    return noise > 255.0/2 ? std::make_shared<Pixel>(): std::make_shared<Sand>();
}

