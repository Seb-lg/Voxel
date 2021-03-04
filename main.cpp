#include <iostream>
#include "Core/Core.hpp"

int main() {
    auto &core = Core::get();
    std::cout << "Hello, World!" << std::endl;
    while (core.run());
    return 0;
}
