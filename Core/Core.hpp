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
    void dynamicTileDrawing(std::shared_ptr<Pixel> newTile, bool override);
    void replaceTile(std::shared_ptr<Pixel> newTile, sf::Vector2<int> pixelPos, bool override);
    std::shared_ptr<Chunk> getChunk(sf::Vector2<int> chunk_idxes);
    std::shared_ptr<Pixel> createTileFromPerlin(int x, int y);
    sf::Vector2<int> getRandomPosition(int min, int max);


private:
    Core();
    ~Core();
    void updateChunks();
    int isInSim(int x, int y);

public:
    sf::RenderWindow screen;

private:
    sf::Vector2i position;

    siv::PerlinNoise perlin;
    std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks;

    int activeMaterialIdx;
    std::vector<std::shared_ptr<Pixel>> materialList;
};

template <typename T>
using s = std::shared_ptr<T>;