/*
 * pptypes.hpp
 *
 * Based on constructions and designs from the following sources. Rebuilt entirely
 * to be more general, C++98 compatible, and repeatedly usable.
 *
 * http://stackoverflow.com/questions/1386183/
 *      how-to-call-a-templated-function-if-it-exists-and-something-else-otherwise
 * http://www.rsdn.ru/forum/cpp/2759773.1
 * http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
 * http://stackoverflow.com/questions/257288/
 *      is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence/264088#264088
 *
 * Capabilities:
 * Can't tell difference between references and non-references
 */
#ifndef PREPROCESSOR_TYPES_H_
#define PREPROCESSOR_TYPES_H_

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/static_assert.hpp>
#include "detail/pptypeMemberCheck.hpp"

namespace core {

template<bool T, class R=void>
struct enableIf {
   typedef R type;
};

template<class R>
struct enableIf<false, R>{};

template<bool T, class R=void>
struct disableIf {
   typedef R type;
};

template<class R>
struct disableIf<true, R>{};

template<typename SrcType, typename DestType>
struct constnessCheck {
    typedef DestType type;
};
template<typename SrcType, typename DestType>
struct constnessCheck<const SrcType, DestType> {
    typedef const DestType type;
};

template<typename Type>
struct referenceStrip {
    typedef Type type;
};
template<typename Type>
struct referenceStrip<Type&> {
    typedef Type type;
};

/*
 * Fake definition of HasMember for CDT parser, since it thinks boost::is_class
 * doesn't exist.
 */
#ifdef __CDT_PARSER__
#define CREATE_MEMBER_CHECK(mname)                                                          \
template<typename Type>                                                                     \
class HasMember_##mname                                                                     \
{                                                                                           \
public:                                                                                     \
    static const bool value = false;                                                        \
    typedef typename boost::integral_constant<bool, value> type;                            \
}
#else
/*
 * Originally taken freely from http://www.rsdn.ru/forum/cpp/2759773.1
 * Edited heavily to become generic and for easier reading
 */
#define CREATE_MEMBER_CHECK(mname)                                                          \
template<typename Type, typename SFINAE = void>                                             \
/* Fallback class definition if Type is not a class */                                      \
class HasMember_##mname                                                                     \
{                                                                                           \
public:                                                                                     \
    /* Non-class types never have members */                                                \
    static const bool value = false;                                                        \
    /* Used for boost template logic */                                                     \
    typedef typename boost::integral_constant<bool, value> type;                            \
};                                                                                          \
                                                                                            \
/* Class definition for Types which are classes */                                          \
template<typename Type>                                                                     \
class HasMember_##mname <Type, typename ::core::enableIf<                                   \
    boost::is_class<Type>::value>::type>                                                    \
{                                                                                           \
    /* Use char and long sizes to indicate occurrence */                                    \
    typedef char Yes;                                                                       \
    typedef long No;                                                                        \
                                                                                            \
    struct Derived {                                                                        \
        void mname(){}                                                                      \
    };                                                                                      \
                                                                                            \
    struct Check : public Type, public Derived {};                                          \
                                                                                            \
    /* Only types which match the signature T can be placed in the second template */       \
    template<typename T, T> class MemberCheck{};                                            \
                                                                                            \
    template<typename U>                                                                    \
    static No resolve(U*, MemberCheck<void (Derived::*)(), &U::mname>* = NULL);             \
    static Yes resolve(...);                                                                \
                                                                                            \
public:                                                                                     \
    /* True if the class has the member */                                                  \
    static const bool value = (sizeof(Yes) == sizeof(resolve((Check*)(NULL))));             \
    /* Used for boost template logic */                                                     \
    typedef typename boost::integral_constant<bool, value> type;                            \
}
#endif


/*
 * Macros that are expanded upon in pptypeMemberCheck.hpp to generate
 * Checks for various number of arguments.
 */
#define MEMBER_CHECK_ARG_GEN(mname, targlist, carglist, farglist)                           \
template<targlist, typename R>                                                              \
struct Check<true, carglist> {                                                              \
    static const bool value =                                                               \
        sizeof(ReturnValueCheck<Type, R>::resolve(                                          \
                /* Fake comma operator for void return */                                   \
                (((DerivedType*)NULL)->mname(farglist),                                     \
                        hidden::voidResultHolder_##mname <Type>()))                         \
               ) == sizeof(Yes);                                                            \
};                                                                                          \
template <targlist, typename R>                                                             \
struct Check<true, carglist const> {                                                        \
    static const bool value =                                                               \
        sizeof(ReturnValueCheck<Type, R>::resolve(                                          \
                /* Fake comma operator for void return */                                   \
                (((DerivedConstMethodType*)NULL)->mname(farglist),                          \
                        hidden::voidResultHolder_##mname <Type>()))                         \
               ) == sizeof(Yes);                                                            \
}

/*
 * Fake definition of IsMemberFunc for CDT parser, since there is an ambiguity
 * for one of the templates on non-const checks.
 */
#ifdef __CDT_PARSER__
#define CREATE_MEMBER_FUNC_CHECK(mname)                                                     \
template<typename Type, typename CallDetails>                                                                     \
struct IsMemberFunc_##mname                                                                 \
{                                                                                           \
public:                                                                                     \
    static const bool value = false;                                                        \
    typedef typename boost::integral_constant<bool, value> type;                            \
}
#else
#define CREATE_MEMBER_FUNC_CHECK(mname)                                                     \
template<typename Type, typename CallDetails>                                               \
struct IsMemberFunc_##mname;                                                                \
                                                                                            \
namespace hidden {                                                                          \
namespace membercheck {                                                                     \
    /* Hide the member check function here to avoid name clobbering */                      \
    CREATE_MEMBER_CHECK(mname);                                                             \
}                                                                                           \
    template<typename Type>                                                                 \
    class voidResultHolder_##mname {};                                                      \
                                                                                            \
    /* Use comma operator because it has lowest precidence of C++ operators */              \
    /* This resolves bad return types like void blowing up template calls */                \
    template<typename Type, typename U>                                                     \
    U const& operator,(U const&, voidResultHolder_##mname <Type>);                          \
    /* Need const version as well */                                                        \
    template<typename Type, typename U>                                                     \
    U& operator,(U&, voidResultHolder_##mname <Type>);                                      \
}                                                                                           \
                                                                                            \
template<typename Type, typename CallDetails>                                               \
struct IsMemberFunc_##mname                                                                 \
{                                                                                           \
private:                                                                                    \
    /* Keep Yes and No as a struct to avoid ambiguity in inheritance */                     \
    struct Yes { char check; };                                                             \
    struct No { Yes notCheck[2]; };                                                         \
                                                                                            \
    /* Check the result of the expression */                                                \
    template<typename T, typename R>                                                        \
    struct ReturnValueCheck {                                                               \
        static Yes resolve(R);                                                              \
        static Yes resolve(Yes);                                                            \
        static No resolve(...);                                                             \
        static No resolve(No);                                                              \
    };                                                                                      \
                                                                                            \
    /* Fallback template typing for void returns */                                         \
    template<typename T>                                                                    \
    struct ReturnValueCheck<T, void> {                                                      \
        static Yes resolve(Yes);                                                            \
        static Yes resolve(...);                                                            \
        static No resolve(No);                                                              \
    };                                                                                      \
                                                                                            \
    /* Add an overloaded method */                                                          \
    struct Derived : public Type {                                                          \
        using Type::mname;                                                                  \
        /* Gives warning about ambiguity, can't keep behavior and lose warning... */        \
        No mname(...);                                                                      \
        No mname(...) const;                                                                \
    };                                                                                      \
    /* Add an overloaded method */                                                          \
    struct DerivedConst : public Type {                                                     \
        using Type::mname;                                                                  \
        No mname(...) const;                                                                \
    };                                                                                      \
                                                                                            \
    /* Keep constness of Type in Derived Type */                                            \
    typedef typename ::core::constnessCheck<Type, Derived>::type DerivedType;               \
    /* Force constant methods to have constant type */                                      \
    typedef typename ::core::constnessCheck                                                 \
        <const int, DerivedConst>::type DerivedConstMethodType;                             \
                                                                                            \
    template<bool has, typename F>                                                          \
    struct Check {                                                                          \
        static const bool value = false;                                                    \
    };                                                                                      \
                                                                                            \
    /* The no argument case can't be perfectly solved without C++11 */                      \
    /* I spent a good 3 days trying... :( */                                                \
    template<typename R>                                                                    \
    struct Check<true, R()> {                                                               \
        static const bool value = true;                                                     \
        BOOST_STATIC_ASSERT_MSG(sizeof(R) == 0,                                             \
                "Cannot check no-argument functions -- use HasMember instead");             \
    };                                                                                      \
    template<typename R>                                                                    \
    struct Check<true, R() const> {                                                         \
        static const bool value = true;                                                     \
        BOOST_STATIC_ASSERT_MSG(sizeof(R) == 0,                                             \
                "Cannot check no-argument functions -- use HasMember instead");             \
    };                                                                                      \
                                                                                            \
    /* Create checks for up to 15 arguments */                                              \
    MEMBER_CHECK_1_ARG(mname);                                                              \
    MEMBER_CHECK_2_ARG(mname);                                                              \
    MEMBER_CHECK_3_ARG(mname);                                                              \
    MEMBER_CHECK_4_ARG(mname);                                                              \
    MEMBER_CHECK_5_ARG(mname);                                                              \
    MEMBER_CHECK_6_ARG(mname);                                                              \
    MEMBER_CHECK_7_ARG(mname);                                                              \
    MEMBER_CHECK_8_ARG(mname);                                                              \
    MEMBER_CHECK_9_ARG(mname);                                                              \
    MEMBER_CHECK_10_ARG(mname);                                                             \
    MEMBER_CHECK_11_ARG(mname);                                                             \
    MEMBER_CHECK_12_ARG(mname);                                                             \
    MEMBER_CHECK_13_ARG(mname);                                                             \
    MEMBER_CHECK_14_ARG(mname);                                                             \
    MEMBER_CHECK_15_ARG(mname);                                                             \
                                                                                            \
public:                                                                                     \
    /* First check the availability of a method using HasMember */                          \
    static const bool value = Check<hidden::membercheck::HasMember_##mname <Type>::value,   \
                                   CallDetails>::value;                                     \
    typedef typename boost::integral_constant<bool, value> type;                            \
}
#endif

#define CREATE_MEMBER_FUNC_SIG_CHECK(mname, signature, signame)                             \
namespace hidden                                                                            \
{                                                                                           \
namespace sigcheck##signame {                                                               \
    CREATE_MEMBER_FUNC_CHECK(mname);                                                        \
}                                                                                           \
}                                                                                           \
                                                                                            \
template<typename Type>                                                                     \
struct BOOST_PP_CAT(IsMemberSigFunc_##mname, _##signame)                                    \
{                                                                                           \
public:                                                                                     \
    /* First check the availability of a method using HasMember */                          \
    static const bool value = hidden::sigcheck##signame::IsMemberFunc_##mname               \
                                                            <Type, signature>::value;       \
    typedef typename boost::integral_constant<bool, value> type;                            \
}

}

#endif /* PREPROCESSOR_TYPES_H_ */
