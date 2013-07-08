/*
 * testTime.hpp
 * Tests that time specific operators return expected values.
 */

#ifndef TEST_TIME_HPP_
#define TEST_TIME_HPP_

#include "time.hpp"
#include <boost/thread/thread_time.hpp>
#include <boost/test/unit_test.hpp>

namespace core {
BOOST_AUTO_TEST_SUITE(TimeTests)

/*
 * Tests requesting future time.
 */
BOOST_AUTO_TEST_CASE(futureTime) {
    const boost::system_time future = getFuture(1500);
    const boost::system_time now = boost::get_system_time();

    BOOST_CHECK(future - boost::posix_time::milliseconds(1400) > now);
    BOOST_CHECK(future - boost::posix_time::milliseconds(1600) < now);
}

/*
 * Tests requesting past time.
 */
BOOST_AUTO_TEST_CASE(pastTime) {
    const boost::system_time past = getPast(1500);
    const boost::system_time now = boost::get_system_time();

    BOOST_CHECK(past + boost::posix_time::milliseconds(1400) < now);
    BOOST_CHECK(past + boost::posix_time::milliseconds(1600) > now);
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_TIME_HPP_ */
