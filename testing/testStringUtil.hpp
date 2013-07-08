/*
 * testStringUtil.hpp
 * Tests the full capabilities of string utility functions.
 */

#ifndef TEST_STRING_UTIL_HPP_
#define TEST_STRING_UTIL_HPP_

#include "stringutil.hpp"
#include <boost/test/unit_test.hpp>

namespace core {
BOOST_AUTO_TEST_SUITE(StringUtilityTests)

/*
 * Tests various conversions between string types.
 */
BOOST_AUTO_TEST_CASE(stringToString) {
    // Standard string to string
    BOOST_CHECK_EQUAL(toString("char star").compare("char star"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow("char star").compare("char star"), 0);

    BOOST_CHECK_EQUAL(toString(std::string("string")).compare("string"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(std::string("string")).compare("string"), 0);

    BOOST_CHECK_EQUAL(toString(L"wchar_t star").compare("wchar_t star"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(L"wchar_t star").compare("wchar_t star"), 0);

    BOOST_CHECK_EQUAL(toString(std::wstring(L"wstring")).compare("wstring"), 0);
    BOOST_CHECK_EQUAL(toStringNoThrow(std::wstring(L"wstring")).compare("wstring"), 0);

    // String to wstring
    BOOST_CHECK_EQUAL(std::wstring(L"wide string").compare(
                stringToType<std::wstring>(std::string("wide string"))), 0);
    BOOST_CHECK_EQUAL(std::wstring(L"wide string").compare(
                stringToTypeNoThrow<std::wstring>(std::string("wide string"))), 0);

    // UTF to string and visa versa
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
}

/*
 * Tests various conversions between utf string types.
 */
BOOST_AUTO_TEST_CASE(utfToUtf) {
    // UTF 8to16, 8to32, 16to8, etc...
    char ctest[4] = {'U', 'T', 'F', '\0'};
    std::string strtest(ctest);
    wchar_t wctest[4] = {L'U', L'T', L'F', L'\0'};
    std::wstring wstrtest(wctest);
    UTF8 u8test[4] = {0x55, 0x54, 0x46, 0};
    u8string u8strtest(u8test);
    UTF16 u16test[4] = {0x55, 0x54, 0x46, 0};
    u16string u16strtest(u16test);
    UTF32 u32test[4] = {0x55, 0x54, 0x46, 0};
    u32string u32strtest(u32test);

    // shi-ru (katakana)
    UTF8 u8test2[10] = {0xE3, 0x82, 0xB7, 0xE3, 0x83, 0xBC, 0xE3, 0x87, 0xBD, 0};
    u8string u8strtest2(u8test2);
    UTF16 u16test2[4] = {0x30B7, 0x30FC, 0x31FD, 0};
    u16string u16strtest2(u16test2);
    UTF32 u32test2[4] = {0x30B7, 0x30FC, 0x31FD, 0};
    u32string u32strtest2(u32test2);

    // string to UTF8
    BOOST_CHECK_EQUAL(stringToUTF8(ctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(ctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(strtest).compare(u8strtest), 0);

    // wstring to UTF8
    BOOST_CHECK_EQUAL(stringToUTF8(wctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(wctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(wstrtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(wstrtest).compare(u8strtest), 0);

    // UTF16 to UTF8
    BOOST_CHECK_EQUAL(UTF16ToUTF8(u16test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF8NoThrow(u16test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF8(u16strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF8NoThrow(u16strtest).compare(u8strtest), 0);

    BOOST_CHECK_EQUAL(UTF16ToUTF8(u16test2).compare(u8strtest2), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF8NoThrow(u16test2).compare(u8strtest2), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF8(u16strtest2).compare(u8strtest2), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF8NoThrow(u16strtest2).compare(u8strtest2), 0);

    // UTF32 to UTF8
    BOOST_CHECK_EQUAL(UTF32ToUTF8(u32test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF8NoThrow(u32test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF8(u32strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF8NoThrow(u32strtest).compare(u8strtest), 0);

    BOOST_CHECK_EQUAL(UTF32ToUTF8(u32test2).compare(u8strtest2), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF8NoThrow(u32test2).compare(u8strtest2), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF8(u32strtest2).compare(u8strtest2), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF8NoThrow(u32strtest2).compare(u8strtest2), 0);

    // Any to UTF8
    BOOST_CHECK_EQUAL(stringToUTF8(ctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(ctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(strtest).compare(u8strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF8(wctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(wctest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(wstrtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(wstrtest).compare(u8strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF8(u8test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u8test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(u8strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u8strtest).compare(u8strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF8(u16test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u16test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(u16strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u16strtest).compare(u8strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF8(u32test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u32test).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8(u32strtest).compare(u8strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u32strtest).compare(u8strtest), 0);

    // string to UTF16
    BOOST_CHECK_EQUAL(stringToUTF16(ctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(ctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(strtest).compare(u16strtest), 0);

    // wstring to UTF16
    BOOST_CHECK_EQUAL(stringToUTF16(wctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(wctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(wstrtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(wstrtest).compare(u16strtest), 0);

    // UTF8 to UTF16
    BOOST_CHECK_EQUAL(UTF8ToUTF16(u8test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF16NoThrow(u8test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF16(u8strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF16NoThrow(u8strtest).compare(u16strtest), 0);

    BOOST_CHECK_EQUAL(UTF8ToUTF16(u8test2).compare(u16strtest2), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF16NoThrow(u8test2).compare(u16strtest2), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF16(u8strtest2).compare(u16strtest2), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF16NoThrow(u8strtest2).compare(u16strtest2), 0);

    // UTF32 to UTF16
    BOOST_CHECK_EQUAL(UTF32ToUTF16(u32test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF16NoThrow(u32test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF16(u32strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF16NoThrow(u32strtest).compare(u16strtest), 0);

    BOOST_CHECK_EQUAL(UTF32ToUTF16(u32test2).compare(u16strtest2), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF16NoThrow(u32test2).compare(u16strtest2), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF16(u32strtest2).compare(u16strtest2), 0);
    BOOST_CHECK_EQUAL(UTF32ToUTF16NoThrow(u32strtest2).compare(u16strtest2), 0);

    // Any to UTF16
    BOOST_CHECK_EQUAL(stringToUTF16(ctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(ctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(strtest).compare(u16strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF16(wctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(wctest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(wstrtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(wstrtest).compare(u16strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF16(u8test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u8test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(u8strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u8strtest).compare(u16strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF16(u16test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u16test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(u16strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u16strtest).compare(u16strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF16(u32test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u32test).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16(u32strtest).compare(u16strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u32strtest).compare(u16strtest), 0);

    // string to UTF32
    BOOST_CHECK_EQUAL(stringToUTF32(ctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(ctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(strtest).compare(u32strtest), 0);

    // wstring to UTF32
    BOOST_CHECK_EQUAL(stringToUTF32(wctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(wctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(wstrtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(wstrtest).compare(u32strtest), 0);

    // UTF8 to UTF32
    BOOST_CHECK_EQUAL(UTF8ToUTF32(u8test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF32NoThrow(u8test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF32(u8strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF32NoThrow(u8strtest).compare(u32strtest), 0);

    BOOST_CHECK_EQUAL(UTF8ToUTF32(u8test2).compare(u32strtest2), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF32NoThrow(u8test2).compare(u32strtest2), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF32(u8strtest2).compare(u32strtest2), 0);
    BOOST_CHECK_EQUAL(UTF8ToUTF32NoThrow(u8strtest2).compare(u32strtest2), 0);

    // UTF16 to UTF32
    BOOST_CHECK_EQUAL(UTF16ToUTF32(u16test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF32NoThrow(u16test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF32(u16strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF32NoThrow(u16strtest).compare(u32strtest), 0);

    BOOST_CHECK_EQUAL(UTF16ToUTF32(u16test2).compare(u32strtest2), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF32NoThrow(u16test2).compare(u32strtest2), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF32(u16strtest2).compare(u32strtest2), 0);
    BOOST_CHECK_EQUAL(UTF16ToUTF32NoThrow(u16strtest2).compare(u32strtest2), 0);

    // Any to UTF16
    BOOST_CHECK_EQUAL(stringToUTF32(ctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(ctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(strtest).compare(u32strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF32(wctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(wctest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(wstrtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(wstrtest).compare(u32strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF32(u8test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u8test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(u8strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u8strtest).compare(u32strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF32(u16test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u16test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(u16strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u16strtest).compare(u32strtest), 0);

    BOOST_CHECK_EQUAL(stringToUTF32(u32test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u32test).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32(u32strtest).compare(u32strtest), 0);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u32strtest).compare(u32strtest), 0);
}

/*
 * Tests utf conversions that throw exceptions or are ignored by
 * NoThrow options.
 */
BOOST_AUTO_TEST_CASE(illegalUTF) {
    u8string u8null;
    u16string u16null;
    u32string u32null;

    // UTF8 early termination
    UTF8 u8i[3] = {0xF0, 0xA4, 0};
    u8string u8stri(u8i);

    BOOST_CHECK_THROW(stringToUTF16(u8i), CastException);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u8i).compare(u16null), 0);

    BOOST_CHECK_THROW(stringToUTF32(u8i), CastException);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u8i).compare(u32null), 0);

    // Illegal UTF8 code (4 bytes)
    UTF8 u8i2[5] = {0xF4, 0x14, 0x00, 0x00, 0};
    u8string u8stri2(u8i2);

    BOOST_CHECK_THROW(stringToUTF16(u8i2), CastException);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u8i2).compare(u16null), 0);

    BOOST_CHECK_THROW(stringToUTF32(u8i2), CastException);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u8i2).compare(u32null), 0);

    // Illegal UTF8 code (5 bytes)
    UTF8 u8i3[6] = {0xFA, 0x20, 0x00, 0x00, 0x00, 0};
    UTF16 u16expect3[2] = {0x20, 0};
    UTF32 u32expect3[2] = {0x20, 0};
    u8string u8stri3(u8i3);

    BOOST_CHECK_THROW(stringToUTF16(u8i3), CastException);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u8i3).compare(u16expect3), 0);

    BOOST_CHECK_THROW(stringToUTF32(u8i3), CastException);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u8i3).compare(u32expect3), 0);

    // Illegal UTF8 code (6 bytes)
    UTF8 u8i4[7] = {0xFD, 0x40, 0x00, 0x00, 0x00, 0};
    UTF16 u16expect4[2] = {0x40, 0};
    UTF32 u32expect4[2] = {0x40, 0};
    u8string u8stri4(u8i4);

    BOOST_CHECK_THROW(stringToUTF16(u8i4), CastException);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u8i4).compare(u16expect4), 0);

    BOOST_CHECK_THROW(stringToUTF32(u8i4), CastException);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u8i4).compare(u32expect4), 0);

    // UTF16 early termination
    UTF16 u16i5[2] = {0xDBFF, 0};

    BOOST_CHECK_THROW(stringToUTF8(u16i5), CastException);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u16i5).compare(u8null), 0);

    BOOST_CHECK_THROW(stringToUTF32(u16i5), CastException);
    BOOST_CHECK_EQUAL(stringToUTF32NoThrow(u16i5).compare(u32null), 0);

    // Quasi-Illegal UTF16 codes are generally accepted (I couldn't find
    // specific examples that trigger an exception other than early terminations).

    // Illegal UTF32 code (1 character)
    UTF32 u16i6[2] = {0x110000, 0};

    BOOST_CHECK_THROW(stringToUTF8(u16i6), CastException);
    BOOST_CHECK_EQUAL(stringToUTF8NoThrow(u16i6).compare(u8null), 0);

    BOOST_CHECK_THROW(stringToUTF16(u16i6), CastException);
    BOOST_CHECK_EQUAL(stringToUTF16NoThrow(u16i6).compare(u16null), 0);
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
 * Checks that strstarter correctly produces strings from arbitrary data.
 */
BOOST_AUTO_TEST_CASE(stringStream) {
    BOOST_CHECK_EQUAL((strstarter << "char star")->
            buildString().compare("char star"), 0);
    BOOST_CHECK_EQUAL((strstarter << "char star"
            << strender).compare("char star"), 0);

    BOOST_CHECK_EQUAL((strstarter << std::string("string"))->
            buildString().compare("string"), 0);
    BOOST_CHECK_EQUAL((strstarter << std::string("string")
            << strender).compare("string"), 0);

    BOOST_CHECK_EQUAL((strstarter << L"wide char star")->
            buildString().compare("wide char star"), 0);
    BOOST_CHECK_EQUAL((strstarter << L"wide char star"
            << strender).compare("wide char star"), 0);

    BOOST_CHECK_EQUAL((strstarter << std::wstring(L"wstring"))->
            buildString().compare("wstring"), 0);
    BOOST_CHECK_EQUAL((strstarter << std::wstring(L"wstring")
            << strender).compare("wstring"), 0);


    float delta = 0.00000001;
    BOOST_CHECK_EQUAL((strstarter << 15)->buildString().compare("15"), 0);
    BOOST_CHECK_EQUAL((strstarter << 15 << strender).compare("15"), 0);

    BOOST_CHECK_EQUAL((strstarter << (3.14 + delta))->
            buildString().substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL((strstarter << (3.14 + delta)
            << strender).substr(0, 4).compare("3.14"), 0);


    BOOST_CHECK_EQUAL((strstarter << "multiple" << " " << "char" << " " << "stars")->
            buildString().compare("multiple char stars"), 0);
    BOOST_CHECK_EQUAL((strstarter << "multiple" << " " << "char" << " " << "stars"
            << strender).compare("multiple char stars"), 0);

    BOOST_CHECK_EQUAL((strstarter << std::string("multiple") << std::string(" ")
            << std::string("strings"))->buildString().compare("multiple strings"), 0);
    BOOST_CHECK_EQUAL((strstarter << std::string("multiple") << std::string(" ")
            << std::string("strings") << strender).compare("multiple strings"), 0);

    BOOST_CHECK_EQUAL((strstarter << 'c' << 'h' << 'a' << 'r' << 's')->
                buildString().compare("chars"), 0);
    BOOST_CHECK_EQUAL((strstarter << 'c' << 'h' << 'a' << 'r' << 's'
                << strender).compare("chars"), 0);


    BOOST_CHECK_EQUAL((strstarter << "multiple" << ' ' << "(more than " << 2 << ")"
            << ' ' << std::wstring(L"string") << '-' << "types" << ' '
            << std::string("added") << ' ' << "together")->buildString().compare(
                    "multiple (more than 2) string-types added together"), 0);
    BOOST_CHECK_EQUAL((strstarter << "multiple" << ' ' << "(more than " << 2 << ")"
            << ' ' << std::wstring(L"string") << '-' << "types" << ' '
            << std::string("added") << ' ' << "together" << strender).compare(
                    "multiple (more than 2) string-types added together"), 0);
}

std::string stringName = "Stringable";
class Stringable {
public:
    std::string toString() const {
        return stringName;
    }
};

class StringableInherit : public Stringable {};

/*
 * Checks that any class with the toString method is callable with toString function.
 */
BOOST_AUTO_TEST_CASE(toStringClass) {
    Stringable stringy;
    StringableInherit inherit;
    bool hasToStr = detail::HasMemberSigFunc_toString_strcheck<Stringable>::value;
    BOOST_CHECK(hasToStr);
    hasToStr = detail::HasMemberSigFunc_toString_strcheck<StringableInherit>::value;
    BOOST_CHECK(hasToStr);
    BOOST_CHECK_EQUAL(toString(stringy).compare(stringName), 0);
    BOOST_CHECK_EQUAL(toString(inherit).compare(stringName), 0);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_STRING_UTIL_HPP_ */
