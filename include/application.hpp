/*
 * application.h
 */

#ifndef CORE_APPLICATION_H_
#define CORE_APPLICATION_H_

#include "pointers.hpp"
#include "factory.hpp"
#include "logger.hpp"
#include "threading/threadTracker.hpp"

namespace core {

// Forward declarations
class Application;
typedef pointers::smart<Application>::SharedPtr ApplicationPtr;
typedef pointers::smart<Application>::WeakPtr ApplicationWPtr;

class Application : public pointers::smart<Application>::Sharable {
private:
    LoggingFactory loggingFactory;

    threading::ThreadManager threadManager;

public:
    Application() : loggingFactory(weakFromThis()),
                    threadManager(weakFromThis()) {}

    // TODO update or remove these
    bool checkThreadsQuiting() {return false;}
    void allThreadsQuit() {}
    void stopTrackingThread() {}
};

}

#endif /* CORE_APPLICATION_H_ */
