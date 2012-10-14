/*
 * conditionLockable.h
 */

#ifndef CONDITIONLOCKABLE_H_
#define CONDITIONLOCKABLE_H_

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include "lockable.hpp"

namespace core { namespace threading {

class ConditionLock;
class ConditionLockProxy;
class ReadWriteConditionLock;
class ReadWriteConditionLockProxy;

/*
 * An abstract wrapper on the condition type that combines
 * the locking and condition characteristics into a single
 * object.
 * Implementing classes must provide the locking mechanism.
 */
class Condition {
	friend class boost::detail::basic_condition_variable;
	friend class boost::condition_variable_any;

protected:
	typedef boost::condition_variable_any ConditionVariable;

	/*
	 * These must be implemented by any Condition classes. We didn't
	 * inherit from Lockable to avoid multiple inheritance issues.
	 * The only part of Lockable that required is these three methods.
	 */
	virtual void lock() = 0;
	virtual bool try_lock() = 0;
	virtual void unlock() = 0;

public:
	/*
	 * Used to retrieve the condition variable directly.
	 */
	virtual ConditionVariable& getConditionVariable() = 0;

	Condition() {}
	virtual ~Condition() {}

	/*
	 * Wrappers on the condition calls with the lock component
	 * written into each method. This prevents accidental mismatches
	 * between what's locked and what' being conditioned on.
	 */
	void notify_one() {
		getConditionVariable().notify_one();
	}
	void notify_all() {
		getConditionVariable().notify_all();
	}

	void wait() {
		getConditionVariable().wait(*this);
	}

	template<typename predicate_type>
	void wait(predicate_type predicate) {
		getConditionVariable().wait(*this, predicate);
	}

	bool timed_wait(boost::system_time const& abs_time) {
		return getConditionVariable().timed_wait(*this, abs_time);
	}

	template<typename duration_type>
	bool timed_wait(duration_type const& rel_time) {
		return getConditionVariable().timed_wait(*this, rel_time);
	}

	template<typename predicate_type>
	bool timed_wait(boost::system_time const& abs_time,predicate_type predicate) {
		return getConditionVariable().timed_wait(*this, abs_time, predicate);
	}

	template<typename duration_type,typename predicate_type>
	bool timed_wait(duration_type const& rel_time, predicate_type predicate) {
		return getConditionVariable().timed_wait(*this, rel_time, predicate);
	}

	// backwards compatibility
	bool timed_wait(boost::xtime const& abs_time) {
		return getConditionVariable().timed_wait(*this, abs_time);
	}

	template<typename predicate_type>
	bool timed_wait(boost::xtime const& abs_time,predicate_type predicate) {
		return getConditionVariable().timed_wait(*this, abs_time, predicate);
	}
};

/*
 * An object which holds a shared pointer to a LockedReference
 * and allows for pointer style operations.
 */
template<typename Ref>
class LockedConditionReferencePtr : public LockedReferencePtr<Ref> {
public:
	typedef ConditionLock LockType;

	LockedConditionReferencePtr(boost::shared_ptr<Ref> reference,
			boost::shared_ptr<LockType> lockPtr) :
		LockedReferencePtr<Ref>(reference, lockPtr){}

	virtual ~LockedConditionReferencePtr() {}

	Condition& getCondition() {
		/*
		 * We know this will always be a ConditionLock, because it had
		 * to be so in the constructor. Thus it's safe to static cast
		 * this to a Condition.
		 */
		return *(boost::static_pointer_cast<LockType, Lockable>(this->getLockablePtr()));
	}
};

/*
 * An object which holds a shared pointer to a LockedReference
 * and allows for pointer style operations.
 */
template<typename Ref>
class LockedConditionReadableReferencePtr : public LockedReferencePtr<Ref> {
public:
	typedef ReadWriteConditionLock LockType;

	LockedConditionReadableReferencePtr(boost::shared_ptr<Ref> reference,
			boost::shared_ptr<LockType> lockPtr) :
		LockedReferencePtr<Ref>(reference, lockPtr){}

	virtual ~LockedConditionReadableReferencePtr() {}

	Condition& getCondition() {
		/*
		 * We know this will always be a ConditionLock, because it had
		 * to be so in the constructor. Thus it's safe to static cast
		 * this to a Condition.
		 */
		return *(boost::static_pointer_cast<LockType, Lockable>(this->getLockablePtr()));
	}
};

/*
 * Wraps a condition variable with a lock. This allows for lock ownership
 * of conditions and creates an easy way to get a locked object with quick
 * access to it's associated condition variable.
 */
class ConditionLock : public WriteLock, public Condition {
protected:
	ConditionVariable condition;

