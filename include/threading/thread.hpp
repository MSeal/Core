#ifndef CORE_THREAD_H_
#define CORE_THREAD_H_

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/thread.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core { namespace threading {

typedef boost::thread Thread;
typedef boost::shared_ptr<Thread> ThreadPtr;

}}

#endif /* CORE_THREAD_H_ */
