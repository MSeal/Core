/*
 * enum.h
 *
 * This file defines some wrappers on bimap relations that
 * allow for more automated mappings of enumerations to other
 * types.
 *
 * The mappings work through a few templated function calls.
 * In many cases enumMappings can be defined by itself and serve
 * in the default definitions of enumToValue and valueToEnum.
 * The function simply defines the retrieval of the bimap between
 * the given enumeration and some ValType. The ValType will need
 * to have a defined less than operator.
 *
 * In certain cases the enumToValue and/or valueToEnum need to be
 * defined to avoid issues with bimap relations. Cases that cause
 * this are when ValType doesn't define a less than operator, when
 * there is a one-to-many relationship between the enumeration and
 * ValType, or when a pre-existing mapping function already exists.
 * In these cases you can define the function to circumvent the
 * bimap relation and use other functions to perform mapping.
 *
 * If there exists a one-to-many relationship between the
 * enumeration and ValType, you can use
 * detail::enumToValueMultiVal<EnumType, ValType>(eval)
 * to define the enumToValue function.
 */
#ifndef ENUM_HPP_
#define ENUM_HPP_

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/assign/list_of.hpp>

namespace core {

template <typename EnumType, typename ValType>
const boost::bimap<EnumType, ValType>& enumMappings();

namespace detail {
    template <typename EnumType, typename ValType, typename MapType>
    const ValType& enumToValueBase(const MapType& map, EnumType eval) {
        return map.left.at(eval);
    }
    template <typename EnumType, typename ValType, typename MapType>
    EnumType valueToEnumBase(const MapType& map, const ValType& val) {
        return map.right.at(val);
    }

    template <typename EnumType, typename ValType>
    const ValType& enumToValueDefault(EnumType eval) {
        return enumToValueBase<EnumType, ValType, boost::bimap<EnumType, ValType> >
                (enumMappings<EnumType, ValType>(), eval);
    }
    template <typename EnumType, typename ValType>
    EnumType valueToEnumDefault(const ValType& val) {
        return valueToEnumBase<EnumType, ValType, boost::bimap<EnumType, ValType> >
                (enumMappings<EnumType, ValType>(), val);
    }

    template <typename EnumType, typename ValType>
    const ValType& enumToValueMultiVal(EnumType eval) {
        return enumToValueBase<EnumType, ValType, boost::bimap<EnumType, boost::bimaps::multiset_of<ValType> > >
                (enumMappings<EnumType, boost::bimaps::multiset_of<ValType> >(), eval);
    }
}

template <typename ValType, typename EnumType>
const ValType& enumToValue(EnumType eval) {
    return detail::enumToValueDefault<EnumType, ValType>(eval);
}
template <typename EnumType, typename ValType>
EnumType valueToEnum(const ValType& val) {
    return detail::valueToEnumDefault<EnumType, ValType>(val);
}

#define LIST_BUILDER boost::assign::list_of

/*
 * Constructs a boost::bimap<ENUM_TYPE, VAL_TYPE> by the following means:
 *
 * const boost::bimap<MyEnum, MyValue> myEnumMap =
 *    BI_MAP_BUILDER(MyEnum, MyValue)
 *      (enum1, value1)
 *      (enum2, value2)
 *      ...
 *      (enumN, valueN);
 *
 * Don't forget to use core namespacing.
 * Also ensure that MyValue has a < operator.
 */
#define BI_MAP_BUILDER(ENUM_TYPE, VAL_TYPE)                     \
        LIST_BUILDER<boost::bimap<ENUM_TYPE, VAL_TYPE>::relation>

}
#endif /* ENUM_HPP_ */