	/*
	 * Creates a locked reference for which this lockable is locked
	 * until the locked reference goes out of scope (deconstructs).
	 */
	template<typename Ref> LockedConditionReferencePtr<Ref>
	generateLockedConditionReference(boost::shared_ptr<Ref> reference) {
		return LockedConditionReferencePtr<Ref>(reference, castLock<ConditionLock>());
	}

public:
	/*
	 * Used to retrieve the condition variable directly.
	 */
	ConditionVariable& getConditionVariable() {
		return condition;
	}

	explicit ConditionLock(int priority = 0) :
		WriteLock(priority), Condition() {}

	virtual ~ConditionLock() {}

	void lock() { WriteLock::lock(); }
	bool try_lock() { return WriteLock::try_lock(); }
	void unlock() { WriteLock::unlock(); }
};

/*
 * Acts as a ConditionLock by proxy reference. Any Lockable calls
 * or Condition calls get forwarded to the proxy instead, so the
 * proxy appears as a Lockable without needing to implement it's
 * locking protocol.
 */
class ConditionLockProxy : public LockableProxy, public Condition {
protected:
	/*
	 * Creates a locked reference for which this lockable is locked
	 * until the locked reference goes out of scope (deconstructs).
	 */
	template<typename Ref> LockedConditionReferencePtr<Ref>
	generateLockedConditionReference(boost::shared_ptr<Ref> reference) {
		return LockedConditionReferencePtr<Ref>(reference, castLock<ConditionLock>());
	}

public:
	/*
	 * Used to retrieve the condition variable directly.
	 */
	ConditionVariable& getConditionVariable() {
		return castLock<ConditionLock>()->getConditionVariable();
	}

	explicit ConditionLockProxy(boost::shared_ptr<ConditionLock> lReference) :
		LockableProxy(lReference), Condition() {}

	virtual ~ConditionLockProxy() {}

	void lock() { LockableProxy::lock(); }
	bool try_lock() { return LockableProxy::try_lock(); }
	void unlock() { LockableProxy::unlock(); }
};

/*
 * Wraps a condition variable with a lock. This allows for lock ownership
 * of conditions and creates an easy way to get a locked object with quick
 * access to it's associated condition variable.
 */
class ReadWriteConditionLock : public ReadWriteLock, public Condition {
protected:
	ConditionVariable condition;

	/*
	 * Creates a locked reference for which this lockable is locked
	 * until the locked reference goes out of scope (deconstructs).
	 */
	template<typename Ref> LockedConditionReadableReferencePtr<Ref>
	generateLockedConditionReference(boost::shared_ptr<Ref> reference) {
		return LockedConditionReadableReferencePtr<Ref>(reference, castLock<ReadWriteConditionLock>());
	}

public:
	/*
	 * Used to retrieve the condition variable directly.
	 */
	ConditionVariable& getConditionVariable() {
		return condition;
	}

	explicit ReadWriteConditionLock(int priority = 0) :
			ReadWriteLock(priority), Condition() {}

	virtual ~ReadWriteConditionLock() {}

	void lock() { ReadWriteLock::lock(); }
	bool try_lock() { return ReadWriteLock::try_lock(); }
	void unlock() { ReadWriteLock::unlock(); }
};

/*
 * Acts as a ReadWriteConditionLock by proxy reference. Any
 * ReadWriteLockable calls or Condition calls get forwarded to
 * the proxy instead, so the proxy appears as a ReadWriteLockable
 * without needing to implement it's locking protocol.
 */
class ReadWriteConditionLockProxy : public ReadWriteLockableProxy, public Condition {
protected:
	/*
	 * Creates a locked reference for which this lockable is locked
	 * until the locked reference goes out of scope (deconstructs).
	 */
	template<typename Ref> LockedConditionReadableReferencePtr<Ref>
	generateLockedConditionReference(boost::shared_ptr<Ref> reference) {
		return LockedConditionReadableReferencePtr<Ref>(reference, castLock<ReadWriteConditionLock>());
	}

public:
	/*
	 * Used to retrieve the condition variable directly.
	 */
	ConditionVariable& getConditionVariable() {
		return castLock<ReadWriteConditionLock>()->getConditionVariable();
	}

	explicit ReadWriteConditionLockProxy(boost::shared_ptr<ReadWriteConditionLock> lReference) :
		ReadWriteLockableProxy(lReference), Condition() {}

	virtual ~ReadWriteConditionLockProxy() {}

	void lock() { ReadWriteLockableProxy::lock(); }
	bool try_lock() { return ReadWriteLockableProxy::try_lock(); }
	void unlock() { ReadWriteLockableProxy::unlock(); }
};

}}
#endif /* CONDITIONLOCKABLE_H_ */
