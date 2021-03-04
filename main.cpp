#include <iostream>
#include "Core/Core.hpp"
#include "Components/Pixel.hpp"

int main() {
    auto &core = Core::get();
    std::cout << "Hello, World!" << std::endl;

//    auto jej = std::make_shared<Sand>();
//
//    static_cast<Pixel*>(jej.get())->update();

    while (core.run());
    return 0;
}
