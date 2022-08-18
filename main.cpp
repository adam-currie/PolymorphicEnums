#include "enum.h"
#include <iostream>

int main() {
    typedef Enum<"red", "blue", "green", "yellow", "purple"> Color;
    typedef Enum<"black", "white", "gray", "darkGray", "lightGray", "offWhite"> Shade;
    typedef Subset<Color, "red", "green"> LColor;
    typedef Subset<Color, "green", "yellow", "purple"> L2Color;
    typedef Subset<L2Color, "green", "yellow"> L3Color;
    
    auto a = Color("green");
    Color aa = "green";
    auto b = LColor("green");
    auto c = L2Color("green");
    auto d = L3Color("gre5en");

    switch (Color("purple")){
        case Shade("offWhite"):
        case Color("red"):
            std::cout << "r" << std::endl;
            break;
        case Color("blue"):
            std::cout << "b" << std::endl;
            break;
        case Color("green"):
            std::cout << "g" << std::endl;
            break;
        case Color("yellow"):
            std::cout << "y" << std::endl;
            break;
        case Color("purple"):
            std::cout << "p" << std::endl;
            break;
    }

    std::cout << (b == d) << std::endl;

    return 0;
}