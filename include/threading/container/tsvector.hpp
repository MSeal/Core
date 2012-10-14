/*
 * tsvector.h
 * This class creates a thread safe vector implementation.
 */

#ifndef TSVECTOR_H_
#define TSVECTOR_H_

#include <vector>
#include <boost/thread.hpp>
#include "tswrapper.hpp"

namespace core { namespace threading { namespace container {

/*
 * TSVector implements a boost::thread safe version of a vector.
 * The implementation uses a wrapped vector for all calls. A
 * limited number of base calls are available without retrieving
 * a locked reference of the vector.
 */
template <typename T, typename Alloc = std::allocator<T> >
class TSVector : public TSReadWriteWrapper<std::vector<T, Alloc> > {
private:
	// Class renaming for readability
	typedef std::vector<T, Alloc> VectorType;
	typedef boost::shared_ptr<VectorType> VectorTypePtr;
	typedef TSReadWriteWrapper<VectorType> Vector;
	typedef typename Vector::ScopedLock ScopedLock;
	typedef typename Vector::SharedScopedLock SharedScopedLock;

	// Assignment operator is not made, because all lockable objects
	// are non-copyable.

public:
	explicit TSVector(int priority = 0) : Vector(priority) {}
	explicit TSVector(VectorType& other, int priority = 0) : Vector(priority) {
		this->wrapped = VectorTypePtr(VectorType(other));
	}
	explicit TSVector(TSVector& other) : Vector(other.getPriority()) {
		other.lock();
		this->wrapped = VectorTypePtr(VectorType(other.wrapped));
		other.unlock();
	}
	explicit TSVector(TSVector& other, int priority) : Vector(priority) {
		other.lock();
		this->wrapped = VectorTypePtr(VectorType(other.wrapped));
		other.unlock();
	}
	template <typename InputIterator>
	TSVector(InputIterator first, InputIterator last, int priority = 0) : Vector(priority) {
		this->wrapped = VectorTypePtr(VectorType(first, last));
	}

	/*
	 * Ensure that all memory is deallocated and the appropiate
	 * delete calls are made.
	 */
	~TSVector() {
		clear();
	}

	/* Returns the total capacity of the vector */
	std::size_t capacity() const {
		SharedScopedLock lock(this->getMutex());
		return this->wrapped->capacity();
	}

	/* Clears all queue elements from the queue */
	void clear() {
		ScopedLock lock(this->getMutex());
		this->wrapped->clear();
	}

	/* Returns true if the queue is empty */
	bool empty() {
		SharedScopedLock lock(this->getMutex());
		return this->wrapped->empty();
	}

	/* Returns the allocator object used to construct the vector */
	Alloc get_allocator() const {
		SharedScopedLock lock(this->getMutex());
		return this->wrapped->get_allocator();
	}

	/* Returns the maximum number of elements that the wrapped vector can hold */
	std::size_t max_size() const {
		SharedScopedLock lock(this->getMutex());
		return this->wrapped->max_size();
	}

	/*
	 * Adds a new element at the end of the vector, after its current last element.
	 * The content of this new element is initialized to a copy of x.
	 */
	void push_back(const T& x) {
		ScopedLock lock(this->getMutex());
		this->wrapped->push_back(x);
	}

	/* Returns the size of the queue */
	std::size_t size() {
		SharedScopedLock lock(this->getMutex());
		return this->wrapped->size();
	}

	/*
	 * Exchanges the content of the vector by the content of vec, which is another
	 * vector of the same type. Sizes may differ.
	 *
	 * After the call to this member function, the elements in this container are
	 * those which were in vec before the call, and the elements of vec are those
	 * which were in this.
	 */
	void swap(VectorType& vec) {
		ScopedLock lock(this->getMutex());
		this->wrapped->swap(vec);
	}
};

}}}
#endif /* TSVECTOR_H_ */
