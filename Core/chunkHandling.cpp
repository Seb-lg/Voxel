//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include <list>
#include <thread>
#include "Core.hpp"


std::shared_ptr<Chunk> Core::getChunk(sf::Vector2<int> chunk_idxes) {
    // Return the chunk at the specified indexes, create it if needed
    auto itX = map.chunks.find(chunk_idxes.x);
    if (itX != map.chunks.end()) {
        auto itY = itX->second.find(chunk_idxes.y);
        if (itY != itX->second.end())
            if (itY->second)
                return itY->second;
    }
    std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(
            sf::Vector2i(chunk_idxes.x, chunk_idxes.y),
            map.perlin
    );
    map.chunks[chunk_idxes.x][chunk_idxes.y] = newChunk;
    return newChunk;
}

void Core::initChunks() {
    // Create each chunks, which handle their own pixels creation (based on perlin noise)
    auto tmp = CHUNK_SIZE * PIXEL_SIZE;
    auto chunk_width_nbr = WIDTH / tmp;
    auto chunk_height_nbr = HEIGHT / tmp;
    if (WIDTH % tmp > 0)
        chunk_width_nbr++;
    if (HEIGHT % tmp > 0)
        chunk_height_nbr++;
    auto startTime = getTime();
    for (int x = 0; x < chunk_width_nbr; ++x) {
        for (int y = 0; y < chunk_height_nbr; ++y) {
            float percentage = (float) (x * chunk_width_nbr + y) / (chunk_width_nbr * chunk_height_nbr) * 100.0;
            std::cout << "Map init: " << percentage << "\r" << std::flush;
            map.chunks[x][y] = std::make_shared<Chunk>(sf::Vector2i(x, y), map.perlin);
        }
    }
    std::cout << "Map init: " << (getTime() - startTime) << " ms" << std::endl;
    printf("%d chunks loaded (%d/%d)\n", chunk_width_nbr * chunk_height_nbr, chunk_width_nbr, chunk_height_nbr);
}

void Core::updateChunks() {
    std::vector<std::list<std::shared_ptr<Chunk>>> threadChunkList(4);

    for (auto &column : map.chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                auto tmp = elem.second;
                if (tmp->pos.y % 2 == 0 && tmp->pos.x % 2 == 0)
                    threadChunkList[0].push_back(elem.second);
                else if (tmp->pos.y % 2 == 0 && (tmp->pos.x - 1) % 2 == 0)
                    threadChunkList[1].push_back(elem.second);
                else if (tmp->pos.y % 2 && tmp->pos.x % 2 == 0)
                    threadChunkList[2].push_back(elem.second);
                else
                    threadChunkList[3].push_back(elem.second);
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        threadPool.addData(threadChunkList[i]);
        threadPool.waitForEnd();
    }


    for (auto &column : map.chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                rawGameTexture.draw(elem.second->vertices);
                for (auto &pixel : elem.second->pixels)
                    pixel->processed = false;
            }
        }
    }
}
