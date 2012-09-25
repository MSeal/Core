/*
 * threadTracker.h
 * Defines a singleton which keeps information on all threads.
 * This information can be accessed globally and in a thread-safe
 * manner.
 */

#ifndef THREAD_TRACKER_H_
#define THREAD_TRACKER_H_

#include "threading/thread.h"
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include "threading/container/tsvector.h"

namespace core {

// Forward delcaration
class Application;

namespace threading {

class ThreadTracker {
public:
	const std::string name;
	const boost::uint32_t number;
	const ThreadPtr thread;

	ThreadTracker(const std::string& thrdName, boost::uint32_t thrdNumber, ThreadPtr thrd) :
		name(thrdName), number(thrdNumber), thread(thrd) {}
};
typedef boost::shared_ptr<ThreadTracker> ThreadTrackerPtr;

/*
 * The thread manager tracks and stores references to all
 * threads. This allows easy lookup and processing of
 * thread specific information. It also allows for threads
 * to easily check if the program is quitting.
 */
class ThreadManager {
	/* To allow our protected constructor to be called */
	friend class core::Application;
private:
	typedef container::TSVector<ThreadTrackerPtr> ThreadVector;
	typedef boost::shared_ptr<ThreadVector> ThreadVectorPtr;
	typedef ThreadVector::WrapperType ThreadVectorType;
	typedef ThreadVector::LockedWrapper LockedVectorPtr;
	typedef ThreadVector::ReadLockedWrapper ReadLockedVectorPtr;

	ThreadVector threads;
	volatile boost::uint32_t curThreadCount;
	volatile boost::uint32_t quitIndicator;

	core::Application *const application;

protected:
	/* Singleton constructor */
	ThreadManager(Application *app);

public:
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
	ThreadTrackerPtr spawnThread(const std::string& name, boost::function<void()> worker);

	/*
	 * Functions that provide various ways of retrieving thread objects
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
	ThreadTrackerPtr stopTrackingThread(const boost::thread::id id = boost::this_thread::get_id());

	/*
	 * Tells all threads that listen to the tracker that the
	 * program is ending
	 */
	void quit();

	/*
	 * A check for if the program is ending (used by threads)
	 */
	bool quiting();
};

}}
#endif /* THREAD_TRACKER_H_ */
