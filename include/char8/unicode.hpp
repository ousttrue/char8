#pragma once
#include <string>
#include <stdint.h>

namespace c8::unicode
{

enum class UnicodePlanes
{
    BMP, // Basic Multilingual Plane
    SMP, // Supplementary Multilingual Plane
    SIP, // Supplementary Ideographic Plane
    TIP, // Tertiary Ideographic Plane
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _A,
    _B,
    _C,
    _D,
    SSP,    // Supplement­ary Special-purpose Plane
    SPUA_A, // Supplement­ary Private Use Area-A
    SPUA_B, // Supplement­ary Private Use Area-B
    NoBlock,
};

inline UnicodePlanes get_plane(char32_t unicode)
{
    return (UnicodePlanes)(unicode >> 16);
}

#include "unicode.blocks.hpp"

inline UnicodeBlock search(const UnicodeBlock *begin, int n, char32_t unicode)
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
    else if (unicode > mid->back)
    {
        return search(mid, n - half, unicode);
    }

    assert(false);
    return {};
}

inline UnicodeBlock get_block(char32_t unicode)
{
    size_t count = std::extent<decltype(blocks)>::value;
    return search(blocks, count, unicode);
}

} // namespace c8::unicode
