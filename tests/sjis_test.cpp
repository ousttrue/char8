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
        REQUIRE(cp.to_jis() == jis);
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
    }

    {
        // JIS第２水準
        c8::utf8::codepoint utf8(u8"丼");
        auto jis = c8::jis::from_unicode(utf8.to_unicode());
        auto cp = c8::sjis::codepoint(jis);
        REQUIRE(cp.codeunit_count() == 2);
        REQUIRE(cp.c0 == 0x98);
        REQUIRE(cp.c1 == 0xA5);
        REQUIRE(cp.to_jis() == jis);
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
    }

    {
        // JIS第３水準
        c8::utf8::codepoint utf8(u8"彧");
        auto jis = c8::jis::from_unicode(utf8.to_unicode());
        auto cp = c8::sjis::codepoint(jis);
        REQUIRE(cp.codeunit_count() == 2);
        REQUIRE(cp.c0 == 0xEA);
        REQUIRE(cp.c1 == 0xBC);
        REQUIRE(cp.to_jis() == jis);
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
    }

    {
        // JIS第４水準
        c8::utf8::codepoint utf8(u8"㐬");
        auto unicode = utf8.to_unicode();
        auto jis = c8::jis::from_unicode(unicode);
        REQUIRE(jis.row == 0xA1); // 0x7F から第２面
        REQUIRE(jis.cell == 0xB2);
        // no sjis
        // TODO: SJIS2004
        REQUIRE(jis.to_unicode() == utf8.to_unicode());
    }
}
