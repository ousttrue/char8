#pragma once
#include <assert.h>
#include <string_view>

namespace c8
{

namespace utf8
{

enum
{
    UTF8_HEAD1 = 128,
    UTF8_HEAD2 = 128 + 64,
    UTF8_HEAD3 = 128 + 64 + 32,
    UTF8_HEAD4 = 128 + 64 + 32 + 16,
    UTF8_HEAD5 = 128 + 64 + 32 + 16 + 8,
    UTF8_MASK6 = 32 + 16 + 8 + 4 + 2 + 1,
    UTF8_MASK5 = 16 + 8 + 4 + 2 + 1,
    UTF8_MASK4 = 8 + 4 + 2 + 1,
    UTF8_MASK3 = 4 + 2 + 1,
};

static int codeunit_count_from_firstbyte(char8_t b)
{
    if (b <= 0x7F)
    {
        // ascii
        return 1;
    }
    else if (b < UTF8_HEAD2)
    {
        // error
        return -1;
    }
    else if (b < UTF8_HEAD3)
    {
        return 2;
    }
    else if (b < UTF8_HEAD4)
    {
        return 3;
    }
    else if (b < UTF8_HEAD5)
    {
        return 4;
    }
    else
    {
        // error
        return -1;
    }
}

struct codepoint
{
    uint32_t value = 0;
    char8_t *data()
    {
        return (char8_t *)&value;
    }
    const char8_t *data() const
    {
        return (const char8_t *)&value;
    }

    codepoint()
    {
    }

    codepoint(std::basic_string_view<char8_t> src)
    {
        if (src.empty())
            return;
        int size = codeunit_count_from_firstbyte(src[0]);
        auto b = data();
        b[0] = src[0];

        if (1 >= size || !src[1])
            return;
        b[1] = src[1];

        if (2 >= size || !src[2])
            return;
        b[2] = src[2];

        if (3 >= size || !src[3])
            return;
        b[3] = src[3];
    }

    bool operator==(const codepoint &rhs) const
    {
        return value == rhs.value;
    }

    int codeunit_count() const
    {
        return codeunit_count_from_firstbyte(*data());
    }

    std::basic_string_view<char8_t> view()const
    {
        return std::basic_string_view<char8_t>(data(), codeunit_count());
    }

    char32_t to_unicode() const
    {
        auto utf8 = data();
        switch (codeunit_count())
        {
        case 1:
            // ascii
            return utf8[0];

        case 2:
            return ((utf8[0] & UTF8_MASK5) << 6) | (utf8[1] & UTF8_MASK6);

        case 3:
            return ((utf8[0] & UTF8_MASK4) << 12) | ((utf8[1] & UTF8_MASK6) << 6) | (utf8[2] & UTF8_MASK6);

        case 4:
            return ((utf8[0] & UTF8_MASK3) << 18) | ((utf8[1] & UTF8_MASK6) << 12) | ((utf8[2] & UTF8_MASK6) << 6) | (utf8[3] & UTF8_MASK6);

        default:
            assert(false);
            return -1;
        }
    }
}; // namespace codepoint
static_assert(sizeof(codepoint) == 4, "utf8::codepoint.sizeof");

inline codepoint from_unicode(char32_t unicode)
{
    codepoint cp;
    auto utf8 = cp.data();
    if (unicode < (1 << 7))
    {
        utf8[0] = unicode;
    }
    else if (unicode < (1 << (5 + 6)))
    {
        utf8[0] = UTF8_HEAD2 | UTF8_MASK5 & (unicode >> 6);
        utf8[1] = UTF8_HEAD1 | UTF8_MASK6 & unicode;
    }
    else if (unicode < (1 << (4 + 6 + 6)))
    {
        utf8[0] = UTF8_HEAD3 | ((unicode >> 12));
        utf8[1] = UTF8_HEAD1 | (UTF8_MASK6 & (unicode >> 6));
        utf8[2] = UTF8_HEAD1 | (UTF8_MASK6 & unicode);
    }

    else if (unicode < (1 << (3 + 6 + 6 + 6)))
    {
        utf8[0] = UTF8_HEAD4 | ((unicode >> 18));
        utf8[1] = UTF8_HEAD1 | (UTF8_MASK6 & (unicode >> 12));
        utf8[2] = UTF8_HEAD1 | (UTF8_MASK6 & (unicode >> 6));
        utf8[3] = UTF8_HEAD1 | (UTF8_MASK6 & unicode);
    }
    else
    {
        assert(false);
    }
    return cp;
}

} // namespace utf8
} // namespace c8
