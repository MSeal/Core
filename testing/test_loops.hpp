/*
 * testLoops.hpp
 * Tests the capability to iterate over a variety of custom loops.
 */

#ifndef TEST_LOOPS_HPP_
#define TEST_LOOPS_HPP_

#include "loops.hpp"
#include "ppcomp.hpp"
#include <limits>

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {
BOOST_AUTO_TEST_SUITE(LoopTests)

BOOST_AUTO_TEST_CASE(repeatLoops) {
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
    
    uint64_t lng = 0;
    forRepeatLong((uint64_t)20) {
        lng++;
    }
    BOOST_CHECK_EQUAL(lng, (uint64_t)20);

    // This test takes a really long time to run
    // Uses unsigned long long int to fix issue of int32 in place of long.
    // uint64_t itr = std::numeric_limits<uint32_t>::max();
    // itr *= 5;
    // uint64_t lng = 0;
    // forRepeatLong(itr) {
    //     lng++;
    // }
    // BOOST_CHECK_EQUAL(lng, itr);
}

BOOST_AUTO_TEST_CASE(forIterLoops) {
    int j;
    int i = 0;
    forIter(j, 0, 10) {
        i++;
    }
    BOOST_CHECK_EQUAL(i, 10);

    reverseForIter(j, 10, 0) {
        i--;
    }
    BOOST_CHECK_EQUAL(i, 0);
}

BOOST_AUTO_TEST_CASE(forEachLoops) {
    std::vector<int> vect;
    for(int i = 0; i < 10; i++) {
        vect.push_back(i);
    }

    int external = -1;
    int j;
    forEach(j, vect) {
        external++;
        BOOST_CHECK_EQUAL(j, external);
    }

    // Test reverse
    reverseForEach(j, vect) {
        BOOST_CHECK_EQUAL(j, external);
        external--;
    }
}

BOOST_AUTO_TEST_CASE(enumerateEachLoops) {
    int count = 0;
    std::vector<int> emptyvec;
    // Test empty
    enumerateEach(count, IGNORE_UNUSED_WARNING(int uncalled), emptyvec) {}
    BOOST_CHECK_EQUAL(count, 0);

    std::vector<int> vect;
    for(int i = 0; i < 10; i++) {
        vect.push_back(i);
    }

    int external = -1;
    int j;
    enumerateEach(count, j, vect) {
        external++;
        BOOST_CHECK_EQUAL(j, external);
        BOOST_CHECK_EQUAL(count, j);
    }

    // Test reverse
    int extcount = 0;
    reverseEnumerateEach(count, j, vect) {
        BOOST_CHECK_EQUAL(j, external);
        BOOST_CHECK_EQUAL(count, extcount);
        external--;
        extcount++;
    }
}

BOOST_AUTO_TEST_CASE(enumerateEachMacroLoops) {
    std::vector<int> macroVect;
    for(int i = 0; i < 10; i++) {
        macroVect.push_back(i);
    }

    int count;
    int external = -1;
    enumerateEach(count, int j, macroVect) {
        external++;
        BOOST_CHECK_EQUAL(j, external);
        BOOST_CHECK_EQUAL(count, j);
    }
}

void sumRef(int *total, int& i) {
    (*total) += i;
}

void sum(int *total, int i) {
    (*total) += i;
}

void noOp(int i) {}
void noOpRef(int& i) {}

BOOST_AUTO_TEST_CASE(onEachNoRefLoops) {
    int total = 0, check;
    boost::function1<void, int> callback(boost::bind(&sum, &total, _1));

    std::vector<int> vect;
    for(int i = 0; i < 10; i++) {
        vect.push_back(i);
    }

    onEach(check, callback, vect);
    BOOST_CHECK_EQUAL(check, 9);
    BOOST_CHECK_EQUAL(total, 45);
    total = 0;
    reverseOnEach(check, callback, vect);
    BOOST_CHECK_EQUAL(check, 0);
    BOOST_CHECK_EQUAL(total, 45);
    onEach(check, &noOp, vect);
    reverseOnEach(check, &noOp, vect);

}

BOOST_AUTO_TEST_CASE(onEachRefFuncLoops) {
    int total = 0, check;
    boost::function1<void, int&> callbackRef(boost::bind(&sumRef, &total, _1));

    std::vector<int> vect;
    for(int i = 0; i < 10; i++) {
        vect.push_back(i);
    }

    onEach(check, callbackRef, vect);
    BOOST_CHECK_EQUAL(check, 9);
    BOOST_CHECK_EQUAL(total, 45);
    total = 0;
    reverseOnEach(check, callbackRef, vect);
    BOOST_CHECK_EQUAL(check, 0);
    BOOST_CHECK_EQUAL(total, 45);
    onEach(check, &noOpRef, vect);
    reverseOnEach(check, &noOpRef, vect);
}

BOOST_AUTO_TEST_CASE(onEachNoItemLoops) {
    int total = 0;
    boost::function1<void, int> callback(boost::bind(&sum, &total, _1));
    boost::function1<void, int&> callbackRef(boost::bind(&sumRef, &total, _1));

    std::vector<int> vect;
    for(int i = 0; i < 10; i++) {
        vect.push_back(2);
    }

    total = 0;
    onEach(callback, vect);
    BOOST_CHECK_EQUAL(total, 20);

    total = 0;
    reverseOnEach(callback, vect);
    BOOST_CHECK_EQUAL(total, 20);
    onEach(&noOp, vect);
    reverseOnEach(&noOp, vect);

    total = 0;
    onEach(callbackRef, vect);
    BOOST_CHECK_EQUAL(total, 20);
    total = 0;
    reverseOnEach(callbackRef, vect);
    BOOST_CHECK_EQUAL(total, 20);
    onEach(&noOpRef, vect);
    reverseOnEach(&noOpRef, vect);
}

BOOST_AUTO_TEST_SUITE_END()
}
#endif /* TEST_LOOPS_HPP_ */
