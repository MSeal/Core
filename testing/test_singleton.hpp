/*
 * testSingleton.hpp
 * Tests the singleton abstract class capabilities.
 */

#ifndef TEST_SINGLETON_HPP_
#define TEST_SINGLETON_HPP_

#include "singleton.hpp"

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/test/unit_test.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {
BOOST_AUTO_TEST_SUITE(SingletonTests)

class SingletonTest {
public:
    int value;
    SingletonTest() : value(0) {}
    ~SingletonTest() {}
};

/*
 * Tests creation and use of singletons.
 */
BOOST_AUTO_TEST_CASE(singletonRequests) {
    BOOST_CHECK_EQUAL(Singleton<SingletonTest>::instance().value, 0);
    Singleton<SingletonTest>::instance().value = 42;
    BOOST_CHECK_EQUAL(Singleton<SingletonTest>::instance().value, 42);

    Singleton<SingletonTest>::reset();
    BOOST_CHECK_EQUAL(Singleton<SingletonTest>::instance().value, 0);
    Singleton<SingletonTest>::instance().value++;
    BOOST_CHECK_EQUAL(Singleton<SingletonTest>::instance().value, 1);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_SINGLETON_HPP_ */
