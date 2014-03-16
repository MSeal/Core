/*
 * factory.h
 *
 */
#ifndef CORE_FACTORY_H_
#define CORE_FACTORY_H_

#include <boost/ptr_container/ptr_map.hpp>
#include "exceptions.hpp"
#include "pointers.hpp"

namespace core {

namespace detail {
template<typename T>
struct DefaultBuilder {
    T *build() {
        return new T();
    }

    // Used by tracked producer
    template<typename Key>
    T *build(const Key&) {
        return new T();
    }
};
}

/*
 * A very simple interface for a factory of objects.
 * Implementing the template for a specific object
 * type will allow for quick specialization of producer.
 */
template<typename T,
         typename Builder=detail::DefaultBuilder<T>,
         typename PtrType=typename pointers::smart<T>::UniquePtr>
class Factory {
protected:
    Builder builder;

public:
    typedef PtrType TPtr;

    Factory() : builder() {}
    Factory(Builder build) : builder(build) {}
    virtual ~Factory() {}

    virtual PtrType produce() {
        return PtrType(builder.build());
    }
};


/*
 * A tracked factory, which produces new objects based on
 * a key value and tracks that key/value pair. If the same
 * key object is requested, it is returned instead of
 * producing a new object.
 *
 * Tracker type must implement find, insert, end, erase,
 * iterator, and the [] operator.
 */
template<typename Key, typename T,
         typename Builder=detail::DefaultBuilder<T>,
         typename Tracker=boost::ptr_map<const Key, T> >
class TrackedFactory {
protected:
    Tracker tracker;
    Builder builder;

public:
    typedef T* TPtr;
    typedef Tracker MapType;

    TrackedFactory() : tracker(), builder() {}
    TrackedFactory(Tracker track) : tracker(track), builder() {}
    TrackedFactory(Builder build) : tracker(), builder(build) {}
    TrackedFactory(Tracker track, Builder build) : tracker(track), builder(build) {}
    virtual ~TrackedFactory() {}

    /*
     * Throws ParameterException upon failure to insert.
     */
    virtual TPtr produce(const Key& key) {
        T *elem = builder.build(key);
        std::pair<typename MapType::iterator, bool> result = tracker.insert(key, elem);
        if (!result.second) {
            throwInsertFailedException("Unable to insert element into tracker");
        }
        return TPtr(elem);
    }

    /*
     * Returns an empty shared pointer if there is no element
     * with that key.
     *
     * Throws ParameterException if the key is not in the tracker.
     */
    TPtr get(const Key& key) {
        typename Tracker::iterator it = tracker.find(key);
        if (it == tracker.end()) {
            throwAttributeException("Key not found in Factory");
        }
        return TPtr(it->second);
    }

    TPtr tryGet(const Key& key) {
        typename Tracker::iterator it = tracker.find(key);
        if (it == tracker.end()) {
            return TPtr(NULL);
        } else {
            return TPtr(it->second);
        }
    }

    TPtr getOrProduce(const Key& key) {
        TPtr elem = tryGet(key);
        if (!elem) {
            return produce(key);
        }
        return elem;
    }

    /*
     * Drops an element from the tracker. A new getOrProduce
     * request will create a new instance of the object type.
     */
    TPtr drop(const Key& key) {
        TPtr elem = tryGet(key);
        if (!elem) {
            throwAttributeException("Key not found in Factory");
        }
        tracker.erase(key);
        return elem;
    }

    /*
     * Attempts to drops an element from the tracker. Returns
     * empty pointer on failure.
     */
    TPtr tryDrop(const Key& key) {
        TPtr elem = tryGet(key);
        if (elem) {
            tracker.erase(key);
        }
        return elem;
    }
};

//TODO locked version of factory

}

#endif /* CORE_FACTORY_H_ */
