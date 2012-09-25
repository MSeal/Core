/* 
 * numutil.h
 * Provides basic numerical aids for various computations.
 */

#ifndef NUM_UTIL_H_
#define NUM_UTIL_H_

#include <boost/config/select_compiler_config.hpp> // For BOOST_HAS_LONG_LONG
#include <math.h>
#include <algorithm>

namespace core {

/*
 * Rounds a floating type
 * 'round' is a name conflict with gcc compiler
 */
inline float roundValue(float f) {
    return (f > 0.0) ? floor(f + 0.5F) : ceil(f - 0.5F);
}
inline double roundValue(double d) {
    return (d > 0.0) ? floor(d + 0.5) : ceil(d - 0.5);
}
inline long double roundValue(long double d) {
    return (d > 0.0) ? floor(d + 0.5) : ceil(d - 0.5);
}


/* Rounds a float to an integer */
inline int roundToInt(float f) {
    return f > 0.0 ? int(f + 0.5F) : int(f - 0.5F);
}
inline int roundToInt(double d) {
    return d > 0.0 ? int(d + 0.5) : int(d - 0.5);
}
inline int roundToInt(long double d) {
    return d > 0.0 ? int(d + 0.5) : int(d - 0.5);
}


/* Rounds a float to a long integer */
inline long roundToLong(float f) {
    return f > 0.0 ? long(f + 0.5F) : long(f - 0.5F);
}
inline long roundToLong(double d) {
    return d > 0.0 ? long(d + 0.5) : long(d - 0.5);
}
inline long roundToLong(long double d) {
    return d > 0.0 ? long(d + 0.5) : long(d - 0.5);
}

#ifdef BOOST_HAS_LONG_LONG
/* Rounds a float to a long long integer */
inline long long roundToLongLong(float f) {
    return f > 0.0 ? (long long)(f + 0.5F) : (long long)(f - 0.5F);
}
inline long long roundToLongLong(double d) {
    return d > 0.0 ? (long long)(d + 0.5) : (long long)(d - 0.5);
}
inline long long roundToLongLong(long double d) {
    return d > 0.0 ? (long long)(d + 0.5) : (long long)(d - 0.5);
}
#endif

/* Returns true if x is a power of two */
inline bool isPowerOfTwo(unsigned short x) {
  return ((x != 0) && !(x & (x - 1)));
}
inline bool isPowerOfTwo(unsigned int x) {
  return ((x != 0) && !(x & (x - 1)));
}
inline bool isPowerOfTwo(unsigned long x) {
  return ((x != 0) && !(x & (x - 1)));
}
inline bool isPowerOfTwo(unsigned long long x) {
  return ((x != 0) && !(x & (x - 1)));
}


/* Returns the next lowest positive power of 2 */
template<typename T>
inline T floorToPowerOfTwo(const T input) {
    T value = 1;
    while ((value << 1)-1 < input) { value <<= 1; }
    return value;
}
inline unsigned long floorToPowerOfTwo(float input) {
    float value = 1.0;
    float check = input / 2.0F;
    while (value < check) { value *= 2.0F; }
    return roundToLong(value);
}
inline unsigned long floorToPowerOfTwo(double input) {
    double value = 1.0;
    double check = input / 2.0;
    while (value < check) { value *= 2.0F; }
    return roundToLong(value);
}
inline unsigned long floorToPowerOfTwo(long double input) {
    long double value = 1.0;
    long double check = input / 2.0;
    while (value < check) { value *= 2.0F; }
    return roundToLong(value);
}


/* Returns the next highest positive power of 2 */
template<typename T>
inline T ceilToPowerOfTwo(const T input) {
    T value = 1;
    while (value < input) { value <<= 1; }
    return value;
}
inline unsigned long ceilToPowerOfTwo(float input) {
    float value = 1.0;
    while (value < input) { value *= 2.0F; }
    return roundToLong(value);
}
inline unsigned long ceilToPowerOfTwo(double input) {
    double value = 1.0;
    while (value < input) { value *= 2.0F; }
    return roundToLong(value);
}
inline unsigned long ceilToPowerOfTwo(long double input) {
    long double value = 1.0;
    while (value < input) { value *= 2.0F; }
    return roundToLong(value);
}


/* Limits the val to be between a minVal and maxVal and returns its new value */
template <typename T>
inline T constrained(const T val, const T minval, const T maxval) {
	return std::min(std::max(val, minval), maxval);
}
}

#endif /* NUM_UTIL_H_ */
