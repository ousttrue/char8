#include <catch.hpp>
#include <char8/char8.hpp>

TEST_CASE("sjis codepoint", "[sjis]")
{
    {
        c8::utf8::codepoint utf8(u8"A");
        auto jis = c8::jis::from_unicode(utf8.to_unicode());
        auto cp = c8::sjis::codepoint(jis);
        REQUIRE(cp.codeunit_count() == 1);
        REQUIRE(cp.c0 == 'A');
        REQUIRE(cp.c1 == 0);
        REQUIRE(cp.to_jis() == jis);
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
    }

    {
        c8::utf8::codepoint utf8(u8"ｱ");
        auto jis = c8::jis::from_unicode(utf8.to_unicode());
        auto cp = c8::sjis::codepoint(jis);
        REQUIRE(cp.codeunit_count() == 1);
        REQUIRE(cp.c0 == 0xB1);
        REQUIRE(cp.c1 == 0);
        REQUIRE(cp.to_jis() == jis);
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
    }

    {
        c8::utf8::codepoint utf8(u8"あ");
        auto jis = c8::jis::from_unicode(utf8.to_unicode());
        auto cp = c8::sjis::codepoint(jis);
        REQUIRE(cp.codeunit_count() == 2);
        REQUIRE(cp.c0 == 0x82);
        REQUIRE(cp.c1 == 0xA0);
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
        REQUIRE(cp.to_jis() == jis);
    }
}
