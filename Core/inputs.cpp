//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
#include "Core.hpp"


void Core::handleInputs() {
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
    for (auto element: materialsMapping) {
        if (sf::Keyboard::isKeyPressed(element.first))
            activeMaterial = element.second;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        dynamicTileDrawing(activeMaterial, false);
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        dynamicTileDrawing(PixelType::Air, true);
}
