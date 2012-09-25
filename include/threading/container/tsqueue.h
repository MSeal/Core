/* 
 * tsqueue.h
 * This class creates a thread safe queue implementation.
 */

#ifndef TS_QUEUE_H_
#define TS_QUEUE_H_

#include "container/uniquePtrList.h"
#include "tswrapper.h"
#include "pointers.h"

namespace core { namespace threading { namespace container {

/*
 * TSQueue implements a boost::thread safe version of a basic queue. 
 * The implementation uses linked lists to create an easy use queue.
 */
template <typename T,
          typename CloneAllocator = boost::heap_clone_allocator,
          typename Allocator = std::allocator<void*> >
class TSQueue : public TSWrapper<core::container::UniquePtrList<T, CloneAllocator, Allocator> > {
private:
	// Class renaming for readability
	typedef TSWrapper<core::container::UniquePtrList<T, CloneAllocator, Allocator> > Queue;
	typedef typename Queue::ScopedLock ScopedLock;

protected:
	CloneAllocator cloner;

public:
	typedef T ElemType;
	typedef typename pointers<T>::UniquePtr PtrType;
	typedef core::container::UniquePtrList<T, CloneAllocator, Allocator> QList;
	typedef typename pointers<QList>::SharedPtr QListPtr;
	explicit TSQueue(int priority = 0) : Queue(priority), cloner() {}

	/*
	 * Ensure that all memory is deallocated and the appropiate
	 * delete calls are made.
	 */
	~TSQueue() {
		clear();
	}

	/* Returns true if the queue is empty */
	bool empty() {
		ScopedLock lock(this->getMutex());
		return this->wrapped->empty();
	}

	/* Returns the size of the queue */
	std::size_t size() {
		ScopedLock lock(this->getMutex());
		return this->wrapped->size();
	}

	/* Clears all queue elements from the queue */
	void clear() {
		ScopedLock lock(this->getMutex());
		this->wrapped->clear();
	}

	/*
	 * Enqueues a new element into the queue. This element can
	 * be pulled back out of the queue by dequeuing all of the
	 * elements off the front of the queue.
	 */
	template<typename U>
	void enqueue(const U& enq) {
	    ScopedLock lock(this->getMutex());
	    this->wrapped->push_back(cloner.allocate_clone(enq));
	}

    void enqueue(T *const enq) {
        ScopedLock lock(this->getMutex());
        this->wrapped->push_back(enq);
    }

    void enqueue(const PtrType enq) {
        ScopedLock lock(this->getMutex());
        this->wrapped->push_back(enq);
    }

    void enqueue(const typename pointers<T>::AutoPtr enq) {
        ScopedLock lock(this->getMutex());
        this->wrapped->push_back(enq);
    }

	/*
	 * Dequeues an element off of the front of the queue.
	 */
	PtrType dequeue() {
		ScopedLock lock(this->getMutex());
		// can't dequeue from an empty queue
		if (!this->wrapped->empty()) {
		    return this->wrapped->pop_front();
		}
		return PtrType();
	}

	/*
	 * Dequeues up to N items from the queue at once and returns
	 * a list containing all of those items.
	 */
	QListPtr dequeueN(std::size_t numDequeue) {
		QListPtr deq;

		ScopedLock lock(this->getMutex());
		numDequeue = std::min(numDequeue, this->wrapped->size());
		// Taking all elements?
		if (numDequeue == this->wrapped->size()) {
		    // Just give our whole QListPtr
		    deq = this->wrapped;
            this->wrapped = QListPtr(new QList());
		}
		// Taking some of our elements
		else {
		    deq = QListPtr(new QList());
            for (std::size_t i = 0; i < numDequeue; i++) {
                deq->push_back(this->wrapped->pop_front());
            }
		}
		return deq;
	}

	/*
	 * Dequeues up to all items from the queue at once and returns
	 * a list containing all of those items. This has very little
	 * locking overhead.
	 */
	QListPtr dequeueAll() {
	    ScopedLock lock(this->getMutex());
		QListPtr oldQueue = this->wrapped;
		this->wrapped = QListPtr(new QList());
		return oldQueue;
	}
};
}}}

#endif
