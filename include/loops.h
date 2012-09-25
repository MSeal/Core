/*
 * loops.h
 * Defines several loop specific macros and functions
 *
 * These loops are as follows:
 *
 * Generates a simple for loop that repeats numiter times. Use the
 * long version for repetitions larger than can fit in an unsigned
 * int.
 *
 *
 * 	    forRepeat(numiter)
 * 	    forRepeatLong(numiter)
 *
 *
 * Runs a loop iterator from start to end repetitions. This is mostly
 * unnecessary compared to the standard forEach loop, but in cases
 * where direct access to the iterator is needed, this becomes useful
 * again. The loop increments iter each iteration until it matches end.
 *
 *
 *      forIter(iter, start, end)
 *      reverseForIter(iter, start, end)
 *
 *
 * Creates a for loop that iterating through each item in a container.
 * This is a simple renaming of BOOST_FOREACH, which is a fantastic
 * macro for iterating over arbitrary data structures.
 *
 *
 *      forEach(item, container)
 *      reverseForEach(item, container)
 *
 *
 * Performs a forEach on the item and container, but also sets count to
 * the number of times for which the loop as iterated. Thus the first
 * iteration count will be 0, then 1 on the next, and so on until the
 * loop ends.
 *
 *
 *      enumerateEach(count, item, container)
 *      reverseEnumerateEach(count, item, container)
 *
 *
 * Runs the function on each item in container. This is the only loop
 * from this file which is an actual function and not macro based.
 * Since the function is passed to us, it's syntactically possible to
 * keep this inside a function and hold tighter type checks.
 *
 *
 *      onEach(item, function, container)
 *      onEach(function, container)
 * 	    reverseOnEach(item, function, container)
 * 	    reverseOnEach(function, container)
 */

#ifndef LOOPS_H_
#define LOOPS_H_

#include <boost/foreach.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>

/* For Repeat */
#define forRepeat(numiter)                                                          \
    if (unsigned int BOOST_FOREACH_ID(_iter_unique_) = 0) {} else                   \
        for(unsigned int BOOST_FOREACH_ID(_iter_maxiter_) = numiter;                \
            BOOST_FOREACH_ID(_iter_unique_) != BOOST_FOREACH_ID(_iter_maxiter_);    \
            BOOST_FOREACH_ID(_iter_unique_)++)

#define forRepeatLong(numiter)                                                      \
    if (unsigned long BOOST_FOREACH_ID(_iter_unique_) = 0) {} else                  \
        for(unsigned long BOOST_FOREACH_ID(_iter_maxiter_) = numiter;               \
            BOOST_FOREACH_ID(_iter_unique_) != BOOST_FOREACH_ID(_iter_maxiter_);    \
            BOOST_FOREACH_ID(_iter_unique_)++)

/* End For Repeat */

/* For Iter */
#define forIter(iter, start, end) for(iter = start; iter != end; iter++)
#define reverseForIter(iter, start, end) for(iter = start; iter != end; iter--)
/* End For Iter */

/* For Each */
/* Used to trick the CDT Parser to accepting our syntax */
#ifdef __CDT_PARSER__
#   define forEach(item, container) for(item; container;)
#   define reverseForEach(item, container) for(item; container;)
#else
#   define forEach BOOST_FOREACH
#   define reverseForEach BOOST_REVERSE_FOREACH
#endif
/* End For Each */

/* Enumerate */
namespace core { namespace loops { namespace detail {
template<typename T>
void incrementT(T *t) {
	(*t)++;
}

struct IncrementCounterPassthrough {
	bool checker;
	boost::function<void(void)> incrementer;

	template<typename Count>
	IncrementCounterPassthrough(Count& t) {
		t = -1;
		checker = true;
		incrementer = boost::bind(&incrementT<Count>, &t);
	}

	template<typename T>
	T& operator=(T& rhs) {
		incrementer();
		return rhs;
	}
};
}}}

/* Used to trick the CDT Parser to accepting our syntax */
#ifdef __CDT_PARSER__
#   define enumerateEach(count, item, container) count = 0; for(item; container;)
#   define reverseEnumerateEach(count, item, container) count = 0; for(item; container;)
#else
#   define enumerateEach(count, item, container)                                                                \
        for(::core::loops::detail::IncrementCounterPassthrough BOOST_FOREACH_ID(_foreach_count_pass)(count);	\
                BOOST_FOREACH_ID(_foreach_count_pass).checker; 													\
                BOOST_FOREACH_ID(_foreach_count_pass).checker = false)											\
            BOOST_FOREACH(item = BOOST_FOREACH_ID(_foreach_count_pass), container)

#   define reverseEnumerateEach(count, item, container)                                                         \
        for(::core::loops::detail::IncrementCounterPassthrough BOOST_FOREACH_ID(_foreach_count_pass)(count);	\
                BOOST_FOREACH_ID(_foreach_count_pass).checker; 													\
				BOOST_FOREACH_ID(_foreach_count_pass).checker = false)											\
            BOOST_REVERSE_FOREACH(item = BOOST_FOREACH_ID(_foreach_count_pass), container)
#endif
/* End Enumerate */

/* On Each */
template <class I, class C>
inline void onEach(void (*function)(I), C & container) {
	forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, void (*function)(I), C & container) {
	forEach(item, container) function(item);
}
template <class I, class C>
inline void onEach(void (*function)(I&), C & container) {
	forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, void (*function)(I&), C & container) {
	forEach(item, container) function(item);
}
// Do the same for boost::function
template <class I, class C>
inline void onEach(boost::function<void(I)>& function, C & container) {
    forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, boost::function<void(I)>& function, C & container) {
    forEach(item, container) function(item);
}
template <class I, class C>
inline void onEach(boost::function<void(I&)>& function, C & container) {
    forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, boost::function<void(I&)>& function, C & container) {
    forEach(item, container) function(item);
}
/* End On Each */

/* On Each Reverse */
template <class I, class C>
inline void reverseOnEach(void (*function)(I), C & container) {
	reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I & item, void (*function)(I), C & container) {
	reverseForEach(item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(void (*function)(I&), C & container) {
	reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I & item, void (*function)(I&), C & container) {
	reverseForEach(item, container) function(item);
}
// Do the same for boost::function
template <class I, class C>
inline void reverseOnEach(boost::function<void(I&)> function, C & container) {
    reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I & item, boost::function<void(I&)> function, C & container) {
    reverseForEach(item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(boost::function<void(I&)>& function, C & container) {
    reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I & item, boost::function<void(I&)>& function, C & container) {
    reverseForEach(item, container) function(item);
}
/* End On Each Reverse */

#endif /* LOOPS_H_ */
