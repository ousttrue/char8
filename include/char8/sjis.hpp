#pragma
#include "utf8.hpp"
#include "jis.hpp"

/// [Shift_JIS変形]
///
/// JISX0208 の区点(row, cell)
/// 94(0x21~7Ex) x 94(0x21~0x7E)
///
/// 区を
/// 0x81~0x9F(31) + 0xE0~0xEF(31) の区間に格納し、
/// 点を
/// 0x40~0xFC(188) の区間に格納する(0x7Fに穴が開いている)。
///
/// 区は94を格納することができない一方で、点は94の2倍以上の領域がある。
/// 区を半分に割って、点を区の奇数・偶数に応じて分配する。
/// 94/2 x (94(偶数) + 94(奇数)) に変形する。
///

namespace c8::sjis
{
struct codepoint
{
    uint8_t c0 = 0;
    uint8_t c1 = 0;

    const int FIRST_ROW_AREA = 0x81;
    const int SECOND_ROW_AREA = 0xC1;
    const int FIRST_CELL_AREA = 0x40;
    const int SECOND_CELL_AREA = FIRST_CELL_AREA +
                                 94 + // ISO-2022 charset
                                 1    // skip 0x7F
        ;

    codepoint(const jis::codepoint &cp)
    {
        if (cp.row == 0)
        {
            c0 = cp.cell;
            return;
        }

        // 区
        auto row = cp.row - 0x21;
        if (row <= 62)
        {
            c0 = (row >> 1) + FIRST_ROW_AREA;
        }
        else
        {
            c0 = (row >> 1) + SECOND_ROW_AREA;
        }

        // 点
        auto cell = cp.cell - 0x21;
        if ((row & 1) == 0)
        {
            c1 = cell + FIRST_CELL_AREA;
            if (c1 >= 0x7F)
            {
                // skip 0x7F
                ++c1;
            }
        }
        else
        {
            c1 = cell + SECOND_CELL_AREA;
        }

        auto a0 = 0;
    }

    int codeunit_count() const
    {
        if (c1 == 0)
        {
            // ascii
            return 1;
        }
        return 2;
    }

    jis::codepoint to_jis() const
    {
        if (c1 == 0)
        {
            return {c0};
        }

        // 点
        int cell = 0x21;
        int row_odd_even = (c1 >= SECOND_CELL_AREA) ? 1 : 0;
        if (row_odd_even)
        {
            cell += (c1 - SECOND_CELL_AREA);
        }
        else
        {
            cell += (c1 - FIRST_CELL_AREA);
            if (cell >= 64)
            {
                // pop 0x7F
                --cell;
            }
        }

        // 区
        int row;
        if (c0 >= SECOND_ROW_AREA)
        {
            row = (c0 - SECOND_ROW_AREA);
        }
        else
        {
            row = (c0 - FIRST_ROW_AREA);
        }

        jis::codepoint cp;
        cp.row = 0x21 + row * 2 + row_odd_even;
        cp.cell = cell;
        return cp;
    }
};

} // namespace c8::sjis
