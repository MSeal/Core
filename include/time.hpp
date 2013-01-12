/*
 * time.hpp
 */

#ifndef CORE_TIME_H_
#define CORE_TIME_H_

namespace core {
/*
 * Returns a system time representing milliseconds in the future.
 */
const boost::system_time getFuture(long milliseconds) {
    return boost::get_system_time() + boost::posix_time::milliseconds(milliseconds);
}
}

#endif /* CORE_TIME_H_ */
