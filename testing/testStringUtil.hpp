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
 * Checks a few values to find their floor/ceil power of 2 value
 */
BOOST_AUTO_TEST_CASE(stringStarter) {
    BOOST_CHECK_EQUAL((strstart << "char star").compare("char star"), 0);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_STRING_UTIL_HPP_ */
