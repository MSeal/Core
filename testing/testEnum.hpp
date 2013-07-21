/*
 * testEnum.hpp
 * Tests the syntax for creating bi-directionally mapped enumerations.
 */

#ifndef TEST_ENUM_HPP_
#define TEST_ENUM_HPP_

#include "enum.hpp"
#include <boost/bimap/multiset_of.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/logic/tribool.hpp>
// tribool doesn't have a less than operator, which breaks the bimap build.
namespace boost { namespace logic {
    inline tribool operator<(tribool x, tribool y) {
        if (y && (!x || (x == tribool::indeterminate_value))) {
            return tribool::true_value;
        } else if (x && (!y || (y == tribool::indeterminate_value))) {
            return tribool::false_value;
        } else {
            return tribool::indeterminate_value;
        }
    }
}}

namespace core {

enum ResponseEnumTest {
    YES_TEST       = 0,
    NO_TEST        = 1,
    MAYBE_TEST     = 2
};


/* Float */
const float YES_TEST_FLOAT = 1.0;
const float NO_TEST_FLOAT = 0.0;
const float MAYBE_TEST_FLOAT = 0.5;

typedef const boost::bimap<ResponseEnumTest, float> ResponseTestFloatMap;

// Forward declaration
template<> inline ResponseTestFloatMap& enumMappings<ResponseEnumTest, float>();


ResponseTestFloatMap RESPONSE_TEST_FLOAT_BIMAP =
        BI_MAP_BUILDER(ResponseEnumTest, float)
        (YES_TEST, YES_TEST_FLOAT)
        (NO_TEST, NO_TEST_FLOAT)
        (MAYBE_TEST, MAYBE_TEST_FLOAT);

template<>
inline ResponseTestFloatMap& enumMappings<ResponseEnumTest, float>() {
    return RESPONSE_TEST_FLOAT_BIMAP;
}


/* Bool */
typedef const boost::bimap<ResponseEnumTest, boost::bimaps::multiset_of<bool> > ResponseTestBoolMap;

// Forward declaration
template<> inline ResponseTestBoolMap& enumMappings<ResponseEnumTest, boost::bimaps::multiset_of<bool> >();

ResponseTestBoolMap RESPONSE_TEST_BOOL_BIMAP =
        BI_MAP_BUILDER(ResponseEnumTest, boost::bimaps::multiset_of<bool>)
        (YES_TEST, true)
        (NO_TEST, false)
        (MAYBE_TEST, false);

template<>
inline ResponseTestBoolMap& enumMappings<ResponseEnumTest, boost::bimaps::multiset_of<bool> >() {
    return RESPONSE_TEST_BOOL_BIMAP;
}

// Need to redefine enumToValue and valueToEnum to be Multiset aware
template<>
const bool& enumToValue<bool>(ResponseEnumTest eval) {
    return detail::enumToValueMultiVal<ResponseEnumTest, bool>(eval);
}
template<>
ResponseEnumTest valueToEnum<ResponseEnumTest>(const bool& val) {
    // Can't map values into Enums with multiset
    return val ? YES_TEST : NO_TEST;
}


/* String */
const std::string YES_TEST_STRING = "Yes";
const std::string NO_TEST_STRING = "No";
const std::string MAYBE_TEST_STRING = "Maybe";

typedef const boost::bimap<ResponseEnumTest, std::string> ResponseTestStringMap;

// Forward declaration
template<> inline ResponseTestStringMap& enumMappings<ResponseEnumTest, std::string>();

ResponseTestStringMap RESPONSE_TEST_STRING_BIMAP =
        BI_MAP_BUILDER(ResponseEnumTest, std::string)
        (YES_TEST, YES_TEST_STRING)
        (NO_TEST, NO_TEST_STRING)
        (MAYBE_TEST, MAYBE_TEST_STRING);

template<>
inline ResponseTestStringMap& enumMappings<ResponseEnumTest, std::string>() {
    return RESPONSE_TEST_STRING_BIMAP;
}

/* Tribool */
typedef const boost::bimap<ResponseEnumTest, boost::tribool> ResponseTestTriboolMap;

// Forward declaration
template<> inline ResponseTestTriboolMap& enumMappings<ResponseEnumTest, boost::tribool>();

ResponseTestTriboolMap RESPONSE_TEST_TRIBOOL_BIMAP =
        BI_MAP_BUILDER(ResponseEnumTest, boost::logic::tribool)
        (YES_TEST, boost::logic::tribool::true_value)
        (NO_TEST, boost::logic::tribool::false_value)
        (MAYBE_TEST, boost::logic::tribool::indeterminate_value);

template<>
inline ResponseTestTriboolMap& enumMappings<ResponseEnumTest, boost::tribool>() {
    return RESPONSE_TEST_TRIBOOL_BIMAP;
}

BOOST_AUTO_TEST_SUITE(EnumTests)

BOOST_AUTO_TEST_CASE(floatResponseEnum) {
    BOOST_CHECK_EQUAL(enumToValue<float>(YES_TEST), YES_TEST_FLOAT);
    BOOST_CHECK_EQUAL(enumToValue<float>(NO_TEST), NO_TEST_FLOAT);
    BOOST_CHECK_EQUAL(enumToValue<float>(MAYBE_TEST), MAYBE_TEST_FLOAT);

    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(YES_TEST_FLOAT), YES_TEST);
    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(NO_TEST_FLOAT), NO_TEST);
    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(MAYBE_TEST_FLOAT), MAYBE_TEST);
}

BOOST_AUTO_TEST_CASE(boolResponseEnum) {
    BOOST_CHECK_EQUAL(enumToValue<bool>(YES_TEST), true);
    BOOST_CHECK_EQUAL(enumToValue<bool>(NO_TEST), false);
    BOOST_CHECK_EQUAL(enumToValue<bool>(MAYBE_TEST), false);

    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(true), YES_TEST);
    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(false), NO_TEST);
}

//TODO finish test, use typedef::left/right, update enum.hpp comment

BOOST_AUTO_TEST_SUITE_END()
}
#endif /* TEST_ENUM_HPP_ */
