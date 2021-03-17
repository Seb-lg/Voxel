//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Core.hpp"


void Core::handleInputs() {
    // Screenshot
    static int screenNb = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F12)){
        sf::Vector2u windowSize = screen.getSize();
        sf::Texture texture;
        texture.create(windowSize.x, windowSize.y);
        texture.update(screen);
        sf::Image screenshot = texture.copyToImage();
        screenshot.saveToFile(std::string("screenshot") + std::to_string(screenNb) +".png");
        screenNb++;
    }
    // Material switching
    for (auto element: materialsMapping) {
        if (sf::Keyboard::isKeyPressed(element.first) && element.second != activeMaterial) {
            std::cout << pixelTypeToString(element.second) << " selected" << std::endl;
            activeMaterial = element.second;
        }
    }
    // Material drawing
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        dynamicTileDrawing(activeMaterial, false);
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        dynamicTileDrawing(PixelType::Air, true);

    if (DRAW_TILE_DEBUG) {
        // Debug text
        sf::Vector2<int> mousePos = sf::Mouse::getPosition(screen);
        debugText.setPosition(mousePos.x, mousePos.y);
        sf::Vector2<int> centerPos = mousePos / PIXEL_SIZE;
        if (centerPos.x < 0 || centerPos.y < 0)
            return;
        // maybe create a getTile() function with this?
        std::shared_ptr<Chunk> chunk = getChunk(centerPos / CHUNK_SIZE);
        sf::Vector2<int> offset = sf::Vector2i(centerPos.x % CHUNK_SIZE, centerPos.y % CHUNK_SIZE);
        std::shared_ptr<Tile> pixel = chunk->pixels[offset.y * CHUNK_SIZE + offset.x];
        std::ostringstream oss;
        oss << "(" << centerPos.x << "/" << centerPos.y << ") = " << pixelTypeToString(pixel->type);
        std::string var = oss.str();
        debugText.setString(var);
    }
}
