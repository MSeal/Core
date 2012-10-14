/*
 * Tests the correctness of various numutil functions.
 */

#ifndef TEST_NUM_UTIL_H_
#define TEST_NUM_UTIL_H_

#include "numutil.hpp"
#include <boost/test/unit_test.hpp>

namespace core {
BOOST_AUTO_TEST_SUITE(NumberUtility)

/*
 * Checks a few values to find their floor/ceil power of 2 value
 */
BOOST_AUTO_TEST_CASE(powerOfTwo) {
    BOOST_CHECK_EQUAL(floorToPowerOfTwo(0), 1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(0), 1);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(1), 1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(1), 1);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(2), 2);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(2), 2);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(3), 2);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(3), 4);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(20), 16);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(20), 32);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(0.1), (unsigned long)1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(0.1), (unsigned long)1);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(0.6), (unsigned long)1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(0.6), (unsigned long)1);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(1.1), (unsigned long)1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(1.1), (unsigned long)2);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(1.6), (unsigned long)1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(1.6), (unsigned long)2);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(2.9), (unsigned long)2);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(2.9), (unsigned long)4);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(312.4), (unsigned long)256);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(312.4), (unsigned long)512);

    // Negative inputs always map to 1
    BOOST_CHECK_EQUAL(floorToPowerOfTwo(-1), 1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(-1), 1);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(-250), 1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(-250), 1);

    BOOST_CHECK_EQUAL(floorToPowerOfTwo(-31.2), (unsigned long)1);
    BOOST_CHECK_EQUAL(ceilToPowerOfTwo(-31.2), (unsigned long)1);
}

/*
 * Tests the various rounding functions
 */
BOOST_AUTO_TEST_CASE(roundTypes) {
    BOOST_CHECK_CLOSE(roundValue(0.1), 0.0, 0.0001);
    BOOST_CHECK_EQUAL(roundToInt(0.1), 0);
    BOOST_CHECK_EQUAL(roundToLong(0.1), 0);
#ifdef BOOST_HAS_LONG_LONG
    BOOST_CHECK_EQUAL(roundToLongLong(-0.1), 0);
#endif

    BOOST_CHECK_CLOSE(roundValue(0.6), 1.0, 0.0001);
    BOOST_CHECK_EQUAL(roundToInt(0.6), 1);
    BOOST_CHECK_EQUAL(roundToLong(0.6), 1);
#ifdef BOOST_HAS_LONG_LONG
    BOOST_CHECK_EQUAL(roundToLongLong(-0.1), 0);
#endif

    BOOST_CHECK_CLOSE(roundValue(0.4999F), 0.0, 0.0001);
    BOOST_CHECK_EQUAL(roundToInt(0.4999F), 0);
    BOOST_CHECK_EQUAL(roundToLong(0.4999F), 0);
#ifdef BOOST_HAS_LONG_LONG
    BOOST_CHECK_EQUAL(roundToLongLong(-0.1), 0);
#endif

    BOOST_CHECK_CLOSE(roundValue(-0.1), 0.0, 0.0001);
    BOOST_CHECK_EQUAL(roundToInt(-0.1), 0);
    BOOST_CHECK_EQUAL(roundToLong(-0.1), 0);
#ifdef BOOST_HAS_LONG_LONG
    BOOST_CHECK_EQUAL(roundToLongLong(-0.1), 0);
#endif

    BOOST_CHECK_CLOSE(roundValue(-0.6), -1.0, 0.0001);
    BOOST_CHECK_EQUAL(roundToInt(-0.6), -1);
    BOOST_CHECK_EQUAL(roundToLong(-0.6), -1);
#ifdef BOOST_HAS_LONG_LONG
    BOOST_CHECK_EQUAL(roundToLongLong(-0.6), -1);
#endif

    BOOST_CHECK_CLOSE(roundValue(3.14F), 3.0F, 0.0001F);
    BOOST_CHECK_EQUAL(roundToInt(3.14F), 3);
    BOOST_CHECK_EQUAL(roundToLong(3.14F), 3);
#ifdef BOOST_HAS_LONG_LONG
    BOOST_CHECK_EQUAL(roundToLongLong(3.14F), 3);
#endif

    // Check very large number rounding
    // This will not round nicely as the floating point
    // error is larger than 1.0
    BOOST_CHECK_CLOSE(round(3.14e50 + 8.65e40), 3.14e50, 0.0001);
    if (sizeof(int) >= 8) {
        BOOST_CHECK_EQUAL(roundToInt(3.14e17 + 0.3543), static_cast<int>(3.14e17));
    }
    if (sizeof(long) >= 8) {
        BOOST_CHECK_EQUAL(roundToLong(3.14e17 + 0.3543), static_cast<long>(3.14e17));
    }
#ifdef BOOST_HAS_LONG_LONG
    if (sizeof(long long) >= 8) {
        BOOST_CHECK_EQUAL(roundToLongLong(3.14e17 + 0.3543), static_cast<long long>(3.14e17));
    }
#endif
}


/* Trys to limitVal some values */
BOOST_AUTO_TEST_CASE(constrain) {
    int test = 100;
    BOOST_CHECK_EQUAL(constrained(test, 1, 20), 20);

    test = -100;
    BOOST_CHECK_EQUAL(constrained(test, -20, -1), -20);

    test = 10;
    BOOST_CHECK_EQUAL(constrained(test, -10, 20), 10);

    test = 10;
    BOOST_CHECK_EQUAL(constrained(test, 10, 10), 10);

    test = -20;
    BOOST_CHECK_EQUAL(constrained(test, -10, 20), -10);

    test = 200;
    BOOST_CHECK_EQUAL(constrained(test, -10, 20), 20);

    test = 200;
    BOOST_CHECK_EQUAL(constrained(test, -10, -5), -5);

    test = 0;
    BOOST_CHECK_EQUAL(constrained(test, 5, 10), 5);

    double testD = 0.1;
    BOOST_CHECK_CLOSE(constrained(testD, 5.1, 10.28), 5.1, 0.0001);

    float testF = -2.2F;
    BOOST_CHECK_CLOSE(constrained(testF, 5.1F, 10.28F), 5.1F, 0.0001);

    testD = 210.7;
    BOOST_CHECK_CLOSE(constrained(testD, 10.2, 5.9), 5.9, 0.0001);

    testF = 210.7;
    BOOST_CHECK_CLOSE(constrained(testF, 10.2F, 5.9F), 5.9F, 0.0001);

    testD = -210.7;
    BOOST_CHECK_CLOSE(constrained(testD, -10.2, -5.9), -10.2, 0.0001);

    testF = -210.7;
    BOOST_CHECK_CLOSE(constrained(testF, -10.2F, -5.9F), -10.2F, 0.0001);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_NUM_UTIL_H_ */
