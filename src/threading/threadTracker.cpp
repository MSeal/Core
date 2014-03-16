#include "threading/threadTracker.hpp"
#include "application.hpp"
#include <boost/interprocess/detail/atomic.hpp>

namespace core { namespace threading {

namespace atomic = boost::interprocess::ipcdetail;

/* Private constructor */
ThreadManager::ThreadManager(ApplicationWPtr app) : threads(), curThreadCount(0),
                                                    quitIndicator(0), application(app) {}

/*
 * Start tracking a given thread
 */
ThreadTrackerPtr ThreadManager::trackThread(const std::string& name, ThreadPtr thrd) {
    ThreadTrackerPtr track = ThreadTrackerPtr
        (new ThreadTracker(name, atomic::atomic_inc32(&curThreadCount),
                           thrd, application));
    threads.push_back(track);
    return track;
}

/*
 * Spawns a new thread from the provided worker function that can
 * be tracked by the global thread tracker.
 */
ThreadTrackerPtr ThreadManager::spawnThread(const std::string& name, void(*worker)())  {
    return trackThread(name, ThreadPtr(new Thread(worker)));
}
ThreadTrackerPtr ThreadManager::spawnThread(const std::string& name, boost::function0<void> worker) {
    return trackThread(name, ThreadPtr(new Thread(worker)));
}

ThreadTrackerPtr ThreadManager::stopTrackingThreadImpl(
        boost::function1<bool, ThreadTrackerPtr> checkFound) {
    bool found = false;
    // This gives us a scope locked wrapper on our vector
    LockedVectorPtr lockedThreads = threads.generateLockedReference();
    // Need to expose the iterators so we can erase the item at the end
    ThreadVectorType::iterator trackerIter = lockedThreads->begin();
    ThreadTrackerPtr thd;
    while(trackerIter != lockedThreads->end()) {
        if (checkFound(*trackerIter)) {
            found = true;
            thd = *trackerIter;
            break;
        }
        trackerIter++;
    }
    if (found) {
        lockedThreads->erase(trackerIter);
    }
    return thd;
}

/*
 * Helper functions which define the comparison process for stopping by
 * name or id.
 */
bool stopTrackingByName(ThreadTrackerPtr tptr, const std::string& name) {
    return name.compare(tptr->name) == 0;
}
bool stopTrackingById(ThreadTrackerPtr tptr, const boost::thread::id id) {
    return tptr->thread->get_id() == id;
}

/*
 * Various ways of track stopping (for example, if the thread has
 * finished executing) the functions return a shared pointer to the
 * ThreadTracker (or an empty pointer if the object didn't exist)
 */
ThreadTrackerPtr ThreadManager::stopTrackingThread(const std::string& name) {
    boost::function1<bool, ThreadTrackerPtr> checkFound =
            boost::bind(stopTrackingByName, _1, boost::cref(name));
    return stopTrackingThreadImpl(checkFound);
}
ThreadTrackerPtr ThreadManager::stopTrackingThread(const boost::thread::id id) {
    boost::function1<bool, ThreadTrackerPtr> checkFound =
            boost::bind(stopTrackingById, _1, id);
    return stopTrackingThreadImpl(checkFound);
}

/*
 * Tells all threads that listen to the tracker that the program is
 * ending.
 */
void ThreadManager::quitThreads() {
    atomic::atomic_inc32(&quitIndicator);
}

/*
 * A check for if the program is ending (used by threads).
 */
bool ThreadManager::checkQuiting() {
    return atomic::atomic_read32(&quitIndicator) != 0;
}
}}
