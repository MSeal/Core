/**
 * lockable.h
 *
 * Defines several objects which can be locked and unlocked
 */

#ifndef LOCKABLE_H_
#define LOCKABLE_H_

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "pointers.hpp"

namespace core { namespace threading {

/* Forward declaration of lockable types */
class Lockable;
typedef typename pointers::smart<Lockable>::SharedPtr LockablePtr;
typedef typename pointers::smart<const Lockable>::SharedPtr ConstLockablePtr;
class ReadWriteLockable;
typedef typename pointers::smart<ReadWriteLockable>::SharedPtr ReadWriteLockablePtr;
typedef typename pointers::smart<const ReadWriteLockable>::SharedPtr ConstReadWriteLockablePtr;

template<typename Ref> class LockedReferencePtr;
template<typename Ref> class ReadLockedReferencePtr;

/**
 * Used to create a locked reference to an object. The mutex
 * is locked while in scope and dereference calls return the
 * reference object.
 */
template<typename Ref>
class LockedReference : private boost::noncopyable {
	friend class LockedReferencePtr<Ref>;

private:
	typedef typename pointers::smart<Ref>::SharedPtr RefPtr;

	LockablePtr lockable;
	RefPtr lockedRef;

	/* Define these after we've defined Lockable */
	void lock();
	void unlock();

public:
	LockedReference(RefPtr reference, LockablePtr lockPtr) :
		lockable(lockPtr), lockedRef(reference) {
		lock();
	}

	Ref& operator *() const {
		return *lockedRef;
	}

	Ref *operator ->() const {
		return lockedRef.get();
	}

	Ref *get() const {
		return lockedRef.get();
	}

	Lockable& getLock() const {
		return *lockable;
	}

	virtual ~LockedReference() {
		unlock();
	}
};

/**
 * An object which holds a shared pointer to a LockedReference
 * and allows for pointer style operations.
 */
template<typename Ref>
class LockedReferencePtr {
private:
    typedef typename pointers::smart<Ref>::SharedPtr RefPtr;
    typedef typename pointers::smart<LockedReference<Ref> >::SharedPtr SharedLockedReferencePtr;

    SharedLockedReferencePtr ptr;

protected:
	LockablePtr getLockablePtr() { return ptr->lockable; }
	RefPtr getReferencePtr() { return ptr->lockedRef; }

public:
	LockedReferencePtr(RefPtr reference, LockablePtr lockPtr) :
		ptr(new LockedReference<Ref>(reference, lockPtr)) {}

	explicit LockedReferencePtr(const LockedReferencePtr& lockedReference) :
		ptr(lockedReference.ptr) {}

	virtual ~LockedReferencePtr() {}

	Ref& operator *() const {
		return **ptr;
	}

	Ref *operator ->() const {
		return ptr->get();
	}

	Ref *get() const {
		return ptr->get();
	}

	Lockable& getLock() const {
		return ptr->getLock();
	}
};

/**
 * Used to create a locked reference to an object. The mutex
 * is locked while in scope and dereference calls return the
 * reference object.
 */
template<typename Ref>
class ReadLockedReference : private boost::noncopyable {
	friend class ReadLockedReferencePtr<Ref>;

private:
	typedef typename pointers::smart<Ref>::SharedPtr RefPtr;
	typedef typename pointers::smart<ReadLockedReference<Ref> >::SharedPtr ReadLockedReferencePtr;

	ReadWriteLockablePtr lockable;
	RefPtr lockedRef;

	/* Define these after we've defined ReadWriteLockable */
	void lock();
	void unlock();

public:
	ReadLockedReference(RefPtr reference, ReadLockedReferencePtr lockPtr) :
		lockable(lockPtr), lockedRef(reference) {
		lock();
	}

	Ref& operator *() const {
		return *lockedRef;
	}

	Ref *operator ->() const {
		return lockedRef.get();
	}

	Ref *get() const {
		return lockedRef.get();
	}

	ReadWriteLockable& getLock() const {
		return *lockable;
	}

	virtual ~ReadLockedReference() {
		unlock();
	}
};

/**
 * An object which holds a shared pointer to a ReadLockedReference
 * and allows for pointer style operations.
 */
template<typename Ref>
class ReadLockedReferencePtr {
private:
    typedef typename pointers::smart<Ref>::SharedPtr RefPtr;
    typedef typename pointers::smart<ReadLockedReference<Ref> >::SharedPtr SharedReadLockedReferencePtr;

