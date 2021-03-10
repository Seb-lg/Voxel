//
// Created by seub on 03/03/2021.
//

#pragma once

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};

#define pixel_size 4
#define height 1024
#define width 1024
#define fps 0
#define chunk_size 64
//#define DEBUG

#ifdef DEBUG
#define rand_seed 6969420
#else
#define rand_seed rd()
#endif
