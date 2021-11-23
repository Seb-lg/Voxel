//
// Created by seub on 3/17/21.
//

#include "ThreadPool.hpp"
#include "../Components/Chunk.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

ThreadPool::ThreadPool(int nbThread) {
    threads.reserve(nbThread);
    for (int i = 0; i < nbThread; ++i) {
        threads.emplace_back(false, [this, i](){
            std::shared_ptr<Chunk> elem;
            while (true) {
                this->mutex.lock();
                if (!this->data.empty()) {
                    elem = this->data.front();
                    this->data.pop_front();
                }
                this->mutex.unlock();
                if (elem) {
                    this->func.lock();
                    std::get<0>(this->threads[i]) = true;
                    this->func.unlock();
                    elem->update(*this->map);
                    elem.reset();
                    this->func.lock();
                    std::get<0>(this->threads[i]) = false;
                    this->func.unlock();
                }
            }
        });
    }
}

void ThreadPool::joinThreads()
{
    for (auto& it : threads)
        std::get<1>(it).join();
}

void ThreadPool::addData(std::list<std::shared_ptr<Chunk>> dataIn) {
    mutex.lock();
    for (auto &elem : dataIn) {
        data.push_back(elem);
    }
    mutex.unlock();
}

void ThreadPool::waitForEnd() {
    while (true) {
        mutex.lock();
        if (data.size() == 0) {
            mutex.unlock();
            func.lock();

            bool theEnd = true;
            for (auto &elem : threads)
                if (std::get<0>(elem))
                    theEnd = false;

            func.unlock();
            if (theEnd)
                return;
        } else
            mutex.unlock();
    }
}

#pragma clang diagnostic pop
