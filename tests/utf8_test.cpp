#include <catch.hpp>
#include <char8/char8.hpp>

TEST_CASE("utf8 codepoint", "[utf8]")
{
    {
        c8::utf8::codepoint cp(u8"a");
        REQUIRE(cp.codeunit_count() == 1);
        REQUIRE(cp.to_unicode() == 'a');
    }
    {
        c8::utf8::codepoint cp(u8"©");
        REQUIRE(cp.codeunit_count() == 2);
        REQUIRE(cp.to_unicode() == 0x00A9);
        REQUIRE(cp == c8::utf8::from_unicode(U'©'));
    }
    {
        c8::utf8::codepoint cp(u8"字");
        REQUIRE(cp.codeunit_count() == 3);
        REQUIRE(cp.to_unicode() == 0x5B57);
        REQUIRE(cp == c8::utf8::from_unicode(U'字'));
    }
    {
        c8::utf8::codepoint cp(u8"🐁");
        REQUIRE(cp.codeunit_count() == 4);
        REQUIRE(cp.to_unicode() == 0x1F401);
        REQUIRE(cp == c8::utf8::from_unicode(0x1F401));
        REQUIRE(cp == c8::utf8::from_unicode(U'🐁'));
    }

    // codepoint take first codepoint
    // BMP
    {
        c8::utf8::codepoint cp(u8"ab");
        REQUIRE(cp.codeunit_count() == 1);
        REQUIRE(cp.value == 'a');
    }
    {
        c8::utf8::codepoint cp(u8"©©");
        REQUIRE(cp.codeunit_count() == 2);
    }
    {
        c8::utf8::codepoint cp(u8"文字");
        REQUIRE(cp.codeunit_count() == 3);
        REQUIRE(cp.data()[3] == 0);
    }
    {
        // SMP
        c8::utf8::codepoint cp(u8"🐁🐃");
        REQUIRE(cp.codeunit_count() == 4);
    }
    {
        // SIP
        c8::utf8::codepoint cp(u8"𠃍");
        REQUIRE(cp.codeunit_count() == 4);
    }
    // TIP
}
