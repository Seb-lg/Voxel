#include <iostream>
#include "Core/Core.hpp"
#include "Components/Pixel.hpp"

int main() {
    auto &core = Core::get();
    core.initChunks();
    while (core.run());
    return 0;
}
