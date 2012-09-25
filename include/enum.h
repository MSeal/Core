/*
 * enum.h
 *
 * TODO description
 */
#ifndef ENUM_H_
#define ENUM_H_

#include <boost/bimap.hpp>
#include <boost/assign/list_of.hpp>

namespace core {

template <typename EnumType, typename ValType>
const boost::bimap<EnumType, ValType>& enumMappings();
template <typename EnumType, typename ValType>
ValType enumToValue(EnumType eval) {
    return enumMappings<ValType, EnumType>().right.at(eval);
}
template <typename EnumType, typename ValType>
EnumType valueToEnum(ValType val) {
    return enumMappings<ValType, EnumType>().left.at(val);
}

// Used to trick the CDT Parser to accepting our syntax
#ifdef __CDT_PARSER__
struct FakeListBuilder {
    template<typename T, typename V> FakeListBuilder& operator()(T, V);
};
#define LIST_BUILDER FakeListBuilder
#else
#define LIST_BUILDER boost::assign::list_of
#endif


/*
 * Constructs a boost::bimap<ENUM_TYPE, VAL_TYPE> by the following means:
 *
 * const boost::bimap<MyEnum, MyValue> myEnumMap =
 *    BI_MAP_BUILDER(MyEnum, MyValue)
 *      (enum1, value1)
 *      (enum2, value2)
 *      ...
 *      (enumN, valueN);
 */
#define BI_MAP_BUILDER(ENUM_TYPE, VAL_TYPE)                     \
        LIST_BUILDER<boost::bimap<ENUM_TYPE, VAL_TYPE>::relation>

}
#endif /* ENUM_H_ */
