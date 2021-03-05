#include <iostream>
#include "Core/Core.hpp"
#include "Components/Pixel.hpp"

int main() {
    auto &core = Core::get();
    std::cout << "Hello, World!" << std::endl;

    core.initChunks();

    while (core.run());

    return 0;
}
