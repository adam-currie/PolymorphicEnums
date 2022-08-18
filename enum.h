#pragma once

#include <algorithm>
#include <string.h>

//todo: replace
template <size_t N>
struct CompileTimeString {
    char value[N];
    consteval CompileTimeString(const char (&s)[N]) {
        std::copy_n(s, N, value);
    }

    template<auto OtherN>
    consteval bool operator ==(const CompileTimeString<OtherN>& other) const {
        return 0 == strcmp(value, other.value);
    }
};

/**
 * @brief specialization for <>, only used as grandfather class
 */
template<CompileTimeString... MoreValues>
class Enum {
    protected:
        consteval Enum(){}

        template<size_t SetCount>
        static consteval size_t indexOf(char const * const (&set)[SetCount], const char * const toFind) {
            size_t index = 0;
            for (const auto& s : set)
                if (0 == strcmp(s, toFind))
                    return index; else ++index;
            throw "value not present in set";
        };
};

template<CompileTimeString FirstValue, CompileTimeString... MoreValues>
class Enum<FirstValue, MoreValues...> : private Enum<> {
    protected:
        const size_t i;

        consteval Enum(size_t i) : i(i) {}

        static consteval size_t indexOf(const char * const toFind) {
            return Enum<>::indexOf(values, toFind);
        };

    public:
        static constexpr const char * const values[] = {FirstValue.value, MoreValues.value...};
        
        consteval Enum(const char * const s) : i(indexOf(s)) {}

        constexpr bool operator==(const Enum& other) const { return i == other.i; }
        constexpr bool operator==(const Enum&& other) const { return i == other.i; }

        constexpr operator char const*const() const { return values[i]; }
        constexpr operator size_t() const { return i; }
};

template<class Base, CompileTimeString FirstValue, CompileTimeString... MoreValues>
requires (std::is_base_of_v<Enum<>,Base>)
class Subset : public Base {
    private:
        template <size_t supersetN, size_t subsetN>
        static consteval bool isSubset(const char * const (&superset)[supersetN], const char * const(&subset)[subsetN]) {
            for (const auto& s : subset)
                if (666 == Enum<>::indexOf(superset, s))//todo: replace 666
                    return false;
            return true;
        }

        static_assert(
            (isSubset(Base::values, {FirstValue.value, MoreValues.value...})), 
            "subset values not all present in the base set"
        );

    protected:
        consteval Subset(size_t i) : Base(i) {}

    public:
        static constexpr char const * const values[] = {FirstValue.value, MoreValues.value...};
        
        consteval Subset(const char * const s) : Subset(Base::indexOf(s)) {
            if (Base::i == 666)//todo: replace 666
                throw "value not present in subset";
        }
};