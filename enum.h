#pragma once

#include <type_traits>
#include <concepts>
#include <algorithm>

template <auto N>
struct CompileTimeString {
    char value[N];
    consteval CompileTimeString(const char (&s)[N]) {
        std::copy_n(s, N, value);
    }
};


// template <typename... Ts>
// struct D {
//     static constexpr int value = 420;
// };

// template <>
// struct D<> {
//     static constexpr int value = 666;
// };

// #include <string>
// #include <typeinfo>
// template<CompileTimeString... Stringstypename T = int>
// std::string f() {
//     std::string s = typeid(T).name();
//     return s;
// }

template<CompileTimeString... Values>
struct Enum;

template<>
struct Enum<> {
    static constexpr char const * const values[] = {};//todo:remove?
};

template<CompileTimeString... Values>
struct Enum : Enum<> {
    static constexpr char const * const values[] = {Values.value...};
};

template<typename Base, CompileTimeString... SubValues>
concept SuperSet = std::is_base_of<Enum<>, Base>::value;//todo: check values are subset

template<SuperSet Base, CompileTimeString... Values>
struct Subset : Base {
    static constexpr char const * const values[] = {Values.value...};
};