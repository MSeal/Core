/*
 * uniquePtrList.h
 * This defines an object which wraps boost::ptr_list but returns unique
 * pointers on all retrieve requests. Since the functionality is so close
 * to boost::ptr_list, the function names use boost's underscore naming
 * conventions.
 */

#ifndef UNIQUEPTRLIST_H_
#define UNIQUEPTRLIST_H_

#include <boost/ptr_container/ptr_list.hpp>
#include "detail/boostPointers.hpp"

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
    typedef typename pointers::detail::smartBoost<UPtrListType>::AutoPtr AutoPtrUniqueListType;
    typedef typename pointers::detail::smartBoost<UPtrListType>::UniquePtr UniquePtrUniqueListType;
    typedef boost::ptr_list<T, CloneAllocator, Allocator> PtrListType;
    typedef typename pointers::detail::smartBoost<PtrListType>::AutoPtr AutoPtrListType;
    typedef typename pointers::detail::smartBoost<PtrListType>::UniquePtr UniquePtrListType;

    struct TDeleter {
        void operator()(T *p) { delete p; }
    };

public:

    typedef typename ::core::pointers::detail::smartBoost<T>::UniquePtr UniquePtr;

    UniquePtrList() {}
    explicit UniquePtrList(const Allocator& a) : PtrListType(a) {}

    // AutoPtr lists
    explicit UniquePtrList(AutoPtrUniqueListType r) : PtrListType(r) {}
    UniquePtrList& operator=(AutoPtrUniqueListType r) {
        PtrListType::operator=(r);
        return *this;
    }
    explicit UniquePtrList(AutoPtrListType r) : PtrListType(r) {}
    UniquePtrList& operator=(AutoPtrListType r) {
        PtrListType::operator=(r);
        return *this;
    }

    // UniquePtr lists
    explicit UniquePtrList(UniquePtrUniqueListType r) : PtrListType(r) {}
    UniquePtrList& operator=(UniquePtrUniqueListType r) {
        PtrListType::operator=(*r);
        return *this;
    }
    explicit UniquePtrList(UniquePtrListType r) : PtrListType(r) {}
    UniquePtrList& operator=(UniquePtrListType r) {
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
    void push_front(UniquePtr x) {
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
