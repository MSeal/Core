/*
 * pointers.hpp
 * Defines and consolidates various pointer types and pointer containers
 * into a single namespace. Most of these classes are predefined and used
 * by their original names elsewhere. Thus this files serves primarily for
 * convenience.
 *
 * Since C++ doesn't allow for nice partial templating (shake angry fist)
 * we have to resort to struct templating to achieve our goal -- which
 * functionally is identical to our intended goal, except we have to apply
 * the template arguments to the struct and not the renamed object.
 *
 * So instead of:
 *  core::pointers::smart::SharedPtr<T>
 * We must do:
 *  core::pointers::smart<T>::SharedPtr
 *
 * Also, this makes typedefs using smart<T>::SharedPtr to specify typename
 * before the compiler will be happy.
 *
 * Thus instead of:
 *  typedef core::pointers::smart::SharedPtr<MyType> MyTypePtr;
 * The compiler expects:
 *  typedef typename core::pointers::smart::SharedPtr<MyType> MyTypePtr;
 *
 * Also note that many of the definitions for the pointer types are hidden in
 * 'detail/boostPointers.hpp'. This is to avoid circular dependency issues.
 * The original typedef mappings are commented above each of the new mappings.
 */

#ifndef POINTERS_H_
#define POINTERS_H_

#include <boost/move/move.hpp>
#include "detail/boostPointers.hpp"
#include "container/uniquePtrList.hpp"

namespace core { namespace pointers {

// Forward declaration
namespace detail { template<typename T> class UntrackedPtr; }

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
struct smart {
    //typedef boost::shared_ptr<T> SharedPtr;
    typedef typename detail::smartBoost<T>::SharedPtr SharedPtr;
    //typedef boost::enable_shared_from_this<T> Sharable;
    typedef typename detail::Sharable<T> Sharable;
    //typedef boost::weak_ptr<T> WeakPtr;
    typedef typename detail::smartBoost<T>::WeakPtr WeakPtr;
    //typedef boost::intrusive_ptr<T> IntrusivePtr;
    typedef typename detail::smartBoost<T>::IntrusivePtr IntrusivePtr;
    //typedef boost::scoped_ptr<T> ScopedPtr;
    typedef typename detail::smartBoost<T>::ScopedPtr ScopedPtr;
    // Not using std::unique_ptr because many platforms
    // don't support std::move yet -- aka any older macs.
    //typedef boost::interprocess::unique_ptr<T, UniqueDeleter> UniquePtr;
    typedef typename detail::smartBoost<T, UniqueDeleter>::UniquePtr UniquePtr;
    //typedef std::auto_ptr<T> AutoPtr;
    typedef typename detail::smartBoost<T>::AutoPtr AutoPtr;
    // typedef here for the sake of same syntax capabilities
    typedef typename detail::UntrackedPtr<T> UntrackedPtr;
};

/*
 * Simple renaming of boost pointer lists. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename T,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct lists {
    //typedef boost::ptr_vector<T, CloneAllocator, Allocator> PtrVector;
    typedef typename detail::listsBoost<T, CloneAllocator, Allocator>::PtrVector PtrVector;
    //typedef boost::ptr_list<T, CloneAllocator, Allocator> PtrList;
    typedef typename detail::listsBoost<T, CloneAllocator, Allocator>::PtrList PtrList;
    //typedef boost::ptr_deque<T, CloneAllocator, Allocator> PtrDeque;
    typedef typename detail::listsBoost<T, CloneAllocator, Allocator>::PtrDeque PtrDeque;

    // Use our UniquePtrList
    typedef container::UniquePtrList<T, CloneAllocator, Allocator> UniquePtrList;
};

/*
 * Simple renaming of boost pointer arrays. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename T,
         unsigned int ArraySize,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct arrays {
    //typedef boost::ptr_array<T, ArraySize, CloneAllocator> PtrArray;
    typedef typename detail::arraysBoost<T, ArraySize, CloneAllocator, Allocator>::PtrArray PtrArray;
};

/*
 * Simple renaming of boost pointer sets. These are here simply to
 * consolidate the location of each, and to provide a consistent interface.
 */
template<typename Key,
         typename Compare=std::less<Key>,
         typename CloneAllocator=boost::heap_clone_allocator,
         typename Allocator=std::allocator<void*> >
struct sets {
    //typedef boost::ptr_set<Key, Compare, CloneAllocator, Allocator> PtrSet;
    typedef typename detail::setsBoost<Key, Compare, CloneAllocator, Allocator>::PtrSet PtrSet;
    //typedef boost::ptr_multiset<Key, Compare, CloneAllocator, Allocator> PtrMultiSet;
    typedef typename detail::setsBoost<Key, Compare, CloneAllocator, Allocator>::PtrMultiSet PtrMultiSet;
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
struct maps {
    //typedef boost::ptr_map<Key, T, Compare, CloneAllocator, Allocator> PtrMap;
    typedef typename detail::mapsBoost<Key, T, Compare, CloneAllocator, Allocator>::PtrMap PtrMap;
    //typedef boost::ptr_multimap<Key, T, Compare, CloneAllocator, Allocator> PtrMultiMap;
    typedef typename detail::mapsBoost<Key, T, Compare, CloneAllocator, Allocator>::PtrMultiMap PtrMultiMap;
};

namespace detail {
/*
 * A pointer which is owned elsewhere and used as a reference. Since this wraps
 * a pointer it can be NULL -- unlike a reference -- but is not tracked in anyway.
 * The obfuscation also makes deleting the underlying pointer accidentally
 * impossible.
 */
template<typename T>
class UntrackedPtr {
private:
    T *uptr;
public:
    UntrackedPtr(T *ptr) : uptr(ptr) {}
    UntrackedPtr(const UntrackedPtr<T>& other) : uptr(other.uptr) {}

    void swap(UntrackedPtr& other) {
        T *tmp = uptr;
        other.uptr = uptr;
        uptr = tmp;
    }

    UntrackedPtr& operator=(const UntrackedPtr<T>& other) {
        uptr = other.uptr;
        return *this;
    }

    T* operator->() const { return uptr; }
    T& operator*() const { return *uptr; }

    T *get() const { return uptr; }

    operator bool() const { return uptr; }
    bool operator !() const { return !uptr; }

    template<typename S>
    bool operator ==(S other) const { return uptr == other; }
    template<typename S>
    bool operator !=(S other) const { return uptr != other; }
};
}

}}
#endif /* POINTERS_H_ */
