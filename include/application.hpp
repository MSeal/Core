/*
 * application.h
 */

#ifndef CORE_APPLICATION_H_
#define CORE_APPLICATION_H_

#include "factory.hpp"
#include "logger.hpp"
#include "threading/threadTracker.hpp"

namespace core {

class Application {
private:
    LoggingFactory loggingFactory;
    threading::ThreadManager threadManager;

public:
    Application() : loggingFactory(this),  threadManager(this) {}

    // TODO update or remove these
    bool checkThreadsQuiting() {return false;}
    void allThreadsQuit() {}
    void stopTrackingThread() {}
};

}

#endif /* CORE_APPLICATION_H_ */
