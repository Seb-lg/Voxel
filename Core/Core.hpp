//
// Created by seub on 03/03/2021.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include "../Components/Pixel.hpp"
#include "../Components/Chunk.hpp"


class Core {
public:
    static Core &get() {
        static Core core;
        return core;
    }
    bool run();

private:
    Core();
    ~Core();

    void handleInputs();
    void dynamicTileDrawing(std::shared_ptr<Pixel> newTile, bool override);
    void replaceTile(std::shared_ptr<Pixel> newTile, sf::Vector2<int> pixelPos, bool override);
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


public:
    sf::RenderWindow screen;
    sf::RenderTexture rawGameTexture;

private:
    sf::Vector2i position;

    siv::PerlinNoise perlin;
    std::map<int, std::map<int, std::shared_ptr<Chunk>, std::greater<int>>> chunks;

    std::shared_ptr<Pixel> activeMaterial;
    std::map<sf::Keyboard::Key, std::shared_ptr<Pixel>> materialsMapping;
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
};

template <typename T>
using s = std::shared_ptr<T>;
