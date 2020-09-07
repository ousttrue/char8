#pragma once
#include <string>
#include <stdint.h>

namespace c8::unicode
{

#include "unicode.blocks.hpp"

inline Range search(const Range *begin, int n, char32_t unicode)
{
    auto half = n / 2;
    auto mid = begin + half;
    if (unicode >= mid->front & unicode <= mid->back)
    {
        return *mid;
    }

    if (unicode < mid->front)
    {
        return search(begin, half, unicode);
    }
    else if (unicode> mid->back)
    {
        return search(mid, n - half, unicode);
    }

    assert(false);
    return {};
}

inline Range get_block(char32_t unicode)
{
    size_t count = std::extent<decltype(blocks)>::value;
    return search(blocks, count, unicode);
}

} // namespace c8::unicode
