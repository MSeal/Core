/*
 * testLoops.hpp
 *
 *  Created on: Jan 14, 2013
 *      Author: Caitlin
 */

#ifndef TESTLOOPS_HPP_
#define TESTLOOPS_HPP_

#include "loops.hpp"
#include <boost/test/unit_test.hpp>
#include <limits>

namespace core {
BOOST_AUTO_TEST_SUITE(Loops)

BOOST_AUTO_TEST_CASE(simpleLoop) {
    int i = 0;
    forRepeat(10) {
        i++;
    }
    BOOST_CHECK_EQUAL(i, 10);

    std::string str = "";
    forRepeat(20) {
        str += "a";
    }
    BOOST_CHECK_EQUAL(str.length(), 20);

    /*
     * Tests forRepeat for long. Uses unsigned long long int to fix bug
     * where some machines use int32 in place of long.
     */
    unsigned long long int itr = std::numeric_limits<unsigned int>::max();
    itr *= 5;
    unsigned long long int lng = 0;

    forRepeatLong(itr) {
        lng++;
    }
    BOOST_CHECK_EQUAL(lng, itr);

    int j;
    i = 0;
    forIter(j, 0, 10) {
        i++;
    }
    BOOST_CHECK_EQUAL(i, 10);

    reverseForIter(j, 10, 0) {
        i--;
    }
    BOOST_CHECK_EQUAL(i, 0);


}

BOOST_AUTO_TEST_CASE(enumerateEachLoop) {
    int count = 0;
    std::vector<int> vect;
    forRepeat(10) {
        vect.push_back(1);
    }

    enumerateEach(count, int i, vect) {
        i++;
        BOOST_CHECK_EQUAL(i, 2);
    }
    BOOST_CHECK_EQUAL(count, 10);

    std::vector<int> emptyVect;
    count = 0;

    enumerateEach(count, int j, emptyVect) {}
    BOOST_CHECK_EQUAL(count, 0);
}


BOOST_AUTO_TEST_SUITE_END()

}
#endif /* TESTLOOPS_HPP_ */