    SharedReadLockedReferencePtr ptr;

protected:
    LockablePtr getLockablePtr() { return ptr->lockable; }
    RefPtr getReferencePtr() { return ptr->lockedRef; }

	/*
	 * Used to wrap the Lockable pointer cast.
	 */
	template<typename LockType>
	boost::shared_ptr<LockType> castLock() {
		return boost::static_pointer_cast<LockType, Lockable>(ptr->lockable);
	}

public:
	ReadLockedReferencePtr(RefPtr reference, LockablePtr lockPtr) :
		ptr(new ReadLockedReference<Ref>(reference, lockPtr)) {}

	explicit ReadLockedReferencePtr(const ReadLockedReferencePtr& lockedReference) :
		ptr(lockedReference.ptr) {}

	virtual ~ReadLockedReferencePtr() {}

	Ref& operator *() const {
		return **ptr;
	}

	Ref *operator ->() const {
		return ptr->get();
	}

	Ref *get() const {
		return ptr->get();
	}

	ReadWriteLockable& getLock() const {
		return ptr->getLock();
	}
};

/**
 * Compares two mutexes and returns an indicator for which
 * mutex should be locked first.
 * A positive value indicates that the first mutex should
 * be locked first, a negative that it should be locked second.
 * The comparison function is deterministic and unchanging
 * between call on the same mutexes
 *
 * @param firstPriority The priority of the first mutex over the second.
 * @param secondPriority The priority of the first mutex over the second.
 * @return A positive value if the first mutex is considered 'greater'
 *         than the second mutex. Priority overrides the standard comparison
 *         metrics.
 */
template<typename FirstMutex, typename SecondMutex>
static int compareMutexes(const FirstMutex& first, const SecondMutex& second,
		int firstPriority = 0, int secondPriority = 0) {
	if (firstPriority == secondPriority) {
		if ((&first) - (&second) > 0) return 1;
		else if ((&first) - (&second) < 0) return -1;
	} else {
		return firstPriority - secondPriority;
	}
	return 0;
}

/**
 * An abstract class for lockable objects, this provides
 * certain interfacing guarantees for lock management classes.
 */
class Lockable : public boost::enable_shared_from_this<Lockable>, private boost::noncopyable {
private:
	const int mutexPriority;
	const bool readLockable;

protected:
	/*
	 * Creates a locked reference for which this lockable is locked
	 * until the locked reference goes out of scope (deconstructs).
	 */
	template<typename Ref>
	LockedReferencePtr<Ref> generateLockedReference(typename pointers::smart<Ref>::SharedPtr reference) {
		return LockedReferencePtr<Ref>(reference, getLockablePtr());
	}

public:
	explicit Lockable(int priority = 0, bool readLock = false) :
		mutexPriority(priority), readLockable(readLock) {}

	virtual void lock() = 0;
	virtual bool tryLock() = 0;
	virtual void unlock() = 0;

	virtual void lockSharedIfPossible() { lock(); }
	virtual bool tryLockSharedIfPossible() { return tryLock(); }
	virtual void unlockSharedIfPossible() { unlock(); }

	virtual bool isReadLockable() const {
		return readLockable;
	}
	virtual int getPriority() const {
		return mutexPriority;
	}

	/*
	 * These four methods allow a reference Lockable to act as
	 * this object's lockable by proxy.
	 */
	virtual LockablePtr getLockablePtr() {
		return getLockable().shared_from_this();
	}
	virtual ConstLockablePtr getLockablePtr() const {
		return getLockable().shared_from_this();
	}
	virtual Lockable& getLockable() {
		return *this;
	}
	virtual const Lockable& getLockable() const {
		return *this;
	}

	/*
	 * Used to wrap the Lockable pointer cast.
	 */
	template<typename LockType>
	typename pointers::smart<LockType>::SharedPtr castLock() {
		return boost::static_pointer_cast<LockType, Lockable>(getLockablePtr());
	}

	/**
	 * Non-blocking comparison between a lockable and a(any)
	 * mutex to determine the locking order.
	 */
	template<typename T>
	int compareLockables(const T& other) const {
		return compareMutexes(getLockable(), other, getPriority());
	}

