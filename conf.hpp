//
// Created by seub on 03/03/2021.
//
#pragma once

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};

#define PIXEL_SIZE 4
#define CHUNK_SIZE 64
#define WIDTH 1024
#define HEIGHT 1024

#define NS_PER_SEC 1000000000
#define FPS 100         // draws per second
#define SPS 200         // Simulation steps per seconds

#define USE_FRAGMENT_SHADERS false
#define USE_VERTEX_SHADERS false
#define DRAW_TILE_DEBUG false

// #define DEBUG
#ifdef DEBUG
#define RANDOM_SEED 6969420
#else
#define RANDOM_SEED rd()
#endif
