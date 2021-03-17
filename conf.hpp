//
// Created by seub on 03/03/2021.
//
#pragma once

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};

#define PIXEL_SIZE 4
#define CHUNK_SIZE 64
#define width 1024
#define height 1024
#define fps 0
#define drawTileDebug true
#define useVertexShaders false
#define useFragmentShaders false

#ifdef DEBUG
#define rand_seed 6969420
#else
#define rand_seed rd()
#endif
