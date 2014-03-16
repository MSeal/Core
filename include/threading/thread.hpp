#ifndef CORE_THREAD_H_
#define CORE_THREAD_H_

#include <boost/thread.hpp>

namespace core { namespace threading {

typedef boost::thread Thread;
typedef boost::shared_ptr<Thread> ThreadPtr;

}}

#endif /* CORE_THREAD_H_ */
