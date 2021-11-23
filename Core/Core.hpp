//
// Created by seub on 03/03/2021.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include "../Components/Tile.hpp"
#include "../Components/Chunk.hpp"
#include "Map.hpp"
#include "../utils/ThreadPool.hpp"


class Core {
public:
    static Core &get() {
        static Core core;
        return core;
    }
    bool run();

private:
    Core();
    ~Core() {};

    void handleInputs();
    void dynamicTileDrawing(PixelType newTileType, bool override);
    void replaceTile(PixelType newTileType, sf::Vector2<int> pixelPos, bool override);
    //// Chunk handling functions
    // chunkHandling.cpp
    void initChunks();
    void updateChunks();
    std::shared_ptr<Chunk> getChunk(sf::Vector2<int> chunk_idxes);
    // utils
    sf::Vector2<int> getRandomPosition(int min, int max);
    // Shaders
    void loadShaders();
    sf::Sprite applyShaders(sf::RenderTexture &rawGameTexture);
    void draw();

public:
    sf::RenderWindow screen;
    sf::RenderTexture rawGameTexture;
    ThreadPool threadPool;

    Map map;

private:
    sf::Vector2i position;

    long lastSimTime;
    long lastFrameDrawTime;

//    siv::PerlinNoise perlin;
//    std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks;

    // Font stuff
    sf::Font font;
    sf::Text debugText;

    // Drawing material handling
    PixelType activeMaterial;
    std::map<sf::Keyboard::Key, PixelType> materialsMapping;

    // Shader stuff
    sf::ContextSettings context_settings;
    const int BLUR_RADIUS_VALUES[4] = { 250, 180, 125, 55 };
    float blur_weight;
    sf::RenderStates shader_states;
    //no blendmode! we make our own - assemble.frag
    sf::Shader luminescence_shader;
    sf::RenderTexture luminescence_render;
    sf::Shader blur_shader;
    std::array<sf::RenderTexture, 4> blur_renders;
    sf::Shader assemble_shader;
    sf::RenderTexture assemble_render;
    sf::Shader multiply_shader;
    // Pixelate shader
    sf::Shader pixelate_shader;

};

template <typename T>
using s = std::shared_ptr<T>;
