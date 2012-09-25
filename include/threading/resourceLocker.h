/*
 * resourceLocker.h
 * Defines a resource which can be incrementally locked
 * and unlocked. This is useful for abstracting a resource
 * in multi-threaded environment.
 */

#ifndef RESOURCELOCKER_H_
#define RESOURCELOCKER_H_

#include <vector>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/logic/tribool.hpp>
#include "lockable.h"

namespace core { namespace threading {

/*
 * The resource locker object defines a set of locks
 * which are all associated with a particular resource.
 * This can aid in abstracting resource management in
 * multi-threaded environments. The most common methods
 * to be used are lock and unlock, which will lock all
 * lockable objects required to avoid deadlocks on the
 * resource.
 *
 * Resource lockers are not inherently threadsafe and
 * should be used on a one per thread basis. There is
 * a provided constructor to allow for such copying.
 *
 * Note too that the locks are re-entrant, so multiple
 * attempts to lock the same lock will not result in
 * crashes or deadlocks.
 */
class ResourceLocker {
private:
	typedef boost::logic::tribool tribool;

	enum LockSetting {
		UNLOCKED,
		READ_LOCKED,
		WRITE_LOCKED
	};

	/*
	 * Nested class used for tracking locks and their
	 * current settings/configurations.
	 */
	class LockableReference {
	public:
		boost::shared_ptr<Lockable> lockable;
		LockSetting setting;
		tribool readLockByDefault;

		bool setReadLockDefault(tribool readLockByDefault) {
			bool set = true;
			if (readLockByDefault && !lockable->isReadLockable()) {
				readLockByDefault = false;
				set = false;
			}
			this->readLockByDefault = readLockByDefault;
			return set;
		}

		explicit LockableReference(boost::shared_ptr<Lockable> lockable,
				tribool readLockByDefault = tribool::indeterminate_value) {
			this->lockable = lockable;
			setting = UNLOCKED;
			setReadLockDefault(readLockByDefault);
		}

		Lockable& operator *() const {
			return *lockable;
		}

		Lockable *operator ->() const {
			return lockable.get();
		}

		bool operator <(const Lockable& b) const {
			return lockable->compareLockables(b) < 0;
		}
		bool operator <(const LockableReference& b) const {
			return lockable->compareLockables(*b) < 0;
		}

		bool operator <=(const Lockable& b) const {
			return lockable->compareLockables(b) <= 0;
		}
		bool operator <=(const LockableReference& b) const {
			return lockable->compareLockables(*b) <= 0;
		}

		bool operator >(const Lockable& b) const {
			return lockable->compareLockables(b) > 0;
		}
		bool operator >(const LockableReference& b) const {
			return lockable->compareLockables(*b) > 0;
		}

		bool operator >=(const Lockable& b) const {
			return lockable->compareLockables(b) >= 0;
		}
		bool operator >=(const LockableReference& b) const {
			return lockable->compareLockables(*b) >= 0;
		}
	};

	std::vector<LockableReference> locks;
	long nextLockIndex;

	/*
	 * Used for sorting and comparing lockable references.
	 */
	static bool LockableLargestFirstCompare(const LockableReference& first,
			const LockableReference& second) {
		return (first > second);
	}

	void writeLockMutex(LockableReference& mutref) {
		mutref.lockable->lock();
		mutref.setting = WRITE_LOCKED;
	}

	void readLockMutex(LockableReference& mutref) {
		if (mutref.lockable->isReadLockable()) {
			((ReadWriteLockable *)mutref.lockable.get())->lock_shared();
			mutref.setting = READ_LOCKED;
		} else {
			writeLockMutex(mutref);
		}
	}

	/*
	 * Locks the mutex.
	 * If asReadLockIfPossible is set to true, then the lock will try to
	 * perform a read lock instead. If this fails it will perform a
	 * write lock. If asReadLockIfPossible is not set, it will default to
	 * the mutex's default lock setting.
	 */
	void lockMutex(LockableReference& mutref,
			tribool asReadLockIfPossible = tribool::indeterminate_value) {
		if (asReadLockIfPossible == tribool::indeterminate_value) {
			asReadLockIfPossible = mutref.readLockByDefault;
		}

		if (asReadLockIfPossible) {
			readLockMutex(mutref);
		} else {
			writeLockMutex(mutref);
		}
	}

