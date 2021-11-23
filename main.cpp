#include <iostream>
#include <thread>
#include "Core/Core.hpp"
#include "Components/Tile.hpp"
#include "Globals.hpp"

int main() {
    std::cout << std::thread::hardware_concurrency() << " available CPU cores" << std::endl;
    auto &core = Core::get();
    while (core.run());
    core.screen.close();
    core.threadPool.joinThreads();
    return 0;
}
