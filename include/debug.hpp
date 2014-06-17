/* 
 * debug.h
 * Prints an error message into a the terminal or output area.
 * These calls are NOT thread safe and for quick error checking in
 * consol only. They are also used for default messaging when
 * the logging system's health is in question/being tested.
 */

#ifndef GAME_DEBUG_H_
#define GAME_DEBUG_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/config/select_compiler_config.hpp>
#include BOOST_COMPILER_CONFIG
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {
/*
 * Indicates if debug mode is on or not
 */
inline bool debugModeOn() {
#ifdef CORE_DEBUG
    return true;
#else
    return false;
#endif
}

#ifdef BOOST_MSVC
inline void debugMessage(const char *str) {
#ifdef CORE_DEBUG
    OutputDebugStringA(str);
    OutputDebugStringA("\n");
#endif
}

template<typename charT, typename traits, typename Alloc>
inline void debugMessage(const std::basic_string<charT, traits, Alloc>& str) {
#ifdef CORE_DEBUG
    OutputDebugStringA(str.c_str());
    OutputDebugStringA("\n");
#endif
}
#else 
inline void debugMessage(const char *str) {
#ifdef CORE_DEBUG
    fprintf(stdout, str);
    fprintf(stdout, "\n");
#endif
}

template<typename charT, typename traits, typename Alloc>
inline void debugMessage(const std::basic_string<charT, traits, Alloc>& str) {
#ifdef CORE_DEBUG
    fprintf(stdout, str.c_str());
    fprintf(stdout, "\n");
#endif
}
#endif
}

#endif
