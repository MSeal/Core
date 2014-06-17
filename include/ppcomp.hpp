/*
 * ppcomp.hpp
 *
 * Defines some useful macros for ignore specific warning or problem statements.
 */
#ifndef PREPROCESSOR_COMPILER_HELPER_H_
#define PREPROCESSOR_COMPILER_HELPER_H_

// For any GCC Pragmas 

#if defined(_MSC_VER)
// Used to declare friends for template struct typedefs
#   define ELABORATED_FRIEND(vartemplate, variable) friend typename variable
// Defined unused warning quelcher for variables with known non-use
#   if (_MSC_VER >= 1500)
#       define MSVC_IGNORE_WARNING(warning, variable)                       \
            __pragma( warning( push ) )                                     \
            __pragma( warning( once : 4385 ) )                              \
            variable                                                        \
            __pragma( warning( pop ) )
#   else
//      Nothing to do...
#       define MSVC_IGNORE_WARNING(warning, variable) variable
#   endif
#   define IGNORE_UNUSED_WARNING(variable) MSVC_IGNORE_WARNING(4385, variable)
#elif defined(__clang__)
#   define ELABORATED_FRIEND(vartemplate, variable) template< vartemplate > friend class variable
#   define IGNORE_UNUSED_WARNING(variable) variable __attribute__((unused))
#elif defined(__GNUC__)
//  See https://git.gnome.org/browse/glib/tree/glib/gmacros.h for gcc specifics
#   define ELABORATED_FRIEND(vartemplate, variable) friend class variable
#   define IGNORE_UNUSED_WARNING(variable) variable __attribute__((unused))
#   if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
//  For any GCC Pragmas calls
#   endif
// Unlikely to be used, but useful when you need it
#   if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#       define BRANCH_LIKELY(expr) __builtin_expect(!!(expr), 1)
#       define BRANCH_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#   endif
#endif

// Now for all the catches on undefined macros
#ifndef ELABORATED_FRIEND
#   define ELABORATED_FRIEND(vartemplate, variable) friend class variable
#endif

#ifndef IGNORE_UNUSED_WARNING
//  Can't automate this generically...
#   define IGNORE_UNUSED_WARNING(variable) variable
#endif
#ifndef IGNORE_LONG_LONG_WARNING
#   define IGNORE_LONG_LONG_WARNING(variable) __extension__ variable
#endif

#ifndef BRANCH_LIKELY
#   define BRANCH_LIKELY(expr) expr
#endif
#ifndef BRANCH_UNLIKELY
#   define BRANCH_UNLIKELY(expr) expr
#endif

#endif /* PREPROCESSOR_COMPILER_HELPER_H_ */
