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
 * Problems:
 *  No argument private methods will flag HasMemberFunc as true while
 * versions with arguments will not (this shouldn't be happening, which makes
 * me worried some compilers will complain/change behavior).
 *  HasMember also flags private methods (not much to be done on this front).
 *
 * TODO Add descriptions
 * TODO Add macro for call if template match
 */
#ifndef PREPROCESSOR_TYPES_H_
#define PREPROCESSOR_TYPES_H_

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/static_assert.hpp>
#include "detail/pptype_member_check.hpp"
#include "ppmessages.hpp"

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

template<bool T, typename Yes, typename No>
struct chooseIf {
   typedef Yes type;
};

template<typename Yes, typename No>
struct chooseIf<false, Yes, No> {
    typedef No type;
};

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

/* Only types which match the signature T can be placed in the second template */
template<typename T, T> struct forceTypeCheck {
    static const bool value = true;
};

#define CREATE_MEMBER_PRESENCE_CHECK(mname)                                                 \
    template<typename Type, typename SFINAE = void>                                         \
    /* Fallback class definition if Type is not a class */                                  \
    class mname##MethodPresent                                                              \
    {                                                                                       \
    public:                                                                                 \
        /* Non-class types never have members */                                            \
        static const bool value = false;                                                    \
        /* Used for boost template logic */                                                 \
        typedef typename boost::integral_constant<bool, value> type;                        \
    };                                                                                      \
                                                                                            \
    /* Class definition for Types which are classes */                                      \
    template<typename Type>                                                                 \
    class mname##MethodPresent <Type, typename ::core::enableIf<                            \
        boost::is_class<Type>::value>::type>                                                \
    {                                                                                       \
        /* Use char and long sizes to indicate occurrence */                                \
        typedef char Yes;                                                                   \
        typedef long No;                                                                    \
                                                                                            \
        struct Derived {                                                                    \
            void mname(){}                                                                  \
        };                                                                                  \
                                                                                            \
        struct MixedType : public Type, public Derived {};                                  \
                                                                                            \
        /* Negative case, if Derived was no in name conflict with Type */                   \
        template<typename U>                                                                \
        static typename ::core::enableIf<                                                   \
            ::core::forceTypeCheck<void (Derived::*)(), &U::mname>::value,                  \
        No>::type                                                                           \
        resolve(U*);                                                                        \
        static Yes resolve(...);                                                            \
                                                                                            \
        /* Performs the check, we could make different versions here if needed */           \
        template<typename T>                                                                \
        struct Check {                                                                      \
            static const bool value = (sizeof(Yes) == sizeof(resolve((T*)(NULL))));         \
        };                                                                                  \
                                                                                            \
    public:                                                                                 \
        /* True if the class has the member */                                              \
        static const bool value = Check<MixedType>::value;                                  \
        /* Used for boost template logic */                                                 \
        typedef typename boost::integral_constant<bool, value> type;                        \
    }

// Visual Studio's SFINAE support is laughablely buggy, so argument checks aren't available
// under any VS platforms (there's an open bug ticket from 2006 which would fix the issue
// if Microsoft ever addresses it -- not fixed in VS2013)
#ifdef _MSC_VER
#define MEMBER_ARG_CHECKS_AVAILABLE 0

#else

#define MEMBER_ARG_CHECKS_AVAILABLE 1

/*
 * Macros that are expanded upon in pptypeMemberCheck.hpp to generate
 * Checks for various number of arguments.
 */
