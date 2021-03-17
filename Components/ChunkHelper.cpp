//
// Created by seub on 3/12/21.
//

#include "ChunkHelper.hpp"

std::shared_ptr<Tile> *ChunkHelper::LookBasedOnVelocity(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos, std::map<int, std::map<int, std::shared_ptr<Chunk>>, std::greater<int>> &map) {
    auto const static get = [&](int x, int y) -> cptr {
        if (map[x][y])
            return map[x][y]->pixels.data();
        return nullptr;
    };
    cptr up         = get(chunk->pos.x, chunk->pos.y - 1);
    cptr upLeft     = get(chunk->pos.x - 1, chunk->pos.y - 1);
    cptr left       = get(chunk->pos.x - 1, chunk->pos.y);
    cptr downLeft   = get(chunk->pos.x - 1, chunk->pos.y + 1);
    cptr down       = get(chunk->pos.x, chunk->pos.y + 1);
    cptr downRight  = get(chunk->pos.x + 1, chunk->pos.y + 1);
    cptr right      = get(chunk->pos.x + 1, chunk->pos.y);
    cptr upRight    = get(chunk->pos.x + 1, chunk->pos.y - 1);

    auto elem = chunk->pixels.data() + pos;

    /** Check velocity to find direction
     * NOTE: increase velocity movement down is valid*/
    if (velocity.x > 0) {
        if (velocity.y < 0) {
            LookUpRight(velocity, chunk, pos, right, upLeft, up);
        } else {
            LookDownRight(velocity, chunk, pos, right, downRight, down);
        }
    } else {
        if (velocity.y < 0) {
            LookUpLeft(velocity, chunk, pos, left, upLeft, up);
        } else {
            LookDownLeft(velocity, chunk, pos, left, downLeft, down);
        }
    }

    return nullptr;
}

cptr ChunkHelper::LookUpLeft(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos, cptr left,
                        cptr upLeft, cptr up) {
    return nullptr;
}

cptr ChunkHelper::LookUpRight(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos, cptr right,
                         cptr upRight, cptr up) {
    auto dx = (velocity.x + pos % CHUNK_SIZE) / (pos % CHUNK_SIZE);
    auto dy = (velocity.y + pos / CHUNK_SIZE) / (pos / CHUNK_SIZE);

    

    return nullptr;
}

cptr ChunkHelper::LookDownLeft(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos, cptr left,
                          cptr downLeft, cptr down) {
    return nullptr;
}

cptr ChunkHelper::LookDownRight(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos, cptr right,
                           cptr downRight, cptr down) {
    return nullptr;
}
