#pragma once
#include <stdint.h>

namespace c8::jis
{

#include "unicode_jisx0213.map.hpp"

///
/// JISX0213 codepoint
/// and JISX0201 if cell == 0
///
/// ISO-2022 compatible
/// row:94(0x21 - 0x7E) x cell:94(0x21 - 0x7E)
///
union codepoint
{
    uint16_t value;

    // little endian order
    struct
    {
        // 点
        uint8_t cell;
        // 区
        uint8_t row;
    };

    bool operator==(const codepoint &rhs) const
    {
        return value == rhs.value;
    }

    char32_t to_unicode() const
    {
        if (value <= 0x7F)
        {
            // ascii
            return value;
        }
        if (value >= 0xA1 && value <= 0xDF)
        {
            // half kana. JISX0201
            return value - 0xA1 + 0xFF61;
        }
        return jisx0213_to_unicode[value - 0x2121];
    }
};
static_assert(sizeof(codepoint) == 2, "jisx0213.sizeof");

inline codepoint from_unicode(char32_t unicode)
{
    codepoint cp;
    if (unicode <= 0x7F)
    {
        // ascii
        cp.value = unicode;
    }
    else if (unicode >= 0xFF61 && unicode <= 0xFF9F)
    {
        // half kana. JISX0201
        cp.value = unicode - 0xFF61 + 0xA1;
    }
    else
    {
        cp.value = unicode_to_jisx0213[unicode];
    }
    return cp;
}

} // namespace c8::jis
