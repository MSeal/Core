/*
 * pptypes.hpp
 */
#ifndef PREPROCESSOR_TYPES_H_
#define PREPROCESSOR_TYPES_H_

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>

namespace core {

namespace detail {
template<bool T, class R=void>
struct enable_if {
   typedef R type;
};

template<class R>
struct enable_if<false, R>{};
}

#define CONCAT_MACRO(first, second) first##second

/*
 * Taken freely from
 * http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
 */
#define CREATE_MEMBER_CHECK_OLD(mname, mtype)                                               \
template<typename Type> class HasMember_##mname                                             \
{                                                                                           \
    /* This type won't compile if the second template parameter isn't of type T,        */  \
    /* so I can put an expected type in the first parameter and the instance variable   */  \
    /* itself in the second thus checking that the function has a specific signature.   */  \
    template<typename T, T> struct Check;                                                   \
                                                                                            \
    typedef char Yes;                                                                       \
    typedef long No;                                                                        \
                                                                                            \
    template<typename T> static Yes  Has_##mname(Check<mtype T::*, &T::mname>*);            \
    template<typename T> static No   Has_##mname(...);                                      \
public:                                                                                     \
    static bool const value = (sizeof(Has_##mname<Type>(0)) == sizeof(Yes));                \
    typedef typename boost::integral_constant<bool, value> type;                            \
};

/*
 * Taken freely from courtesy of FireAphis
 * http://stackoverflow.com/questions/257288/
 * is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence/264088#264088
 */
#define CREATE_MEMBER_FUNC_SIG_CHECK_OLD(fname, fret, fargs, fconst)                        \
template<class Type> class HasMemberFuncSig_##fname                                         \
{                                                                                           \
    /* This type won't compile if the second template parameter isn't of type T,        */  \
    /* so I can put a function pointer type in the first parameter and the function     */  \
    /* itself in the second thus checking that the function has a specific signature.   */  \
    template <typename T, T> struct TypeCheck;                                              \
                                                                                            \
    typedef char Yes;                                                                       \
    typedef long No;                                                                        \
                                                                                            \
    /* A helper struct to hold the declaration of the function pointer.                 */  \
    template <typename T> struct fname {                                                    \
        typedef fret (T::*fptr)(fargs) fconst;                                              \
    };                                                                                      \
                                                                                            \
    template<typename T> static Yes Has_##fname(                                            \
            TypeCheck<typename fname<T>::fptr, &T::fname>*);                                \
    template<typename T> static No  Has_##fname(...);                                       \
                                                                                            \
public:                                                                                     \
    static bool const value = (sizeof(Has_##fname<Type>(0)) == sizeof(Yes));                \
    typedef typename boost::integral_constant<bool, value> type;                            \
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
class HasMember_##mname <Type, typename ::core::detail::enable_if<                          \
    boost::is_class<Type>::value>::type>                                                    \
{                                                                                           \
    /* Use char and long sizes to indicate occurance */                                     \
    typedef char Yes;                                                                       \
    typedef long No;                                                                        \
                                                                                            \
    struct BaseMixin                                                                        \
    {                                                                                       \
        void mname(){}                                                                      \
    };                                                                                      \
                                                                                            \
    struct Base : public Type, public BaseMixin {};                                         \
                                                                                            \
    /* Only types which match the signature T can be placed in the second template */       \
    template<typename T, T> class MemberCheck{};                                            \
                                                                                            \
    template<typename U>                                                                    \
    static No deduce(U*, MemberCheck<void (BaseMixin::*)(), &U::mname>* = 0);               \
    static Yes deduce(...);                                                                 \
                                                                                            \
public:                                                                                     \
    /* True if the class has the member */                                                  \
    static const bool value = (sizeof(Yes) == sizeof(deduce((Base*)(0))));                  \
    /* Used for boost template logic */                                                     \
    typedef typename boost::integral_constant<bool, value> type;                            \
}
#endif

#define CREATE_MEMBER_FUNC_CHECK(mname)                                                     \
namespace detail                                                                            \
{                                                                                           \
namespace membercheck {                                                                     \
    CREATE_MEMBER_CHECK(mname);                                                             \
}                                                                                           \
    template<typename Type>                                                                 \
    class voidExpResult{};                                                                  \
                                                                                            \
    /* Operator overloading comma 'operator,' */                                            \
    template<typename type, typename U>                                                     \
    U const& operator,(U const&, voidExpResult<type>);                                      \
                                                                                            \
    template<typename Type, typename U>                                                     \
    U& operator,(U&, voidExpResult<Type>);                                                  \
                                                                                            \
    template<typename SrcType, typename DestType>                                           \
    struct cloneConstness                                                                   \
    {                                                                                       \
        typedef DestType type;                                                              \
    };                                                                                      \
                                                                                            \
    template<typename SrcType, typename DestType>                                           \
    struct cloneConstness<const SrcType, DestType>                                          \
    {                                                                                       \
        typedef const DestType type;                                                        \
    };                                                                                      \
}                                                                                           \
                                                                                            \
template<typename Type, typename CallDetails>                                               \
struct IsMemberFunc_##mname                                                                 \
{                                                                                           \
private:                                                                                    \
    typedef char Yes;                                                                       \
    class No { Yes m[2]; };                                                                 \
                                                                                            \
    /* Add an overloaded method */                                                          \
    struct Derived : public Type                                                            \
    {                                                                                       \
        using Type::mname;                                                                  \
        No mname(...) const;                                                                \
    };                                                                                      \
                                                                                            \
    /* We take into account the possible constancy class type */                            \
    typedef typename detail::cloneConstness<Type, Derived>::type DerivedType;               \
                                                                                            \
    /* Check the result of the expression */                                                \
    template <typename T, typename DueType>                                                 \
    struct ReturnValueCheck                                                                 \
    {                                                                                       \
        static Yes deduce(DueType);                                                         \
        static No deduce(...);                                                              \
        /* Explicit congestion if Due_type is template? */                                  \
        static No deduce(No);                                                               \
        static No deduce(detail::voidExpResult<Type>);                                      \
    };                                                                                      \
                                                                                            \
    /* Check the result of the expression in the case */                                    \
    /* If we are not interested in the return type of the method */                         \
    template <typename T>                                                                   \
    struct ReturnValueCheck<T, void>                                                        \
    {                                                                                       \
        static Yes deduce(...);                                                             \
        static No deduce(No);                                                               \
    };                                                                                      \
                                                                                            \
    template <bool has, typename F>                                                         \
    struct Impl                                                                             \
    {                                                                                       \
        static const bool value = false;                                                    \
    };                                                                                      \
                                                                                            \
    template <typename arg1, typename r>                                                    \
    struct Impl<true, r(arg1)>                                                              \
    {                                                                                       \
        static const bool value =                                                           \
            sizeof(ReturnValueCheck<Type, r>::deduce(                                       \
                    /* Trick 'operator,' */                                                 \
                    (((DerivedType*)0)->mname(*(arg1*)0), detail::voidExpResult<Type>()))   \
                   ) == sizeof(Yes);                                                        \
    };                                                                                      \
                                                                                            \
public:                                                                                     \
    /* First check the availability of a method using HasMember */                          \
    static const bool value = Impl<detail::membercheck::HasMember_##mname <Type>::value,    \
                                   CallDetails>::value;                                     \
    typedef typename boost::integral_constant<bool, value> type;                            \
}

#define CREATE_MEMBER_FUNC_SIG_CHECK(mname, signature, signame)                             \
namespace detail                                                                            \
{                                                                                           \
namespace sigcheck##signame {                                                               \
    CREATE_MEMBER_FUNC_CHECK(mname);                                                        \
}                                                                                           \
}                                                                                           \
                                                                                            \
template<typename Type>                                                                     \
struct CONCAT_MACRO(IsMemberSigFunc_##mname, _##signame)                                    \
{                                                                                           \
public:                                                                                     \
    /* First check the availability of a method using HasMember */                          \
    static const bool value = detail::sigcheck##signame::IsMemberFunc_##mname               \
                                                            <Type, signature>::value;       \
    typedef typename boost::integral_constant<bool, value> type;                            \
}

/*
 * Freely taken from http://www.rsdn.ru/forum/cpp/2759773.1
 */

template <typename Type, typename Enabled=void>
class has_member {
public:
    static const bool result = false;
};

#ifdef __CDT_PARSER__
template <typename Type>
class has_member<Type, int>
#else
template <typename Type>
class has_member<Type, typename ::core::detail::enable_if<boost::is_class<Type>::value>::type>
#endif
{
    typedef char yes;
    typedef long no;

    struct BaseMixin
    {
        void func(){}
    };

    struct Base : public Type, public BaseMixin {};

    template<typename T, T> class MemberCheck{};

    template<typename U>
    static no deduce(U*, MemberCheck<void (BaseMixin::*)(), &U::func>* = 0);
    static yes deduce(...);

public:
    static const bool result = sizeof(yes) == sizeof(deduce((Base*)(0)));
};

namespace detail
{
    template <typename type>
    class void_exp_result
    {};

    // Operator overloading comma 'operator,'
    template <typename type, typename U>
    U const& operator,(U const&, void_exp_result<type>);

    template <typename type, typename U>
    U& operator,(U&, void_exp_result<type>);

    template <typename src_type, typename dest_type>
    struct clone_constness
    {
        typedef dest_type type;
    };

    template <typename src_type, typename dest_type>
    struct clone_constness<const src_type, dest_type>
    {
        typedef const dest_type type;
    };
}


template <typename type, typename call_details>
struct is_call_possible
{
private:
    class yes {};
    class no { yes m[2]; };

    // Add an overloaded method
    struct derived : public type
    {
        using type::func;
        no func(...) const;
    };

    // We take into account the possible constancy class type
    typedef typename detail::clone_constness<type, derived>::type derived_type;

    // Check the result of the expression
    template <typename T, typename due_type>
    struct return_value_check
    {
        static yes deduce(due_type);
        static no deduce(...);
        // explicit congestion if
        // Due_type template designer has
        static no deduce(no);
        static no deduce(detail::void_exp_result<type>);
    };

    // Check the result of the expression in the case
    // If we are not interested in the return type of the method
    template <typename T>
    struct return_value_check<T, void>
    {
        static yes deduce(...);
        static no deduce(no);
    };

    template <bool has, typename F>
    struct impl
    {
        static const bool value = false;
    };

    template <typename arg1, typename r>
    struct impl<true, r(arg1)>
    {
        static const bool value =
            sizeof(return_value_check<type, r>::deduce(
                    // Trick 'operator,'
                    (((derived_type*)0)->func(*(arg1*)0), detail::void_exp_result<type>()))
                   ) == sizeof(yes);

    };

    // Etc.
    // Template <typename arg1, typename arg2, typename r>
    // Struct impl <true, r(arg1, arg2)> {...

public:
    // first check the availability of a method using has_member
    static const bool value = impl<has_member<type>::result, call_details>::value;
};

}

#endif /* PREPROCESSOR_TYPES_H_ */
