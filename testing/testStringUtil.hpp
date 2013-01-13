/*
 * testStringUtil.hpp
 * Tests the full capabilities of string utility functions.
 */

#ifndef TEST_STRING_UTIL_HPP_
#define TEST_STRING_UTIL_HPP_

#include "stringutil.hpp"
#include <boost/test/unit_test.hpp>

namespace core {
BOOST_AUTO_TEST_SUITE(StringUtility)

/*
 * Tests various conversions between string types.
 */
BOOST_AUTO_TEST_CASE(stringToString) {
    BOOST_CHECK_EQUAL(toString("char star").compare("char star"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow("char star").compare("char star"), 0);

    BOOST_CHECK_EQUAL(toString(std::string("string")).compare("string"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(std::string("string")).compare("string"), 0);

    BOOST_CHECK_EQUAL(toString(L"wchar_t star").compare("wchar_t star"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(L"wchar_t star").compare("wchar_t star"), 0);

    BOOST_CHECK_EQUAL(toString(std::wstring(L"wstring")).compare("wstring"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(std::wstring(L"wstring")).compare("wstring"), 0);

    BOOST_CHECK_EQUAL(std::wstring(L"wide string").compare(
                stringToType<std::wstring>(std::string("wide string"))), 0);
    BOOST_CHECK_EQUAL(std::wstring(L"wide string").compare(
                stringToTypeNoThrow<std::wstring>(std::string("wide string"))), 0);

    UTF8 u8test[5] = {0x55, 0x54, 0x46, 0x38, 0};
    BOOST_CHECK_EQUAL(toString(u8string(u8test)).compare("UTF8"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(u8string(u8test)).compare("UTF8"), 0);
    BOOST_CHECK_EQUAL(u8string(u8test).compare(
                stringToType<u8string>(std::string("UTF8"))), 0);
    BOOST_CHECK_EQUAL(u8string(u8test).compare(
                stringToTypeNoThrow<u8string>(std::string("UTF8"))), 0);

    UTF16 u16test[6] = {0x55, 0x54, 0x46, 0x31, 0x36, 0};
    BOOST_CHECK_EQUAL(toString(u16string(u16test)).compare("UTF16"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(u16string(u16test)).compare("UTF16"), 0);
    BOOST_CHECK_EQUAL(u16string(u16test).compare(
                stringToType<u16string>(std::string("UTF16"))), 0);
    BOOST_CHECK_EQUAL(u16string(u16test).compare(
                stringToTypeNoThrow<u16string>(std::string("UTF16"))), 0);

    UTF32 u32test[6] = {0x55, 0x54, 0x46, 0x33, 0x32, 0};
    BOOST_CHECK_EQUAL(toString(u32string(u32test)).compare("UTF32"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(u32string(u32test)).compare("UTF32"), 0);
    BOOST_CHECK_EQUAL(u32string(u32test).compare(
                stringToType<u32string>(std::string("UTF32"))), 0);
    BOOST_CHECK_EQUAL(u32string(u32test).compare(
                stringToTypeNoThrow<u32string>(std::string("UTF32"))), 0);

    //TODO test 8to16, 8to32, 16to8, etc...
}

/*
 * Tests conversion from integer types to strings.
 */
BOOST_AUTO_TEST_CASE(numToString) {
    float delta = 0.00000001;
    BOOST_CHECK_EQUAL(toString(15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((unsigned int)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((unsigned int)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((short)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((short)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((unsigned short)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((unsigned short)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((long)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((long)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((unsigned long)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((unsigned long)15).compare("15"), 0);

    BOOST_CHECK_EQUAL(toString(-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toString((short)-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((short)-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toString((long)-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow((long)-14).compare("-14"), 0);

    BOOST_CHECK_EQUAL(toString(3.14 + delta).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(3.14 + delta).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toString(float(3.14 + delta)).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(float(3.14 + delta)).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toString(double(3.14 + delta)).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(double(3.14 + delta)).substr(0, 4).compare("3.14"), 0);

    BOOST_CHECK_EQUAL(toString(-1.23 - delta).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(-1.23 - delta).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toString(float(-1.23 - delta)).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(float(-1.23 - delta)).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toString(double(-1.23 - delta)).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(double(-1.23 - delta)).substr(0, 5).compare("-1.23"), 0);
}

/*
 * Checks that stringstart correctly produces strings from arbitrary data.
 */
BOOST_AUTO_TEST_CASE(stringStarter) {
    BOOST_CHECK_EQUAL((strstart << "char star").compare("char star"), 0);
    BOOST_CHECK_EQUAL((strstart << std::string("char star")).compare("char star"), 0);

    BOOST_CHECK_EQUAL((strstart << "multiple" << ' ' << std::wstring(L"string") << '-' << "types" << ' '
            << std::string("added") << ' ' << "together").compare(
                    "multiple string-types added together"), 0);

    float delta = 0.00000001;
    BOOST_CHECK_EQUAL((strstart << 15).compare("15"), 0);
    BOOST_CHECK_EQUAL((strstart << 3.14 + delta).substr(0, 4).compare("3.14"), 0);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_STRING_UTIL_HPP_ */
