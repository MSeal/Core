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

BOOST_AUTO_TEST_CASE(stringToString) {
    BOOST_CHECK_EQUAL(toString("char star").compare("char star"), 0);
    BOOST_CHECK_EQUAL(toString(std::string("string")).compare("string"), 0);
    BOOST_CHECK_EQUAL(toString(std::wstring(L"wide string")).compare("wide string"), 0);
}

BOOST_AUTO_TEST_CASE(numToString) {
    float delta = 0.00000001;
    BOOST_CHECK_EQUAL(toString(15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((unsigned int)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((short)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((unsigned short)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((long)15).compare("15"), 0);
    BOOST_CHECK_EQUAL(toString((unsigned long)15).compare("15"), 0);

    BOOST_CHECK_EQUAL(toString(-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toString((short)-14).compare("-14"), 0);
    BOOST_CHECK_EQUAL(toString((long)-14).compare("-14"), 0);

    BOOST_CHECK_EQUAL(toString(3.14 + delta).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toString(float(3.14 + delta)).substr(0, 4).compare("3.14"), 0);
    BOOST_CHECK_EQUAL(toString(double(3.14 + delta)).substr(0, 4).compare("3.14"), 0);

    BOOST_CHECK_EQUAL(toString(-1.23 - delta).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toString(float(-1.23 - delta)).substr(0, 5).compare("-1.23"), 0);
    BOOST_CHECK_EQUAL(toString(double(-1.23 - delta)).substr(0, 5).compare("-1.23"), 0);
}

/*
 * Checks that stringstart correctly produces strings from arbitrary data.
 */
BOOST_AUTO_TEST_CASE(stringStarter) {
    BOOST_CHECK_EQUAL((strstart << "char star").compare("char star"), 0);
    BOOST_CHECK_EQUAL((strstart << std::string("char star")).compare("char star"), 0);

    BOOST_CHECK_EQUAL((strstart << "multiple" << ' ' << "string" << '-' << "types" << ' '
            << std::string("added") << ' ' << "together").compare(
                    "multiple string-types added together"), 0);

    float delta = 0.00000001;
    BOOST_CHECK_EQUAL((strstart << 15).compare("15"), 0);
    BOOST_CHECK_EQUAL((strstart << 3.14 + delta).substr(0, 4).compare("3.14"), 0);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_STRING_UTIL_HPP_ */
