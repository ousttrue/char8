#include <catch.hpp>
#include <char8/char8.hpp>

TEST_CASE("block", "[unicode]")
{
    REQUIRE(c8::unicode::get_block(0).front == (uint32_t)c8::unicode::UnicodeBlocks::BASIC_LATIN);
    REQUIRE(c8::unicode::get_plane('a')==c8::unicode::UnicodePlanes::BMP);
    REQUIRE(c8::unicode::get_block(0x7F).front == (uint32_t)c8::unicode::UnicodeBlocks::BASIC_LATIN);
    REQUIRE(c8::unicode::get_block(0x80).front != (uint32_t)c8::unicode::UnicodeBlocks::BASIC_LATIN);
    REQUIRE(c8::unicode::get_block(U'あ').front == (uint32_t)c8::unicode::UnicodeBlocks::HIRAGANA);
    REQUIRE(c8::unicode::get_block(U'カ').front == (uint32_t)c8::unicode::UnicodeBlocks::KATAKANA);
    REQUIRE(c8::unicode::get_block(U'字').front == (uint32_t)c8::unicode::UnicodeBlocks::CJK_UNIFIED_IDEOGRAPHS);
    REQUIRE(c8::unicode::get_block(U'😀').front == (uint32_t)c8::unicode::UnicodeBlocks::EMOTICONS);
    REQUIRE(c8::unicode::get_plane(U'😀')==c8::unicode::UnicodePlanes::SMP);
}
