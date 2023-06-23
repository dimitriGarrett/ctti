#pragma once

#include "cstring.h"
#include <stdint.h>

namespace ctti
{
    using hash_t = decltype(sizeof(nullptr));

#if INTPTR_MAX == INT32_MAX
    constexpr hash_t FNVOffset = 2166136261;
    constexpr hash_t FNVPrime = 16777619;
#elif INTPTR_MAX == INT64_MAX
    constexpr hash_t FNVOffset = 0xcbf29ce484222325;
    constexpr hash_t FNVPrime = 1099511628211;
#else
    #error "Unkown architecture"
#endif

    template <typename char_type>
    static constexpr hash_t hash(const char_type* str)
    {
        hash_t result = FNVOffset;

        for (basic_string_literal_size i = 0; i < string_utils::cstring_length(str); ++i)
        {
            result ^= str[i];
            result *= FNVPrime;
        }

        return result;
    }

    template <typename char_type>
    static constexpr hash_t hash(const basic_cstring<char_type>& str)
    {
        hash_t result = FNVOffset;

        for (char_type c : str)
        {
            result ^= c;
            result *= FNVPrime;
        }

        return result;
    }
}

namespace std
{
    template <typename char_type>
    struct hash<ctti::basic_cstring<char_type>>
    {
        constexpr typename ctti::basic_cstring<char_type>::size_type operator() (const ctti::basic_cstring<char_type>& lhs) const
        {
            return ctti::hash(lhs);
        }
    };
}