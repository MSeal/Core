#ifndef CORE_THREAD_H_
#define CORE_THREAD_H_

#include <boost/thread.hpp>

namespace core { namespace threading {

/*
 * Used to trick the CDT Parser to accepting our syntax.
 * Fake Thread definition that makes the CDT Parser see all
 * boost::thread methods when parsing files. The actual
 * compilation completely ignores this fake class.
 */
#ifdef __CDT_PARSER__
class Thread {
public:
    Thread() {}
    template<typename T>
    Thread(T thrd) {}
    Thread(void (*f)()) {}

    void swap(Thread& x) {}

    typedef boost::thread::id id;
    id get_id() const { return boost::this_thread::get_id(); }

    bool joinable() const {}
    void join() {}
    bool timed_join(const boost::system_time& wait_until) {}

    template<typename TimeDuration>
    bool timed_join(TimeDuration const& rel_time) { return true; }

    void detach() {}

    static unsigned hardware_concurrency() {}

    typedef boost::thread::native_handle_type native_handle_type;
    native_handle_type native_handle();

    void interrupt() {}
    bool interruption_requested() const { return true; }

    // backwards compatibility
    bool operator==(const Thread& other) const {}
    bool operator!=(const Thread& other) const {}

    static void yield() {}
    static void sleep(const boost::system_time& xt) {}
};
#else
typedef boost::thread Thread;
#endif

typedef boost::shared_ptr<Thread> ThreadPtr;
}}


#endif /* CORE_THREAD_H_ */
