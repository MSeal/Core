/*
 * pointers.h
 */

#ifndef POINTERS_H_
#define POINTERS_H_

#include <boost/smart_ptr.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/move/move.hpp>

namespace core {

namespace detail {
template<typename T> struct DefaultDeleter {
    void operator()(T *p) { delete p; }
};
}

/*
 * This allows for remapping the names of pointers as well
 * as give unique_ptr a default deleter. C++ doesn't allow
 * for template typedef (yet) so this is the best we can do
 * short of inheriting and avoiding non-virtual deconst
 * errors.
 *
 * To use from global scope:
 * core::pointers<T>::SharedPtr
 */
template<typename T, typename UniqueDeleter=detail::DefaultDeleter<T> >
struct pointers {
    typedef boost::shared_ptr<T> SharedPtr;
    typedef boost::weak_ptr<T> WeakPtr;
    typedef boost::intrusive_ptr<T> IntrusivePtr;
    typedef boost::scoped_ptr<T> ScopedPtr;
    // Not using std::unique_ptr because many platforms
    // don't support std::move yet -- aka any older macs.
    typedef boost::interprocess::unique_ptr<T, UniqueDeleter> UniquePtr;
    typedef std::auto_ptr<T> AutoPtr;
};

}
#endif /* POINTERS_H_ */