	/**
	 * Non-blocking comparison between two lockables to
	 * determine the locking order.
	 */
	int compareLockables(const Lockable& other) const {
		return compareMutexes(getLockable(), other.getLockable(), getPriority(), other.getPriority());
	}

	template<typename T>
	bool operator <(const T& other) const {
		return compareLockables(other) < 0;
	}
	template<typename T>
	bool operator <=(const T& other) const {
		return compareLockables(other) <= 0;
	}
	template<typename T>
	bool operator >(const T& other) const {
		return compareLockables(other) > 0;
	}
	template<typename T>
	bool operator >=(const T& other) const {
		return compareLockables(other) >= 0;
	}

	virtual ~Lockable() {}
};

/**
 * Acts as a Lockable by proxy reference. Any Lockable calls
 * get forwarded to the proxy instead, so the LockableProxy
 * appears as a Lockable without needing to implement it's
 * locking protocol.
 */
class LockableProxy : public Lockable {
protected:
	LockablePtr lockReference;

public:
	explicit LockableProxy(LockablePtr lReference) :
		Lockable(lReference->getPriority(), lReference->isReadLockable()),
		lockReference(lReference) {}

	virtual ~LockableProxy() {}

	virtual void lock() {
		lockReference->lock();
	}
	virtual bool tryLock() {
		return lockReference->tryLock();
	}
	virtual void unlock() {
		lockReference->unlock();
	}

	/*
	 * These four methods allow a reference Lockable to act as
	 * this object's lockable by proxy.
	 */
	virtual LockablePtr getLockablePtr() {
		return lockReference;
	}
	virtual ConstLockablePtr getLockablePtr() const {
		return lockReference;
	}
	virtual Lockable& getLockable() {
		return *lockReference;
	}
	virtual const Lockable& getLockable() const {
		return *lockReference;
	}
};

/**
 * Extends the Lockable abstract class to include read (shared)
 * lock operators.
 */
class ReadWriteLockable : public Lockable {
protected:
	/**
	 * Creates a read locked reference for which this lockable is locked
	 * until the locked reference goes out of scope (deconstructs).
	 */
	template<typename Ref>
	ReadLockedReferencePtr<Ref> generateReadLockedReference(typename pointers::smart<Ref>::SharedPtr reference) {
		return ReadLockedReferencePtr<Ref>(reference, getLockablePtr());
	}

public:
	explicit ReadWriteLockable(int priority = 0) : Lockable(priority, true) {}

	virtual void lockShared() = 0;
	virtual bool tryLockShared() = 0;
	virtual void unlockShared() = 0;

	virtual void lockSharedIfPossible() { lockShared(); }
	virtual bool tryLockSharedIfPossible() { return tryLockShared(); }
	virtual void unlockSharedIfPossible() { unlockShared(); }

	virtual ~ReadWriteLockable() {}
};

/**
 * Acts as a ReadWriteLockable by proxy reference. Any
 * ReadWriteLockable calls get forwarded to the proxy
 * instead, so the ReadWriteLockableProxy appears as a
 * ReadWriteLockable without needing to implement it's
 * locking protocol.
 */
class ReadWriteLockableProxy : public ReadWriteLockable {
private:
	const ReadWriteLockablePtr lockReference;

public:
	explicit ReadWriteLockableProxy(ReadWriteLockablePtr lReference) :
		ReadWriteLockable(lReference->getPriority()), lockReference(lReference) {}

	virtual ~ReadWriteLockableProxy() {}

	virtual void lock() {
		lockReference->lock();
	}
	virtual bool tryLock() {
		return lockReference->tryLock();
	}
	virtual void unlock() {
		lockReference->unlock();
	}

	virtual void lockShared() {
		lockReference->lockShared();
	}
	virtual bool tryLockShared() {
		return lockReference->tryLockShared();
	}
	virtual void unlockShared() {
		lockReference->unlockShared();
	}

