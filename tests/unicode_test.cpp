#include <catch.hpp>
#include <char8/char8.hpp>

TEST_CASE("block", "[unicode]")
{
    REQUIRE(c8::unicode::get_block(0) == c8::unicode::UnicodeBlocks::BASIC_LATIN);
    REQUIRE(c8::unicode::get_plane('a') == c8::unicode::UnicodePlanes::BMP);
    REQUIRE(c8::unicode::get_block(0x7F) == c8::unicode::UnicodeBlocks::BASIC_LATIN);
    REQUIRE(c8::unicode::get_block(0x80) != c8::unicode::UnicodeBlocks::BASIC_LATIN);
    REQUIRE(c8::unicode::get_block(U'あ') == c8::unicode::UnicodeBlocks::HIRAGANA);
    REQUIRE(c8::unicode::get_block(U'カ') == c8::unicode::UnicodeBlocks::KATAKANA);
    REQUIRE(c8::unicode::get_block(U'字') == c8::unicode::UnicodeBlocks::CJK_UNIFIED_IDEOGRAPHS);
    REQUIRE(c8::unicode::get_block(U'😀') == c8::unicode::UnicodeBlocks::EMOTICONS);
    REQUIRE(c8::unicode::get_plane(U'😀') == c8::unicode::UnicodePlanes::SMP);
    REQUIRE(c8::unicode::get_block(0x10FFFF + 1) == c8::unicode::UnicodeBlocks::NO_BLOCK); // out of range
    REQUIRE(c8::unicode::get_block(0x870) == c8::unicode::UnicodeBlocks::NO_BLOCK); // ?
}
