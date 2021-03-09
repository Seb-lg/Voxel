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
        sf::Vector2<int> mouse_pos = mouse.getPosition(screen);
        sf::Vector2<int> pixel_pos = mouse_pos / pixel_size;
        TileResponse flag = addTile(pixel_pos, std::make_shared<Sand>(), true);
        if (flag == TileResponse::OOB)
            std::cout << "not in sim" << std::endl;
        else if (flag == TileResponse::ALREADY_CREATED)
            printf("Already exists: x=%d, y=%d\n", pixel_pos.x, pixel_pos.y);
    }
    if (mouse.isButtonPressed(sf::Mouse::Button::Right)) {
        sf::Vector2<int> mouse_pos = mouse.getPosition(screen);
        sf::Vector2<int> pixel_pos = mouse_pos / pixel_size;
        TileResponse flag = addTile(pixel_pos, std::make_shared<Pixel>(), true);
        if (flag == TileResponse::OOB)
            std::cout << "not in sim" << std::endl;
        else if (flag == TileResponse::ALREADY_CREATED)
            printf("Already exists: x=%d, y=%d\n", pixel_pos.x, pixel_pos.y);
    }

    screen.clear(sf::Color::Black);
    updateChunks();
    screen.display();

    std::cout << "fps : " << 1 / ((getTime() - now) / 1000.0) << "\r" << std::flush;
    return true;
}

/*void Core::updateChunks() {

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
}*/

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

TileResponse Core::addTile(sf::Vector2<int> pixelPos, std::shared_ptr<Pixel> newTile, bool createChunk) {
    // TMP HACK, AS IT ASSUMES CHUNK (0,0) IS AT TOP LEFT
    // WILL BREAK WITH PLAYER MOVEMENT IMPLEMENTATION
    std::shared_ptr<Chunk> chunk = getChunk(pixelPos / chunk_size, createChunk);
    if (chunk == nullptr)
        // TMP FIX, the chunk should be created in getChunk()
        return TileResponse::OOB;
    sf::Vector2<int> offset = sf::Vector2i(pixelPos.x % chunk_size, pixelPos.y % chunk_size);
    return chunk->replaceTile(offset, newTile);
}

std::shared_ptr<Chunk> Core::getChunk(sf::Vector2<int> chunk_idxes, bool createChunk) {
    // CREATE A NEW CHUNK IF NEEDED!!!
    auto itX = chunks.find(chunk_idxes.x);
    if (itX != chunks.end()) {
        auto itY = itX->second.find(chunk_idxes.y);
        if (itY != itX->second.end())
            return itY->second;
    }
    // Not found
    return nullptr;
}

std::shared_ptr<Pixel> Core::createTileFromPerlin(int x, int y) {
    // Called by the Chunk class constructor, will determine if the pixel exists
    // or not, based on the perlin noise
    static const double frequency = 1440 / 8;
    static const int octaves = 10;

    auto noise = static_cast<unsigned char>(perlin.accumulatedOctaveNoise2D_0_1(x / frequency, y / frequency, octaves) * 255.0);
    return noise > 255.0/2 ? std::make_shared<Pixel>(): std::make_shared<Sand>();
}