#define MEMBER_CHECK_ARG_GEN(mname, targlist, carglist, farglist)                           \
    template<targlist, typename R>                                                          \
    struct Check<true, carglist> {                                                          \
        static const bool value =                                                           \
            sizeof(ReturnValueCheck<Type, R>::resolve(                                      \
                    /* Fake comma operator for void return */                               \
                    (((DerivedType*)NULL)->mname(farglist),                                 \
                            hidden::mname##VoidResultHolder <Type>()))                      \
                   ) == sizeof(Yes);                                                        \
    };                                                                                      \
    template <targlist, typename R>                                                         \
    struct Check<true, carglist const> {                                                    \
        static const bool value =                                                           \
            sizeof(ReturnValueCheck<Type, R>::resolve(                                      \
                    /* Fake comma operator for void return */                               \
                    (((DerivedConstMethodType*)NULL)->mname(farglist),                      \
                            hidden::mname##VoidResultHolder <Type>()))                      \
                   ) == sizeof(Yes);                                                        \
    }

/*
 * Macro used to generate all of the instances for checking the type
 * information of no argument member functions.
 */
#define MEMBER_CHECK_NO_ARGS(mname)                                                         \
    /* Enabled if we can cast to a method returning non-void R */                           \
    template<typename R, typename T>                                                        \
    static typename ::core::enableIf                                                        \
        <sizeof(static_cast<R (T::*)()>(&T::mname)) != 0 &&                                 \
                !boost::is_same<R, void>::value, Yes>::type                                 \
    noArgMethodCheck(T*);                                                                   \
    /* Enabled if we can cast to a method returning void */                                 \
    template<typename R, typename T>                                                        \
    static typename ::core::enableIf                                                        \
        <sizeof(static_cast<void (T::*)()>(&T::mname)) != 0,                                \
            typename ::core::chooseIf<boost::is_same<R, void>::value,                       \
            Yes, No>::type>::type                                                           \
    noArgMethodCheck(T*);                                                                   \
    /* Fallback for no match */                                                             \
    template<typename R>                                                                    \
    static No noArgMethodCheck(...);                                                        \
                                                                                            \
    /* Enabled for const objects if we can cast to a method returning non-void R */         \
    template<typename R, typename T>                                                        \
    static typename ::core::enableIf                                                        \
        <sizeof(static_cast<R (T::*)() const>(&T::mname)) != 0 &&                           \
                !boost::is_same<R, void>::value, Yes>::type                                 \
    noArgMethodConstCheck(T*);                                                              \
    /* Enabled for const objects if we can cast to a method returning void */               \
    template<typename R, typename T>                                                        \
    static typename ::core::enableIf                                                        \
        <sizeof(static_cast<void (T::*)() const>(&T::mname)) != 0,                          \
            typename ::core::chooseIf<boost::is_same<R, void>::value,                       \
            Yes, No>::type>::type                                                           \
    noArgMethodConstCheck(T*);                                                              \
    template<typename R>                                                                    \
    /* Fallback for no match */                                                             \
    static No noArgMethodConstCheck(...);                                                   \
    template<typename R>                                                                    \
                                                                                            \
    struct Check<true, R() const> {                                                         \
        static const bool value =                                                           \
            sizeof(noArgMethodConstCheck<R>((DerivedVoid*)NULL)) == sizeof(Yes);            \
    };                                                                                      \
    template<typename R>                                                                    \
    struct Check<true, R()> {                                                               \
        static const bool value =                                                           \
            sizeof(noArgMethodCheck<R>((DerivedVoid*)NULL)) == sizeof(Yes) ||               \
            sizeof(noArgMethodConstCheck<R>((DerivedVoid*)NULL)) == sizeof(Yes);            \
    }

#define CREATE_MEMBER_ARG_CHECKS(mname)                                                     \
    template<typename Type, typename CallDetails>                                           \
    struct mname##MethodSignaturePresent;                                                   \
                                                                                            \
    namespace hidden {                                                                      \
    namespace membercheck {                                                                 \
    /* Hide the member check function here to avoid name clobbering */                      \
    CREATE_MEMBER_PRESENCE_CHECK(mname);                                                    \
    }                                                                                       \
    template<typename Type>                                                                 \
    class mname##VoidResultHolder {};                                                       \
                                                                                            \
    /* Use comma operator because it has lowest precedence of C++ operators */              \
    /* This resolves bad return types (i.e. void) blowing up template calls */              \
    template<typename Type, typename U>                                                     \
    U& operator,(U&, mname##VoidResultHolder <Type>);                                       \
    /* Need const version as well */                                                        \
    template<typename Type, typename U>                                                     \
    U const& operator,(U const&, mname##VoidResultHolder <Type>);                           \
    }                                                                                       \
                                                                                            \
    template<typename Type, typename CallDetails>                                           \
    struct mname##MethodSignaturePresent                                                    \
    {                                                                                       \
    private:                                                                                \
        /* Keep Yes and No as a struct to avoid ambiguity in inheritance */                 \
        struct Yes { char check; };                                                         \
        struct No { Yes notCheck[2]; };                                                     \
                                                                                            \
        /* Check the result of the expression */                                            \
        template<typename T, typename R>                                                    \
        struct ReturnValueCheck {                                                           \
            static Yes resolve(R);                                                          \
            static Yes resolve(Yes);                                                        \
            static No resolve(...);                                                         \
            static No resolve(No);                                                          \
        };                                                                                  \
                                                                                            \
        /* Fallback template typing for void returns */                                     \
        template<typename T>                                                                \
        struct ReturnValueCheck<T, void> {                                                  \
            static Yes resolve(Yes);                                                        \
            static Yes resolve(...);                                                        \
            static No resolve(No);                                                          \
        };                                                                                  \
                                                                                            \
        /* Add an overloaded method */                                                      \
        struct Derived : public Type {                                                      \
            using Type::mname;                                                              \
            /* Gives warning with both active, not sure if necessary ... */                 \
            /*No mname(...);*/                                                              \
            No mname(...) const;                                                            \
        };                                                                                  \
        /* Add an overloaded method */                                                      \
        struct DerivedConst : public Type {                                                 \
            using Type::mname;                                                              \
            No mname(...) const;                                                            \
        };                                                                                  \
        /* Used to hide any private variables */                                            \
        struct DerivedVoid : public Type {                                                  \
            using Type::mname;                                                              \
        };                                                                                  \
                                                                                            \
        /* Keep constness of Type in Derived Type */                                        \
        typedef typename ::core::constnessCheck                                             \
                <Type, Derived>::type DerivedType;                                          \
        /* Force constant methods to have constant type */                                  \
        typedef typename ::core::constnessCheck                                             \
                <const int, DerivedConst>::type DerivedConstMethodType;                     \
                                                                                            \
        /* Default case is false */                                                         \
        template<bool has, typename F>                                                      \
        struct Check {                                                                      \
            static const bool value = false;                                                \
        };                                                                                  \
                                                                                            \
        /* The no arguments case was a pain to get right -- took a week of trying options */\
        /* It doesn't use the same system of inheritance as multi-arg checks */             \
        MEMBER_CHECK_NO_ARGS(mname);                                                        \
        /* Create checks for 1 arg to up to 15 arguments */                                 \
        MEMBER_CHECK_1_ARG(mname);                                                          \
        MEMBER_CHECK_2_ARG(mname);                                                          \
        MEMBER_CHECK_3_ARG(mname);                                                          \
        MEMBER_CHECK_4_ARG(mname);                                                          \
        MEMBER_CHECK_5_ARG(mname);                                                          \
        MEMBER_CHECK_6_ARG(mname);                                                          \
        MEMBER_CHECK_7_ARG(mname);                                                          \
        MEMBER_CHECK_8_ARG(mname);                                                          \
        MEMBER_CHECK_9_ARG(mname);                                                          \
        MEMBER_CHECK_10_ARG(mname);                                                         \
        MEMBER_CHECK_11_ARG(mname);                                                         \
        MEMBER_CHECK_12_ARG(mname);                                                         \
        MEMBER_CHECK_13_ARG(mname);                                                         \
        MEMBER_CHECK_14_ARG(mname);                                                         \
        MEMBER_CHECK_15_ARG(mname);                                                         \
                                                                                            \
    public:                                                                                 \
        /* First check the availability of a method using MethodPresent */                  \
        static const bool value = Check<                                                    \
            hidden::membercheck::mname##MethodPresent <Type>::value, CallDetails>::value;   \
        /* Used for boost template logic */                                                 \
        typedef typename boost::integral_constant<bool, value> type;                        \
    }

#define CREATE_MEMBER_SIG_CHECK(mname, signature, signame)                                  \
    namespace hidden { namespace sigcheck##signame {                                        \
    CREATE_MEMBER_ARG_CHECKS(mname);                                                        \
    }}                                                                                      \
                                                                                            \
    template<typename Type>                                                                 \
    struct BOOST_PP_CAT(mname##MethodSignaturePresent, For##signame)                        \
    {                                                                                       \
    public:                                                                                 \
        /* First check the availability of a method using HasMember */                      \
        static const bool value = hidden::sigcheck##signame::mname##MethodSignaturePresent  \
                                                                <Type, signature>::value;   \
        typedef typename boost::integral_constant<bool, value> type;                        \
    }
#endif

}
#endif /* PREPROCESSOR_TYPES_H_ */
