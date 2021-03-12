//
// Created by seub on 3/12/21.
//

#include "ChunkHelper.hpp"

std::shared_ptr<Pixel> *ChunkHelper::LookBasedOnVelocity(sf::Vector2f velocity, std::shared_ptr<Chunk> &chunk, int pos,
                                                         std::map<int, std::map<int, std::shared_ptr<Chunk>>> &map) {
    auto const static get = [&](int x, int y) -> cptr {
        if (map[x][y])
            return map[x][y]->pixels.data();
        return nullptr;
    };
    cptr up         = get(chunk->pos.x, chunk->pos.y - 1);
    cptr upLeft     = get(chunk->pos.x - 1, chunk->pos.y - 1);
    cptr Left       = get(chunk->pos.x - 1, chunk->pos.y);
    cptr downLeft   = get(chunk->pos.x - 1, chunk->pos.y + 1);
    cptr down       = get(chunk->pos.x, chunk->pos.y + 1);
    cptr downRight  = get(chunk->pos.x + 1, chunk->pos.y + 1);
    cptr right      = get(chunk->pos.x + 1, chunk->pos.y);
    cptr upRight    = get(chunk->pos.x + 1, chunk->pos.y - 1);

    auto elem = chunk->pixels.data() + pos;

    return nullptr;
}
