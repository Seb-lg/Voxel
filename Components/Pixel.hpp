//
// Created by seub on 04/03/2021.
//

#pragma once

class Pixel {
public:
    Pixel() = default;

    void virtual update() {
        std::cout << "NON" << std::endl;
    };
};

class Sand: public Pixel {
public:
    Sand() = default;

    void update() override {
        std::cout << "JE SUIS UNE TRIBUE" << std::endl;
    }
};