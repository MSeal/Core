/*
 * time.hpp
 */

#ifndef CORE_TIME_HPP_
#define CORE_TIME_HPP_

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/thread/thread_time.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {
/*
 * Returns a system time representing milliseconds in the future.
 */
inline const boost::system_time getFuture(long milliseconds) {
    return boost::get_system_time() + boost::posix_time::milliseconds(milliseconds);
}

/*
 * Returns a system time representing milliseconds in the past.
 */
inline const boost::system_time getPast(long milliseconds) {
    return boost::get_system_time() - boost::posix_time::milliseconds(milliseconds);
}
}

#endif /* CORE_TIME_HPP_ */
