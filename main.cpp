#include "enum.h"
#include <iostream>

int main() {
    typedef Enum<"red", "blue", "green", "yellow"> Color;
    typedef Subset<Color, "red", "blue"> LColor;

    for (auto& c : Color::values)
        std::cout << c << std::endl;

    std::cout << std::endl;

    for (auto& c : LColor::values)
        std::cout << c << std::endl;

    return 0;
}