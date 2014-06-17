/*
 * threadTracker.h
 * Defines a singleton which keeps information on all threads.
 * This information can be accessed globally and in a thread-safe
 * manner.
 */

#ifndef THREAD_TRACKER_H_
#define THREAD_TRACKER_H_

#include "threading/thread.hpp"
#include "threading/container/tsvector.hpp"

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {

// Forward delcaration
class Application;
typedef pointers::smart<Application>::SharedPtr ApplicationPtr;
typedef pointers::smart<Application>::WeakPtr ApplicationWPtr;

namespace threading {

class ThreadTracker {
public:
    const std::string name;
    const boost::uint32_t number;
    const ThreadPtr thread;
    const ApplicationWPtr application;

    ThreadTracker(const std::string& thrdName, boost::uint32_t thrdNumber,
            ThreadPtr thrd, ApplicationWPtr app) :
        name(thrdName), number(thrdNumber), thread(thrd), application(app) {}
};
typedef boost::shared_ptr<ThreadTracker> ThreadTrackerPtr;

/*
 * The thread manager tracks and stores references to all
 * threads. This allows easy lookup and processing of
 * thread specific information. It also allows for threads
 * to easily check if the program is quitting.
 */
class ThreadManager {
private:
    // Yes, this will be expensive to operate on -- but we need shared semantics
    typedef container::TSVector<ThreadTrackerPtr> ThreadVector;
    typedef boost::shared_ptr<ThreadVector> ThreadVectorPtr;
    typedef ThreadVector::WrapperType ThreadVectorType;
    typedef ThreadVector::LockedWrapper LockedVectorPtr;
    typedef ThreadVector::ReadLockedWrapper ReadLockedVectorPtr;

    ThreadVector threads;
    volatile boost::uint32_t curThreadCount;
    volatile boost::uint32_t quitIndicator;

    ApplicationWPtr application;

    // Helper function, need the declaration in header...
    ThreadTrackerPtr stopTrackingThreadImpl(boost::function1<bool, ThreadTrackerPtr> checkFound);

public:
    ThreadManager(ApplicationWPtr app);
    ~ThreadManager() {}

    /*
     * Start tracking a given thread
     */
    ThreadTrackerPtr trackThread(const std::string& name, ThreadPtr thrd);

    /*
     * Spawns a new thread from the provided worker function that can
     * be tracked by the global thread tracker.
     */
    ThreadTrackerPtr spawnThread(const std::string& name, void(*worker)());
    ThreadTrackerPtr spawnThread(const std::string& name, boost::function0<void> worker);

    /*
     * Functions that provide various ways of retrieving thread objects.
     * Returns the first thread which matches.
     */
    ThreadTrackerPtr getThread(const std::string & name);
    ThreadTrackerPtr getThread(const boost::thread::id id);

    boost::uint32_t getThreadNum(ThreadTrackerPtr thread);

    /*
     * various ways of track stopping (for example, if the thread has
     * finished executing) the functions return a shared pointer to the
     * ThreadTracker (or an empty pointer if the object didn't exist)
     */
    ThreadTrackerPtr stopTrackingThread(const std::string& name);
    ThreadTrackerPtr stopTrackingThread(
            const boost::thread::id id = boost::this_thread::get_id());

    /*
     * Tells all threads that listen to the tracker that the
     * program is ending
     */
    void quitThreads();

    /*
     * A check for if the program is ending (used by threads)
     */
    bool checkQuiting();
};

}}
#endif /* THREAD_TRACKER_H_ */
