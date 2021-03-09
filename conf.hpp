//
// Created by seub on 03/03/2021.
//

#pragma once

#define pixel_size 2
#define height 800
#define width 800
#define fps 60
#define chunk_size 64
#define nb_chunk 3
#define rand_seed 0

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};
