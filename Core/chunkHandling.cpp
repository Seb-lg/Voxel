//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Core.hpp"


std::shared_ptr<Chunk> Core::getChunk(sf::Vector2<int> chunk_idxes) {
    // Return the chunk at the specified indexes, create it if needed
    auto itX = chunks.find(chunk_idxes.x);
    if (itX != chunks.end()) {
        auto itY = itX->second.find(chunk_idxes.y);
        if (itY != itX->second.end())
            return itY->second;
    }
    std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(
        sf::Vector2i(chunk_idxes.x, chunk_idxes.y),
        perlin
    );
    chunks[chunk_idxes.x][chunk_idxes.y] = newChunk;
    return newChunk;
}

void Core::initChunks() {
    // Create each chunks, which handle their own pixels creation (based on perlin noise)
    auto tmp = CHUNK_SIZE * PIXEL_SIZE;
    auto chunk_width_nbr = width / tmp;
    auto chunk_height_nbr = height / tmp;
    if (width % tmp > 0)
        chunk_width_nbr++;
    if (height % tmp > 0)
        chunk_height_nbr++;
    for (int x = 0; x < chunk_width_nbr; ++x) {
        for (int y = 0; y < chunk_height_nbr; ++y) {
            float percentage = (float)(x*chunk_width_nbr+y) / (chunk_width_nbr*chunk_height_nbr) * 100.0;
            std::cout << "Map init: " << percentage << "\r" << std::flush;
            chunks[x][y] = std::make_shared<Chunk>(sf::Vector2i(x, y), perlin);
        }
    }
    std::cout << "Map init: 100%" << std::endl << chunk_width_nbr * chunk_height_nbr << " ("<<chunk_width_nbr<<"/"<<chunk_width_nbr<<") Chunks loaded" << std::endl;
}

void Core::updateChunks() {
    //TODO: thread line by line -> maybe hard af and render threading useless
    std::map<int, std::list<s<Chunk>>, std::greater<int>> sortedChunkList;
    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                sortedChunkList[elem.second->pos.y].emplace_back(elem.second);
            }
        }
    }

    std::list<std::thread> threads;
    for( auto const &list : sortedChunkList) {
        for (auto &elem : list.second) {
            if (elem->pos.x % 2) {
                threads.emplace_back([&](){elem->update(chunks);});
            }
        }
        for (auto &thread : threads) {
            thread.join();
        }
        threads.clear();
        for (auto &elem : list.second) {
            if (!(elem->pos.x % 2)) {
                threads.emplace_back([&](){elem->update(chunks);});
            }
        }
        for (auto &thread : threads) {
            thread.join();
        }
        threads.clear();
    }

    for (auto &column : chunks) {
        for (auto &elem : column.second) {
            if (elem.second) {
                rawGameTexture.draw(elem.second->vertices);
// #ifdef DEBUG
//                 sf::Transform pos;
//                 pos.translate(elem.second->pos.x * CHUNK_SIZE * PIXEL_SIZE, elem.second->pos.y * CHUNK_SIZE * PIXEL_SIZE);
//                 screen.draw(elem.second->wireframe, pos);
// #endif
//                 for (auto &pixel : elem.second->pixels) {
//                     pixel->draw(rawGameTexture);
//                     pixel->processed = false;
//                 }
            }
        }
    }
}
