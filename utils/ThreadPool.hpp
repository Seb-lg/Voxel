//
// Created by seub on 3/17/21.
//

#pragma once

#include <thread>
#include <vector>
#include <list>
#include <mutex>

class Chunk;
class Map;

class ThreadPool {
public:
    ThreadPool(int nbThread=std::thread::hardware_concurrency());

    void addData(std::list<std::shared_ptr<Chunk>> data);

    void waitForEnd();

    std::mutex mutex;
    std::mutex func;
    std::vector<std::tuple<bool, std::thread>> threads;
    std::list<std::shared_ptr<Chunk>> data;
    Map *map;
};

