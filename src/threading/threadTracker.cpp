#include "threading/threadTracker.hpp"
#include "application.hpp"
#include <boost/interprocess/detail/atomic.hpp>

namespace core { namespace threading {

namespace atomic = boost::interprocess::ipcdetail;

/* Private constructor */
ThreadManager::ThreadManager(Application *app) : threads(), curThreadCount(0),
                                                 quitIndicator(0), application(app) {}

/*
 * Start tracking a given thread
 */
ThreadTrackerPtr ThreadManager::trackThread(const std::string& name, ThreadPtr thrd) {
	ThreadTrackerPtr track = ThreadTrackerPtr
		(new ThreadTracker(name, atomic::atomic_inc32(&curThreadCount), thrd));
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
ThreadTrackerPtr ThreadManager::spawnThread(const std::string& name, boost::function<void()> worker) {
	return trackThread(name, ThreadPtr(new Thread(worker)));
}

/*
 * various ways of track stopping (for example, if the thread has
 * finished executing) the functions return a shared pointer to the
 * ThreadTracker (or an empty pointer if the object didn't exist)
 */
ThreadTrackerPtr ThreadManager::stopTrackingThread(const std::string& name) {
	bool found = false;
	// This gives us a scope locked wrapper on our vector
	LockedVectorPtr lockedThreads = threads.generateLockedReference();
	ThreadVectorType::iterator trackerIter = lockedThreads->begin(), endIter = lockedThreads->end();
	ThreadTrackerPtr thd;
	while(trackerIter != endIter) {
		if (name.compare((*trackerIter)->name) == 0) {
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

ThreadTrackerPtr ThreadManager::stopTrackingThread(const boost::thread::id id) {
	bool found = false;
	// This gives us a locked wrapper on our vector
	LockedVectorPtr lockedThreads = threads.generateLockedReference();
	ThreadVectorType::iterator trackerIter = lockedThreads->begin(), endIter = lockedThreads->end();
	ThreadTrackerPtr thd;
	while(trackerIter != endIter) {
		if ((*trackerIter)->thread->get_id() == id) {
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
 * Tells all threads that listen to the tracker that the
 * program is ending
 */
void ThreadManager::quit() {
	atomic::atomic_inc32(&quitIndicator);
}

/*
 * A check for if the program is ending (used by threads)
 */
bool ThreadManager::quiting() {
	return atomic::atomic_read32(&quitIndicator) != 0;
}
}}
