/*
 * ppcomp.hpp
 *
 * Defines some useful macros for ignore specific warning or problem statements.
 */
#ifndef PREPROCESSOR_COMPILER_HELPER_H_
#define PREPROCESSOR_COMPILER_HELPER_H_

#if defined(_MSC_VER)
// Used to declare friends for template struct typedefs
#   define ELABORATED_FRIEND(vartemplate, variable) friend typename variable
// Defined unused warning quelcher for variables with known non-use
#   define IGNORE_UNUSED_WARNING(variable)                                  \
    __pragma( warning( push ) )                                             \
    __pragma( warning( once : 4385 ) )                                      \
    variable                                                                \
    __pragma( warning( pop ) )
#elif defined(__clang__)
#   define ELABORATED_FRIEND(vartemplate, variable) template< vartemplate > friend class variable
#   define IGNORE_UNUSED_WARNING(variable) variable __attribute__((unused))
#elif defined(__GNUC__)
#   define ELABORATED_FRIEND(vartemplate, variable) friend class variable
#   define IGNORE_UNUSED_WARNING(variable) variable __attribute__((unused))
#else
#   define ELABORATED_FRIEND(vartemplate, variable) friend class variable
// Can't automate this generically...
#   define IGNORE_UNUSED_WARNING(variable) variable
#endif

#endif /* PREPROCESSOR_COMPILER_HELPER_H_ */