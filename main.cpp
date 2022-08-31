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
    const auto c = L2Color("green");
    auto d = L3Color("green");

    int ii = 420;

    auto sw = L2Color::SwitchAll(
        Case<"red">([](){
            std::cout << "case: red" << std::endl;
        }), 
        Case<"blue">([](){
            std::cout << "case: blue" << std::endl;
        }), 
        Case<"green">([](){
            std::cout << "case: green" << std::endl;
        }), 
        Case<"yellow">([](){
            std::cout << "case: yellow" << std::endl;
        }), 
        Case<"purple">()
    );

    sw("blue");
    sw("green");
    sw("purple");
    sw("red");
    sw("yellow");

    return 0;
}