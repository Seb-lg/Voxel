#include <iostream>
#include <thread>
#include "Core/Core.hpp"
#include "Components/Pixel.hpp"
#include "Globals.hpp"

int main() {
    auto mesboules = PixelType::Concrete;
    std::cout << "Cpu count: " << std::thread::hardware_concurrency() << std::endl;
    auto &core = Core::get();
    while (core.run());
    core.screen.close();
    return 0;
}
