/*
 * ppmessages.hpp
 *
 * Defines some useful macros for ignore specific warning or problem statements.
 */
#ifndef PREPROCESSOR_COMPILER_HELPER_H_
#define PREPROCESSOR_COMPILER_HELPER_H_

#ifdef _MSC_VER

#define IGNORE_UNUSED_WARNING(variable)                                     \
    __pragma( warning( push ) )                                             \
    __pragma( warning( once : 4385 ) )                                      \
    variable                                                                \
    __pragma( warning( pop ) )
#elif __GNUC__

#define IGNORE_UNUSED_WARNING(variable) variable __attribute__((unused))

#else

// Can't automate this generically...
#define IGNORE_UNUSED_WARNING(variable) variable

#endif

#endif /* PREPROCESSOR_COMPILER_HELPER_H_ */