#pragma once

#include <type_traits>
#include <concepts>
#include <algorithm>
#include <string.h>

template <auto N>
struct CompileTimeString {
    char value[N];
    consteval CompileTimeString(const char (&s)[N]) {
        std::copy_n(s, N, value);
    }
};

struct _EnumBase {};

template<CompileTimeString FirstValue, CompileTimeString... MoreValues>
struct Enum : _EnumBase {
    static constexpr char const * const values[] = {FirstValue.value, MoreValues.value...};
};

template<std::derived_from<_EnumBase> Base, CompileTimeString FirstValue, CompileTimeString... MoreValues>
struct Subset : Base {
    private:
        template <size_t supersetN, size_t subsetN>
        static constexpr bool isSubset(char const * const (&superset)[supersetN], char const * const (&subset)[subsetN]) {
            for (const auto& subItem : subset) {
                bool found = false;
                for (const auto& superItem : superset) {
                    if (0 == strcmp(subItem, superItem)) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return false;
                }
            }
            return true;
        }
        static_assert((isSubset(Base::values, {FirstValue.value, MoreValues.value...})), "subset values not all present in the base set");

    public:
        static constexpr char const * const values[] = {FirstValue.value, MoreValues.value...};
};