	void unlockMutex(LockableReference& mutref) {
		if (mutref.setting == WRITE_LOCKED) {
			mutref.setting = UNLOCKED;
			mutref.lockable->unlock();
		} else if (mutref.setting == READ_LOCKED) {
			mutref.setting = UNLOCKED;
			((ReadWriteLockable *)mutref.lockable.get())->unlock_shared();
		}
		// Otherwise do nothing
	}

	/*
	 * Locks the next sequentially significant lock.
	 * If asReadLockIfPossible is set to true, then the lock will try to
	 * perform a read lock instead. If this fails it will perform a
	 * write lock. If asReadLockIfPossible is not set, it will default to
	 * the mutex's default lock setting.
	 */
	void lockNext(tribool asReadLockIfPossible = tribool::indeterminate_value) {
		if (nextLockIndex < (long)locks.size()) {
			lockMutex(locks[nextLockIndex], asReadLockIfPossible);
			nextLockIndex++;
		}
	}

	/*
	 * Unlock the next sequentially significant lock
	 */
	void unlockLastLocked() {
		if (nextLockIndex > 0) {
			nextLockIndex--;
			unlockMutex(locks[nextLockIndex]);
		}
	}

	/*
	 * Sorts the locks such that they lock in the correct ordering
	 */
	void sortLocks() {
		std::sort(locks.begin(), locks.end(), LockableLargestFirstCompare);
	}

public:
	/*
	 * Returns the number of locks associated with this resource.
	 */
	long getNumLocks() const {
		return locks.size();
	}

	/*
	 * Gets the lock position among all of the resource's locks.
	 * This position can change if a new lock is added to the
	 * resource.
	 * Returns -1 if the lock is not among this resource's locks.
	 */
	long getLockPosition(boost::shared_ptr<Lockable> lock) {
		// Could store hash entries for this, but it's probably not worth the trouble
		for (long index = 0; index < (long)locks.size(); index++) {
			if (locks[index].lockable == lock) {
				return index;
			}
		}
		return -1;
	}

	/*
	 * Checks if the lock is contained among the locks of this resource
	 */
	bool containsLock(boost::shared_ptr<Lockable> lock) {
		return (getLockPosition(lock) >= 0);
	}

	/*
	 * Gets the lock at a specified position.
	 */
	boost::shared_ptr<Lockable> getLock(long position) const {
		if (position < (long)locks.size()) {
			return locks[position].lockable;
		} else {
			return boost::shared_ptr<Lockable>();
		}
	}

	/*
	 * Returns true if any locks are active.
	 */
	bool hasActiveLock() const {
		return nextLockIndex != 0;
	}

	/*
	 * Returns the current lock position for this resource.
	 * This is the last lock that was locked.
	 * Returns -1 if no locks are active.
	 */
	int getCurrentLockPosition() const {
		return nextLockIndex-1;
	}

	/*
	 * Returns true if the specified locks for this resource is actively
	 * locked.
	 * Note does not know about other resource's shared locks.
	 */
	bool isLocked(long position) {
		return position < nextLockIndex;
	}
	bool isLocked(boost::shared_ptr<Lockable> lock) {
		return isLocked(getLockPosition(lock));
	}

	/*
	 * Returns true if all of the locks for this resource are actively
	 * locked.
	 * Note does not know about other resource's shared locks.
	 */
	bool allLocked() {
		return nextLockIndex == (long)locks.size();
	}

	/*
	 * Returns true if none of the locks for this resource are actively
	 * locked.
	 * Note does not know about other resource's shared locks.
	 */
	bool noneLocked() {
		return nextLockIndex == 0;
	}

	/*
	 * Locks all locks through to the input lock/position.
	 * If asReadLockIfPossible is set to true, then the lock will try to
	 * perform a read lock instead. If this fails it will perform a
	 * write lock. If asReadLockIfPossible is not set, it will default to
	 * the mutex's default lock setting.
	 */
	void lock(long position = -1, tribool asReadLockIfPossible = tribool::indeterminate_value) {
		if (position < 0) position = getNumLocks();
		if (position < (long)locks.size()) {
			while(!isLocked(position)) {
				lockNext(asReadLockIfPossible);
			}
		}
	}
	void lock(boost::shared_ptr<Lockable> lock, tribool asReadLockIfPossible = tribool::indeterminate_value) {
		long lockPos = getLockPosition(lock);
		if (lockPos >= 0 && !isLocked(lockPos)) {
			this->lock(lockPos, asReadLockIfPossible);
		}
	}

