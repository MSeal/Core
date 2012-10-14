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
    Factory() : builder() {}
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
    typedef Tracker MapType;

    TrackedFactory() : tracker(), builder() {}
    virtual ~TrackedFactory() {}

    /*
     * Throws ParameterException upon failure to insert.
     */
    virtual T& produce(const Key& key) {
        T *elem = builder.build(key);
        std::pair<typename MapType::iterator, bool> result = tracker.insert(key, elem);
        if (!result.second) {
            throwInsertFailedException("Unable to insert element into tracker");
        }
        return *elem;
    }

    /*
     * Returns an empty shared pointer if there is no element
     * with that key.
     *
     * Throws ParameterException if the key is not in the tracker.
     */
    T& get(const Key& key) {
        typename Tracker::iterator it = tracker.find(key);
        if (it == tracker.end()) {
            throw throwAttributeException("Key not found in Factory");
        }
        return *it;
    }

    T *tryGet(const Key& key) {
        typename Tracker::iterator it = tracker.find(key);
        if (it == tracker.end()) {
            return NULL;
        } else {
            return &(*it);
        }
    }

    T& getOrProduce(const Key& key) {
        T *elem = tryGet(key);
        if (elem == NULL) {
            return produce(key);
        }
        return *elem;
    }

    /*
     * Drops an element from the tracker. A new getOrProduce
     * request will create a new instance of the object type.
     */
    void drop(const Key& key) {
        tracker.erase(key);
    }
};

//TODO locked version of factory

}

#endif /* CORE_FACTORY_H_ */
