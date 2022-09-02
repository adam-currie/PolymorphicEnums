#pragma once

#include <algorithm>
#include <string.h>
#include <limits>
#include <functional>

namespace PolymorphicEnums {

    //todo: replace when alternative becomes part of the standard
    template <size_t N>
    struct CompileTimeString {
        char value[N];
        consteval CompileTimeString(const char (&s)[N]) {
            std::copy_n(s, N, value);
        }

        template<size_t OtherN>
        consteval auto operator<=>(const CompileTimeString<OtherN>& other) const {
            return strcmp(value, other.value);
        }

        constexpr operator char const*const() const { return value; }
    };


    /**
     * @brief specialization for <>, only used as grandfather class
     */
    template<CompileTimeString...>
    class Enum {
        protected:
            static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

            template<typename T, size_t N, auto Compare = std::compare_three_way()>
            struct SortedUniqueArray {
                T values[N];

                constexpr SortedUniqueArray(const T (&toSort)[N]) : values() {
                    std::copy_n(toSort, N, values);
                    for (size_t i=0; i<N-1; ++i) {
                        for (size_t j=i+1; j<N; ++j) {
                            auto cmp = Compare(values[i], values[j]);
                            if (cmp == 0)
                                throw "Enum value set contains duplicates";
                            if (cmp > 0)
                                std::swap(values[i], values[j]);
                        }
                    }
                }

                template<size_t SuperN>
                constexpr bool isSubsetOf(const SortedUniqueArray<T, SuperN, Compare>& superset) const {
                    size_t subIdx = 0;
                    size_t superIdx = 0;
                    while(subIdx < N) {
                        if (superIdx == SuperN) 
                            return false;
                        if (0 == Compare(superset.values[superIdx], values[subIdx]))
                            subIdx++;
                        ++superIdx;
                    }
                    return true;
                };
            };

            consteval Enum(){}

            template<size_t Count>
            static consteval size_t indexOf(char const * const (&values)[Count], const char * const toFind) {
                size_t index = 0;
                for (const auto& s : values)
                    if (0 == strcmp(s, toFind))
                        return index; else ++index;
                return INVALID_INDEX;
            };

            template<typename EnumT, typename... Cases>
            requires (std::is_base_of_v<Enum<>, EnumT>)
            class SwitchAll {
                static constexpr size_t resortingTable[EnumT::COUNT] = {indexOf(EnumT::sorted.values, Cases::V)...};

                static consteval bool noDupes() {
                    size_t occurrenceTable[EnumT::COUNT] = {0};
                    for (int i=0; i<EnumT::COUNT; ++i)
                        if (occurrenceTable[resortingTable[i]]++ != 0)
                            return false;
                    return true;
                }

                static_assert(sizeof...(Cases) == EnumT::COUNT, "wrong number of cases");
                static_assert(noDupes(), "duplicate case values detected");
                
                std::function<void()> caseFuncs[EnumT::COUNT];

                public:
                    SwitchAll(Cases... cases) { //todo: return type, autodeduced, compile time checked to be the same for for every func in every case stuct, disallow case constructor with 0 args when ret type is not void
                        std::function<void()> fs[EnumT::COUNT] = {(cases.f)...};
                        for (int i=0; i<EnumT::COUNT; ++i)
                            caseFuncs[resortingTable[i]] = fs[i];
                    }

                    void operator()(EnumT e) const {
                        caseFuncs[e.i]();
                    }
            };
    };

    template<CompileTimeString Value>
    struct Case {
        static constexpr void noop(){};
        static constexpr CompileTimeString V = Value;
        const std::function<void()> f;
        Case() : f(noop) {}
        Case(std::function<void()> f) : f(f) {}
    };

    template<CompileTimeString FirstValue, CompileTimeString... MoreValues>
    class Enum<FirstValue, MoreValues...> : private Enum<> {
        friend class Enum<>;
        protected:
            static constexpr size_t COUNT = sizeof...(MoreValues)+1;
            static constexpr auto sorted = Enum<>::SortedUniqueArray<const char *, COUNT, strcmp>({FirstValue.value, MoreValues.value...});
            const size_t i;

            consteval Enum(size_t i) : i(i) {}

            static consteval size_t indexOf(const char * const toFind) {
                const size_t idx = Enum<>::indexOf(sorted.values, toFind);
                if (idx == Enum<>::INVALID_INDEX)
                    throw "value not present in set";
                return idx;
            };

        public:
            consteval Enum(const char * const value) : i(indexOf(value)) {}

            constexpr auto operator<=>(const Enum& other) const {
                return std::compare_three_way(i, other.i);
            }

            constexpr operator char const*const() const { return sorted.values[i]; }

            //todo: switch (not all) allow you to have some cases missing, default option should be allowed as well, if using every possible case, print suggestion at compile time to maybe use SwitchAll instead

            template<typename... Cases>
            using SwitchAll = Enum<>::SwitchAll<Enum<FirstValue, MoreValues...>, Cases...>;
    };

    template<class Base, CompileTimeString FirstValue, CompileTimeString... MoreValues>
    requires (std::is_base_of_v<Enum<>,Base>)
    class Subset : public Base {
        protected:
            static constexpr auto sorted = Enum<>::SortedUniqueArray<const char *, sizeof...(MoreValues)+1, strcmp>({FirstValue.value, MoreValues.value...});
            static_assert(sorted.isSubsetOf(Base::sorted), "subset values not all present in base set");
            consteval Subset(size_t i) : Base(i) {}

        public: 
            consteval Subset(const char * const s) : Subset(Base::indexOf(s)) {
                if (Enum<>::indexOf(sorted.values, s) == Enum<>::INVALID_INDEX)
                    throw "value not present in subset";
            }

            template<typename... Cases>
            using SwitchAll = Enum<>::SwitchAll<Enum<FirstValue, MoreValues...>, Cases...>;
    };

}