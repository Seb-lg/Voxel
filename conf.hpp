//
// Created by seub on 03/03/2021.
//

#pragma once

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};

#define pixel_size 4
#define height 1000
#define width 1000
#define fps 5
#define chunk_size 16
#define nb_chunk 15
#define DEBUG

#ifdef DEBUG
#define rand_seed 6969420
#else
#define rd()
#endif