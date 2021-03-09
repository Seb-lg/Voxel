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
#define fps 0
#define chunk_size 64
#define nb_chunk 3
#define rand_seed rd()

//#define DEBUG
//Use 100%
//#define CPU_COUNT (std::thread::hardware_concurrency() * 2)
//Use 50%
//#define CPU_COUNT std::thread::hardware_concurrency()
//Use 25%
//#define CPU_COUNT std::thread::hardware_concurrency() / 2