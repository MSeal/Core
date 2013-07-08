/*
 * singleton.h
 * Defines a singleton class which has can be inherited,
 * or simple used by Singleton<classname>::instance() to
 * retrieve any arbitrary singleton object.
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "pointers.hpp"

namespace core {

/*
 * Inherit this class or use Singleton<classname>::instance()
 * to get an instance of a single object. The T class must
 * implement a default constructor. If that constructor is
 * protected, it must also make Singleton<T> a friend class.
 */
template<typename T>
class Singleton : private boost::noncopyable {
    friend class pointers::smart<T>::ScopedPtr;
public:
    typedef typename pointers::smart<T>::ScopedPtr SingletonPtr;

protected:
	static SingletonPtr singletonInstance;

	/* Singleton constructor */
	Singleton() {}
	virtual ~Singleton() {}

public:
	static void reset() {
        singletonInstance.reset(new T());
    }

	static T& instance() {
		return *singletonInstance;
	}
};

template<typename T> typename Singleton<T>::SingletonPtr Singleton<T>::singletonInstance(new T());

}
#endif /* SINGLETON_H_ */