	/*
	 * Unlocks all locks through to the input lock/position.
	 */
	void unlock(long position = 0) {
		if (position >= 0 && position < (long)locks.size()) {
			while(isLocked(position)) {
				unlockLastLocked();
			}
		}
	}
	void unlock(boost::shared_ptr<Lockable> lock) {
		long lockPos = getLockPosition(lock);
		if (lockPos >= 0 && isLocked(lockPos)) {
			unlock(lockPos);
		}
	}

	/*
	 * Unlocks any read locks (and locks after those read locks) and
	 * relocks each as a write lock.
	 * Note this does release the original lock temporarily.
	 *
	 * Since boost upgrade locks only allow a single thread to have
	 * upgrade status, it can produce large and unexpected bottlenecks
	 * if many readers are write capable without first releasing their
	 * own lock. Thus this resource class doesn't allow for this
	 * behavior and instead forces the coder to release a lock before
	 * upgrading.
	 */
	void relockAllReadsAsWrites() {
		bool readLockFound = false;
		long oldNextLockIndex = nextLockIndex;
		for (long index = 0; index < oldNextLockIndex; index++) {
			if (locks[index].setting == READ_LOCKED) {
				unlock(index);
				readLockFound = true;
				break;
			}
		}
		// If we unlocked some locks
		if (readLockFound) {
			// Lock all as write locks
			// oldNextLockIndex can't be less than 1 or readLockFound
			// would be false (it won't crash if it is less than 1 though).
			lock(oldNextLockIndex-1, false);
		}
	}

	/*
	 * Sets the default action of a lock request for a specified
	 * lock
	 */
	void setLockDefaultType(long position,
			tribool readLockByDefault = tribool::indeterminate_value) {
		if (position >= 0 && position < (long)locks.size()) {
			locks[position].setReadLockDefault(readLockByDefault);
		}
	}
	void setLockDefaultType(boost::shared_ptr<Lockable> lock,
			tribool readLockByDefault = tribool::indeterminate_value) {
		setLockDefaultType(getLockPosition(lock));
	}
	void setAllLockDefaultTypes(tribool readLockByDefault = tribool::indeterminate_value) {
		for (int index = 0; index < (long)locks.size(); index++) {
			locks[index].setReadLockDefault(readLockByDefault);
		}
	}

	/*
	 * Adds a lockable object to the resource.
	 * Releases all locks held by the resource to avoid deadlocking issues.
	 * Avoiding adding locks after resource begins locking.
	 */
	void addLockable(const boost::shared_ptr<Lockable> lock,
			tribool defaultAsReadLock = tribool::indeterminate_value) {
		// Make sure we're given an actual lock and not an empty pointer
		if (lock) {
			LockableReference lockRef(lock, defaultAsReadLock);
			bool needSorting = false;
			if (!locks.empty() && LockableLargestFirstCompare(lockRef, locks[locks.size()-1])) {
				needSorting = true;
			}
			// Unlock all locks
			unlock();
			locks.push_back(lockRef);
			if (needSorting) {
				sortLocks();
			}
		}
	}

	/*
	 * Adds all of the locks from another resource to this resource's
	 * lock pool.
	 * All locks are unlocked on this resource (but not on the other's)
	 * before the operation runs. This is to avoid deadlocks and unwanted
	 * lock acquisitions.
	 * Avoiding adding locks after resource begins locking.
	 */
	void addResourceLockable(const ResourceLocker& other) {
		unlock();
		for (long index = 0; index < (long)other.locks.size(); index++) {
			locks.push_back(other.locks[index]);
			// Our resource doesn't have any locks active yet
			locks[locks.size()-1].setting = UNLOCKED;
		}
		sortLocks();
	}

	/*
	 * Constructors for resource lockers.
	 */
	explicit ResourceLocker() {
		nextLockIndex = 0;
	}

	explicit ResourceLocker(std::vector<boost::shared_ptr<Lockable> >& lockVect) {
		nextLockIndex = 0;
		for (long index = 0; index < (long)lockVect.size(); index++) {
			locks.push_back(LockableReference(lockVect[index]));
		}
		sortLocks();
	}

	explicit ResourceLocker(const ResourceLocker& other) {
		nextLockIndex = 0;
		addResourceLockable(other);
	}
};

}}
#endif /* RESOURCELOCKER_H_ */
