/*
 * uniquePtrList.h
 */

#ifndef UNIQUEPTRLIST_H_
#define UNIQUEPTRLIST_H_

#include <boost/ptr_container/ptr_list.hpp>
#include "pointers.h"

namespace core { namespace container {

/*
 * Acts like a boost::ptr_list except it wraps the output
 * into a unique_ptr from auto_ptr.
 */
template<class T,
         class CloneAllocator = boost::heap_clone_allocator,
         class Allocator = std::allocator<void*> >
class UniquePtrList : public boost::ptr_list<T, CloneAllocator, Allocator> {
protected:
    typedef UniquePtrList<T, CloneAllocator, Allocator> UPtrListType;
    typedef boost::ptr_list<T, CloneAllocator, Allocator> PtrListType;

public:
    typedef typename pointers<T>::UniquePtr UniquePtr;

    UniquePtrList() {}
    explicit UniquePtrList(const Allocator& a) : PtrListType(a) {}

    // AutoPtr lists
    explicit UniquePtrList(typename pointers<UPtrListType>::AutoPtr r) : PtrListType(r) {}
    UniquePtrList& operator=(typename pointers<UPtrListType>::AutoPtr r) {
        PtrListType::operator=(r);
        return *this;
    }
    explicit UniquePtrList(typename pointers<PtrListType>::AutoPtr r) : PtrListType(r) {}
    UniquePtrList& operator=(typename pointers<PtrListType>::AutoPtr r) {
        PtrListType::operator=(r);
        return *this;
    }

    // UniquePtr lists
    explicit UniquePtrList(typename pointers<UPtrListType>::UniquePtr r) : PtrListType(r) {}
    UniquePtrList& operator=(typename pointers<UPtrListType>::UniquePtr r) {
        PtrListType::operator=(*r);
        return *this;
    }
    explicit UniquePtrList(typename pointers<PtrListType>::UniquePtr r) : PtrListType(r) {}
    UniquePtrList& operator=(typename pointers<PtrListType>::UniquePtr r) {
        PtrListType::operator=(*r);
        return *this;
    }

    template<class U>
    UniquePtrList(const boost::ptr_list<U>& r) : PtrListType(r) {}
    template<class U>
    UniquePtrList(const UniquePtrList<U>& r) : PtrListType(r) {}

    template<class U>
    UniquePtrList& operator=(const boost::ptr_list<U> r) {
      this->swap(r);
      return *this;
    }
    template<class U>
    UniquePtrList& operator=(const UniquePtrList<U> r) {
      this->swap(r);
      return *this;
    }

    void merge(UniquePtrList& x) {
        PtrListType::merge(x);
    }

    template< typename Compare >
    void merge(UniquePtrList& x, Compare comp) {
        PtrListType::merge(x, comp);
    }

    /*
     * Add unique_ptr push_front interface.
     */
    template<class U>
    void push_front(typename pointers<U>::UniquePtr x) {
        PtrListType::push_front(x.release());
    }

    /*
     * Convert the pop_front from auto_ptr to unique_ptr.
     *
     * Throws: bad_ptr_container_operation if empty() == true
     */
    UniquePtr pop_front() throw() {
        return UniquePtr(PtrListType::pop_front().release());
    }
};

// Clonability
template< typename T, typename CA, typename A >
inline UniquePtrList<T,CA,A>* new_clone(const UniquePtrList<T,CA,A>& r) {
    return r.clone().release();
}

}}


#endif /* UNIQUEPTRLIST_H_ */
