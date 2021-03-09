#include <iostream>
#include <thread>
#include "Core/Core.hpp"
#include "Components/Pixel.hpp"

int main() {
    std::cout << "Cpu count: " << std::thread::hardware_concurrency() << std::endl;
    auto &core = Core::get();
    core.initChunks();
    while (core.run());
    return 0;
}
