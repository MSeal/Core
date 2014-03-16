/*
 * TSWrapper.h
 * Defines threadsafe wrappers for generic classes
 */

#ifndef TSWRAPPER_H_
#define TSWRAPPER_H_

#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "threading/lockable.hpp"
#include "threading/conditionLockable.hpp"
#include "pointers.hpp"

namespace core { namespace threading { namespace container {

namespace hidden {
/*
 * A hidden nested internal class which wraps several objects
 * to allow constructor ordering.
 *
 * Not to be used outside of this file.
 */
template<typename Wrapped, typename Mutex>
class LockableWrappedContents {
protected:
    typename pointers::smart<Wrapped>::SharedPtr wrapped;
    typename pointers::smart<Mutex>::SharedPtr mutex;

public:
    explicit LockableWrappedContents(int priority) :
        wrapped(new Wrapped()), mutex(new Mutex(priority)) {}
    explicit LockableWrappedContents(Wrapped& other, int priority) :
        wrapped(new Wrapped(other)), mutex(new Mutex(priority)) {}
};
}

/*
 * A wrapper on a particular object which combines the locking
 * and ownership of an object into a single class. To work on the
 * class use the locked reference from generateLockedReference()
 * which is scope locked for the duration of the lockedReference
 * lifetime.
 *
 * Inheritance ordering matters here!
 */
template<typename Wrapped>
class TSWrapper : public hidden::LockableWrappedContents<Wrapped, ConditionLock>,
    public ConditionLockProxy {
private:
    /*
     * There is inheritance from a Lockable class so this class
     * is non-copyable. This is intended since our lock is tied
     * to our data and we cannot guarantee thread safety when
     * swapping the internal pointer -- so the full copy operator
     * is forbidden.
     *
     * Subclasses are allowed to implement Copy constructors (as
     * long as they lock the input data structure), but the
     * assignment operator will most certainly not be thread
     * safe with the provided data structure.
     */

public:
    // For public use in identifying the underlying wrapped class
    typedef Wrapped WrapperType;
    typedef LockedConditionReferencePtr<Wrapped> LockedWrapper;
    typedef ConditionLock LockType;
    typedef ConditionLock Condition;
    typedef boost::interprocess::scoped_lock<LockType> ScopedLock;

    explicit TSWrapper(int priority = 0) :
        hidden::LockableWrappedContents<Wrapped, LockType>(priority),
        ConditionLockProxy(this->mutex) {}

    /*
     * Creates a locked reference of the wrapped object. The object
     * stays locked until the locked wrapper goes out of scope.
     */
    LockedWrapper generateLockedConditionReference() {
        return ConditionLockProxy::generateLockedConditionReference(this->wrapped);
    }

    /*
     * Gives the condition variable for this wrapper without any
     * lock or unlock calls.
     */
    Condition& getCondition() {
        return *(this->mutex);
    }

    /*
     * Gets the lock for this wrapper.
     */
    LockType& getMutex() const {
        return *(this->mutex);
    }
};

/*
 * A wrapper on a particular object which combines the locking
 * and ownership of an object into a single class. To work on the
 * class use the locked reference from generateLockedReference()
 * which is scope locked for the duration of the lockedReference
 * lifetime. This Wrapper support read locked references.
 */
template <typename Wrapped>
class TSReadWriteWrapper : public hidden::LockableWrappedContents<Wrapped, ReadWriteConditionLock>,
    public ReadWriteConditionLockProxy {
private:
    /*
     * Since our lock is tied to our data, we cannot guarantee
     * thread safety when swapping the internal pointer -- so
     * the full copy operator is forbidden. The copy constructor
     * is fine though, because we know no objects are making other
     * calls during construction.
     */
    TSReadWriteWrapper<Wrapped>& operator =(const TSReadWriteWrapper<Wrapped>& other) {
        return *this;
    }

public:
    // For public use in identifying the underlying wrapped class
    typedef Wrapped WrapperType;
    typedef LockedConditionReadableReferencePtr<Wrapped> LockedWrapper;
    typedef ReadLockedReferencePtr<Wrapped> ReadLockedWrapper;
    typedef ReadWriteConditionLock LockType;
    typedef ReadWriteConditionLock Condition;
    typedef boost::interprocess::scoped_lock<LockType> ScopedLock;
    typedef boost::shared_lock<LockType> SharedScopedLock;

    explicit TSReadWriteWrapper(int priority = 0) :
        hidden::LockableWrappedContents<Wrapped, LockType>(priority),
        ReadWriteConditionLockProxy(this->mutex) {}

    /*
     * Creates a locked reference of the wrapped object. The object
     * stays locked until the locked wrapper goes out of scope.
     */
    LockedWrapper generateLockedReference() {
        return ReadWriteConditionLockProxy::generateLockedConditionReference<Wrapped>(this->wrapped);
    }

    ReadLockedWrapper generateReadLockedReference() {
        return ReadWriteConditionLockProxy::generateReadLockedReference<Wrapped>(this->wrapped);
    }
    /*
     * Gives the condition variable for this wrapper without any
     * lock or unlock calls.
     */
    Condition& getCondition() {
        return *(this->mutex);
    }

    /*
     * Gets the lock for this wrapper.
     */
    LockType& getMutex() const {
        return *(this->mutex);
    }
};

}}}

#endif /* TSWRAPPER_H_ */