	/**
	 * These four methods allow a reference Lockable to act as
	 * this object's lockable by proxy.
	 */
	virtual LockablePtr getLockablePtr() {
		return lockReference;
	}
	virtual ConstLockablePtr getLockablePtr() const {
		return lockReference;
	}
	virtual Lockable& getLockable() {
		return *lockReference;
	}
	virtual const Lockable& getLockable() const {
		return *lockReference;
	}
};

/**
 * Locks two lockable objects in the correct order. The lockables can
 * be Lockable or ReadWriteLockable in any ordering.
 */
template <typename T, typename U>
static void lockMutexes(T& first, U& second,
		                bool readLockFirst = false,
		                bool readLockSecond = false) {
	int lockFirst = compareMutexes(first, second, first.getPriority(), second.getPriority());
	if (lockFirst > 0) {
		readLockFirst ? first.lockSharedIfPossible() : first.lock();
		readLockSecond ? second.lockSharedIfPossible() : second.lock();
	} else if (lockFirst < 0) {
		readLockSecond ? second.lockSharedIfPossible() : second.lock();
		readLockFirst ? first.lockSharedIfPossible() : first.lock();
	} else {
		// We were asked to lock the same lock twice...
		(readLockFirst && readLockSecond) ? first.lockSharedIfPossible() : first.lock();
	}
}
template <typename T, typename U>
static void lockMutexes(T& first, boost::shared_ptr<U> second) {
	lockMutexes(first, *second);
}
template <typename T, typename U>
static void lockMutexes(boost::shared_ptr<T> first, U& second) {
	lockMutexes(*first, second);
}
template <typename T, typename U>
static void lockMutexes(boost::shared_ptr<T> first, boost::shared_ptr<U> second) {
	lockMutexes(*first, *second);
}

/**
 * Unlocks two lockable objects. The lockables can
 * be Lockable or ReadWriteLockable in any ordering.
 */
template <typename T, typename U>
static void unlockMutexes(T& first, U& second,
		bool readLockFirst = false, bool readLockSecond = false) {
	if ((&first) != (&second)) {
		readLockFirst ? first.unlockSharedIfPossible() : first.unlock();
		readLockSecond ? second.unlockSharedIfPossible() : second.unlock();
	} else {
		// We were asked to unlock the same lock twice...
		(readLockFirst && readLockSecond) ? first.unlockSharedIfPossible() : first.unlock();
	}
}
template <typename T, typename U>
static void unlockMutexes(T& first, boost::shared_ptr<U> second) {
	unlockMutexes(first, *second);
}
template <typename T, typename U>
static void unlockMutexes(boost::shared_ptr<T> first, U& second) {
	unlockMutexes(*first, second);
}
template <typename T, typename U>
static void unlockMutexes(boost::shared_ptr<T> first, boost::shared_ptr<U> second) {
	unlockMutexes(*first, *second);
}

/**
 * Define the lock and unlock methods after lockable has been
 * defined for the templated LockedReference classes. This avoids
 * ugly circular dependencies with two separate header files.
 */
template <class Ref> void LockedReference<Ref>::lock() {
	lockable->lock();
}
template <class Ref> void LockedReference<Ref>::unlock() {
	lockable->unlock();
}
template <class Ref> void ReadLockedReference<Ref>::lock() {
	lockable->lockShared();
}
template <class Ref> void ReadLockedReference<Ref>::unlock() {
	lockable->unlockShared();
}

/**
 * Write only Mutex
 */
class WriteLock : public Lockable {
private:
	boost::mutex mutex;
	explicit WriteLock(const WriteLock& mutex) : Lockable(), mutex() {}
public:
	explicit WriteLock(int priority = 0) : Lockable(priority), mutex() {}
	virtual ~WriteLock() {}

	void lock() { mutex.lock(); }
	bool tryLock() { return mutex.try_lock(); }
	void unlock() { mutex.unlock(); }
};

/**
 * Read/Write Mutex
 */
class ReadWriteLock : public ReadWriteLockable {
private:
	boost::shared_mutex mutex;
	explicit ReadWriteLock(const ReadWriteLock& mutex) : ReadWriteLockable(), mutex() {}

public:
	explicit ReadWriteLock(int priority = 0) : ReadWriteLockable(priority), mutex() {}
	virtual ~ReadWriteLock() {}

	void lock() { mutex.lock(); }
	bool tryLock() { return mutex.try_lock(); }
	void unlock() { mutex.unlock(); }

	void lockShared() { mutex.lock_shared(); }
	bool tryLockShared() { return mutex.try_lock_shared(); }
	void unlockShared() { mutex.unlock_shared(); }
};

}}
#endif /* LOCKABLE_H_ */
