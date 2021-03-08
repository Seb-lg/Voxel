//
// Created by seub on 03/03/2021.
//

#pragma once

#define pixel_size 5
#define height 1080
#define width 1920
#define fps 144
#define chunk_size 64
#define nb_chunk 2

#include <chrono>

static auto rd = [](){std::srand(std::time(nullptr));return std::rand();};
static auto getTime = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();};
