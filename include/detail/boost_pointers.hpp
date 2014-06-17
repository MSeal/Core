/*
 * boostPointers.hpp
 */

#ifndef POINTERS_BOOSTED_H_
#define POINTERS_BOOSTED_H_

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/smart_ptr.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/move/move.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core { namespace pointers { namespace detail {
template<typename T> struct DefaultDeleter {
    void operator()(T *p) { delete p; }
};

/*
 * This allows for remapping the names of pointers as well
 * as give unique_ptr a default deleter. C++ doesn't allow
 * for template typedef (yet) so this is the best we can do
 * short of inheriting and avoiding non-virtual deconst
 * errors.
 *
 * To use from global scope:
 * core::pointers::smart<T>::SharedPtr
 */
template<typename T, typename UniqueDeleter=detail::DefaultDeleter<T> >
struct smartBoost {
    typedef boost::shared_ptr<T> SharedPtr;
    typedef boost::weak_ptr<T> WeakPtr;
    typedef boost::intrusive_ptr<T> IntrusivePtr;
    typedef boost::scoped_ptr<T> ScopedPtr;
    // Not using std::unique_ptr because many platforms
    // don't support std::move yet -- aka any older macs.
    typedef boost::interprocess::unique_ptr<T, UniqueDeleter> UniquePtr;
    // Not technically a boost ptr...
    typedef std::auto_ptr<T> AutoPtr;
};

/*
 * Name wrapper on enable_shared_from_this with additional weak
 * pointer capabilities.
 */
template<typename T>
class Sharable : public boost::enable_shared_from_this<T> {
public:
    typename smartBoost<T>::SharedPtr sharedFromThis() {
        return this->shared_from_this();
    }
    typename smartBoost<T>::WeakPtr weakFromThis() {
        return typename smartBoost<T>::WeakPtr(sharedFromThis());
    }
};

/*
 * Simple renaming of boost pointer lists. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename T,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct listsBoost {
    typedef boost::ptr_vector<T, CloneAllocator, Allocator> PtrVector;
    typedef boost::ptr_list<T, CloneAllocator, Allocator> PtrList;
    typedef boost::ptr_deque<T, CloneAllocator, Allocator> PtrDeque;
};

/*
 * Simple renaming of boost pointer arrays. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename T,
         unsigned int ArraySize,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct arraysBoost {
    typedef boost::ptr_array<T, ArraySize, CloneAllocator> PtrArray;
};

/*
 * Simple renaming of boost pointer sets. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename Key,
         typename Compare=std::less<Key>,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct setsBoost {
    typedef boost::ptr_set<Key, Compare, CloneAllocator, Allocator> PtrSet;
    typedef boost::ptr_multiset<Key, Compare, CloneAllocator, Allocator> PtrMultiSet;
};

/*
 * Simple renaming of boost pointer maps. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename Key,
         typename T,
         typename Compare=std::less<Key>,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct mapsBoost {
    typedef boost::ptr_map<Key, T, Compare, CloneAllocator, Allocator> PtrMap;
    typedef boost::ptr_multimap<Key, T, Compare, CloneAllocator, Allocator> PtrMultiMap;
};

}}}
#endif /* POINTERS_BOOSTED_H_ */
