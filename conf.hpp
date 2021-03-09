//
// Created by seub on 03/03/2021.
//

#pragma once

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};

#define pixel_size 2
#define height 720
#define width 1080
#define fps 60
#define chunk_size 64
#define nb_chunk 3
#define rand_seed rd()

