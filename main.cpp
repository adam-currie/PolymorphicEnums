#include "enum.h"
#include <iostream>

using namespace PolymorphicEnums;

int main() {
    typedef Enum<"red", "blue", "green", "yellow", "purple"> Color;
    typedef Subset<Color, "red", "green"> Color1;
    typedef Subset<Color, "green", "yellow", "purple"> Color2;
    typedef Subset<Color2, "green", "yellow"> Color3;
    
    auto a = Color("green");
    Color aa = "green";
    auto b = Color1("green");
    const auto c = Color2("green");
    auto d = Color3("green");

    //the order is worked out at compile time and then the case functions are reordered in N steps with no comparisons at construction time
    auto sw = Color::SwitchAll(
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

    auto sw2 = Color2::SwitchAll(
        //this, would fail at compile time 
        // Case<"red">([](){
        //     std::cout << "case: red" << std::endl;
        // }),  
        Case<"green">([](){
            std::cout << "case: green" << std::endl;
        }), 
        Case<"yellow">([](){
            std::cout << "case: yellow" << std::endl;
        }), 
        Case<"purple">()
    );

    return 0;
}