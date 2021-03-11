//
// Created by seub on 07/03/2021.
//

#include "Pixel.hpp"

Pixel::Pixel(
    sf::Vector2i globalIdx, PixelType type,
    sf::Color color, int var, int density, uint life
):
    type(type), processed(false),
    sprite(sf::Quads, 4), density(density), life(life)
{
    // Set vertices color
    var = std::rand() % var - int(var / 2);
    auto pixel_color = sf::Color(color.r + var, color.g + var, color.b + var);
    sprite[0].color = pixel_color;
    sprite[1].color = pixel_color;
    sprite[2].color = pixel_color;
    sprite[3].color = pixel_color;
    // Apply position in VertexArray
    auto drawIdx = globalIdx * pixel_size;
    sprite[0].position = sf::Vector2f(drawIdx.x, drawIdx.y);
    sprite[1].position = sf::Vector2f(drawIdx.x + pixel_size, drawIdx.y);
    sprite[2].position = sf::Vector2f(drawIdx.x + pixel_size, drawIdx.y + pixel_size);
    sprite[3].position = sf::Vector2f(drawIdx.x, drawIdx.y + pixel_size);
};

void Pixel::update(Surrounding surrounding, sf::Vector2<int> pos, sf::Vector2<int> chunk_pos)
{}

void Pixel::draw(sf::RenderTexture &rawGameTexture) {
    if (type != PixelType::Air)
        rawGameTexture.draw(sprite);
}

void Pixel::swapTiles(
        std::shared_ptr<Pixel> *fst, std::shared_ptr<Pixel> *snd,
        sf::Vector2<int> pos, sf::Vector2<int> chunk_pos
) {
    // We're not using the Pixel position (idx) to draw anymore,
    // as this was creating a loooot of computations and reassignations:
    // chunkIdx + Idx -> screenPosition
    // VertexArray = screenPosition (for each vertex)
    // SO! Now we just compute those at THE CREATION of the Pixel object
    // and we swap the VertexArrays in this function:

    auto tmp = (*fst);
    (*fst) = (*snd);
    (*snd) = tmp;

    // Why this does not work? x)
    // sf::VertexArray tmpVertexArray = sprite;
    // sprite = (*fst)->sprite;
    // (*fst)->sprite = tmpVertexArray;
    auto tmp_pos0 = sprite[0].position;
    auto tmp_pos1 = sprite[1].position;
    auto tmp_pos2 = sprite[2].position;
    auto tmp_pos3 = sprite[3].position;
    sprite[0].position = (*fst)->sprite[0].position;
    sprite[1].position = (*fst)->sprite[1].position;
    sprite[2].position = (*fst)->sprite[2].position;
    sprite[3].position = (*fst)->sprite[3].position;
    (*fst)->sprite[0].position = tmp_pos0;
    (*fst)->sprite[1].position = tmp_pos1;
    (*fst)->sprite[2].position = tmp_pos2;
    (*fst)->sprite[3].position = tmp_pos3;
}

Concrete::Concrete(sf::Vector2i globalIdx):
    Pixel(globalIdx, PixelType::Concrete, sf::Color(100, 100, 100), 10, INT_MAX)
{};
