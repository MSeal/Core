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


/*
 * Bool
 *
 * We can define two mappings, one with multiset awareness to map many
 * enumerations to a few values, and another to map a few values one to
 * one with enumerations.
 */
typedef boost::bimaps::multiset_of<bool> MultiMapBool;
typedef const boost::bimap<ResponseEnumTest, MultiMapBool> ResponseTestMultiBoolMap;
typedef const boost::bimap<ResponseEnumTest, bool> ResponseTestBoolMap;

// Forward declarations
template<> inline ResponseTestMultiBoolMap& enumMappings<ResponseEnumTest, MultiMapBool>();
template<> inline ResponseTestBoolMap& enumMappings<ResponseEnumTest, bool>();

ResponseTestMultiBoolMap RESPONSE_TEST_BOOL_BIMAP =
        BI_MAP_BUILDER(ResponseEnumTest, MultiMapBool)
        (YES_TEST, true)
        (NO_TEST, false)
        (MAYBE_TEST, false);

ResponseTestBoolMap RESPONSE_TEST_BOOL_VALUE_BIMAP =
        BI_MAP_BUILDER(ResponseEnumTest, bool)
        (YES_TEST, true)
        (NO_TEST, false);

template<>
inline ResponseTestMultiBoolMap& enumMappings<ResponseEnumTest, MultiMapBool>() {
    return RESPONSE_TEST_BOOL_BIMAP;
}
template<>
inline ResponseTestBoolMap& enumMappings<ResponseEnumTest, bool>() {
    return RESPONSE_TEST_BOOL_VALUE_BIMAP;
}

// Instead we could use this convention and circumvent bimaps
//const boost::tribool BOOL_TEST_TRUE = true;
//const boost::tribool BOOL_TEST_FALSE = false;
//template<>
//const bool& enumToValue<bool>(ResponseEnumTest eval) {
//    switch (eval) {
//        case YES_TEST:
//            return true;
//        case NO_TEST:
//        case MAYBE_TEST:
//        default:
//            return false;
//    }
//}
//template<>
//ResponseEnumTest valueToEnum<ResponseEnumTest>(const bool& val) {
//    return val? YES_TEST : NO_TEST;
//}

// Need to redefine enumToValue to be Multiset aware
// (making use of detail::enumToValueMultiVal)
template<>
const bool& enumToValue<bool>(ResponseEnumTest eval) {
    return detail::enumToValueMultiVal<ResponseEnumTest, bool>(eval);
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

/*
 * Tribool
 *
 * Unfortunately, tribool doesn't have a less than operator, and
 * even adding one still confuses the bimapping because of bool casting.
 * So instead the enumToValue and valueToEnum can be directly overwritten
 * to avoid using a bimap.
 */
// Here's how you would normally define the relationship
//typedef const boost::bimap<ResponseEnumTest, boost::logic::tribool> ResponseTestTriboolMap;
//
//// Forward declaration
//template<> inline ResponseTestTriboolMap& enumMappings<ResponseEnumTest, boost::logic::tribool>();
//
//ResponseTestTriboolMap RESPONSE_TEST_TRIBOOL_BIMAP =
//        BI_MAP_BUILDER(ResponseEnumTest, boost::logic::tribool)
//        (YES_TEST, boost::logic::tribool::true_value)
//        (NO_TEST, boost::logic::tribool::false_value)
//        (MAYBE_TEST, boost::logic::tribool::indeterminate_value);
//
//template<>
//inline ResponseTestTriboolMap& enumMappings<ResponseEnumTest, boost::logic::tribool>() {
//    return RESPONSE_TEST_TRIBOOL_BIMAP;
//}

// Instead we use this convention and circumvent bimaps
#ifdef _MSC_VER
    // MSVC warns about truncating the indeterminate_value assignment
    #pragma warning(disable:4305)
#endif
const boost::tribool TRIBOOL_TEST_TRUE = boost::tribool::true_value;
const boost::tribool TRIBOOL_TEST_FALSE = boost::tribool::false_value;
const boost::tribool TRIBOOL_TEST_INDETERM = boost::tribool::indeterminate_value;
#ifdef _MSC_VER
    #pragma warning(default:4305)
#endif
template<>
const boost::tribool& enumToValue<boost::tribool>(ResponseEnumTest eval) {
    switch (eval) {
        case YES_TEST:
            return TRIBOOL_TEST_TRUE;
        case NO_TEST:
            return TRIBOOL_TEST_FALSE;
        case MAYBE_TEST:
        default:
            return TRIBOOL_TEST_INDETERM;
    }
}
template<>
ResponseEnumTest valueToEnum<ResponseEnumTest>(const boost::tribool& val) {
    if (val) {
        return YES_TEST;
    } else if (!val) {
        return NO_TEST;
    } else {
        return MAYBE_TEST;
    }
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

BOOST_AUTO_TEST_CASE(stringResponseEnum) {
    BOOST_CHECK_EQUAL(enumToValue<std::string>(YES_TEST), YES_TEST_STRING);
    BOOST_CHECK_EQUAL(enumToValue<std::string>(NO_TEST), NO_TEST_STRING);
    BOOST_CHECK_EQUAL(enumToValue<std::string>(MAYBE_TEST), MAYBE_TEST_STRING);

    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(YES_TEST_STRING), YES_TEST);
    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(NO_TEST_STRING), NO_TEST);
    BOOST_CHECK_EQUAL(valueToEnum<ResponseEnumTest>(MAYBE_TEST_STRING), MAYBE_TEST);
}


BOOST_AUTO_TEST_CASE(triboolResponseEnum) {
    BOOST_CHECK_EQUAL(enumToValue<boost::logic::tribool>(YES_TEST), true);
    BOOST_CHECK_EQUAL(enumToValue<boost::logic::tribool>(NO_TEST), false);
    boost::logic::tribool triresult = enumToValue<boost::logic::tribool>(MAYBE_TEST);
    BOOST_CHECK_EQUAL(triresult, boost::logic::tribool::indeterminate_value);

    // Need to assign this separately to avoid macro expansion
    ResponseEnumTest result = valueToEnum<ResponseEnumTest, boost::logic::tribool>(true);
    BOOST_CHECK_EQUAL(result, YES_TEST);
    result = valueToEnum<ResponseEnumTest, boost::logic::tribool>(false);
    BOOST_CHECK_EQUAL(result, NO_TEST);
    result = valueToEnum<ResponseEnumTest, boost::logic::tribool>(boost::indeterminate);
    BOOST_CHECK_EQUAL(result, MAYBE_TEST);
}

//TODO update enum.hpp comment

BOOST_AUTO_TEST_SUITE_END()
}
#endif /* TEST_ENUM_HPP_ */
