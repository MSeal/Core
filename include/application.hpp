/*
 * application.hpp
 */

#ifndef CORE_APPLICATION_H_
#define CORE_APPLICATION_H_

#include "pointers.hpp"
#include "factory.hpp"
#include "logger.hpp"
#include "threading/thread_tracker.hpp"

namespace core {
class Application : public pointers::smart<Application>::Sharable {
public:
    LoggingFactory loggingFactory;
    threading::ThreadManager threadManager;

    Application() : loggingFactory(weakFromThis()),
                    threadManager(weakFromThis()) {}

    /*
     * Retrieves or produces a logger by name. The returned pointer
     * is effectively a raw pointer to the logger which should live
     * for the duration of Application.
     */
    LoggerPtr getLogger(const std::string& name) {
        return loggingFactory.getOrProduce(name);
    }

    /*
     * Checks if the program is quitting and threads have been ordered
     * to exit.
     */
    bool checkThreadsQuiting() {
        return threadManager.checkQuiting();
    }

    /*
     * Tells all threads/application to exit gracefully.
     */
    void allThreadsQuit() {
        threadManager.quitThreads();
    }
};

}

#endif /* CORE_APPLICATION_H_ */
