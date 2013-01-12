#include "logger.hpp"
#include "application.hpp"
#include "time.hpp"

namespace core {
/*
 * Log Level Enum Definitions
 */
const std::string LOG_INFO_STRING = "Info";
const std::string LOG_WARNING_STRING = "Warning";
const std::string LOG_ERROR_STRING = "Error";
const std::string LOG_DEBUG_STRING = "Debug";

// Optimization for const string&, which does no copying
template<>
const std::string& enumToValue<LogLevel, const std::string&>(LogLevel eval) {
    switch(eval) {
    case LOG_DEBUG: return LOG_DEBUG_STRING;
    case LOG_INFO: return LOG_INFO_STRING;
    case LOG_WARNING: return LOG_WARNING_STRING;
    case LOG_ERROR:
    default: return LOG_ERROR_STRING;
    }
}

LogStringMap LOG_STRING_BIMAP =
        BI_MAP_BUILDER(LogLevel, std::string)
        (LOG_INFO, LOG_INFO_STRING)
        (LOG_WARNING, LOG_WARNING_STRING)
        (LOG_ERROR, LOG_ERROR_STRING)
        (LOG_DEBUG, LOG_DEBUG_STRING);

const std::string LOG_INFO_MESSAGE_STRING = "";
const std::string LOG_WARNING_MESSAGE_STRING = "[Warning]";
const std::string LOG_ERROR_MESSAGE_STRING = "[Error]";
const std::string LOG_DEBUG_MESSAGE_STRING = "[Debug]";

const std::string LOG_INFO_PRE_FORMATING = "[%time%] ";
const std::string LOG_WARNING_PRE_FORMATING = "[%time%] ";
const std::string LOG_ERROR_PRE_FORMATING = "[%time%] ";
const std::string LOG_DEBUG_PRE_FORMATING = "[%time%] (T%thread%) ";
const std::string LOG_TIME_FORMAT = "[%yy%, %MM%, %dd%, %hh%:%mm%.%ss%]";

/*
 * Checks if the application is alive still.
 */
bool TSQueueSink::appLive() {
    return !(application.lock()->checkThreadsQuiting());
}

void TSQueueSink::processQueue() {
    // Process all messages that are available
    while (!msgQueue.empty()) {
        MessageQueue::QListPtr msgList = msgQueue.dequeueAll();
        forEach(TSLevelString& msg, *msgList) {
            processMessage(msg);
        }
    }
}


/*
 * Performs all of the work required to process msgQueues
 * for incoming messages. Each such message is passed to
 * processMessage for final processing. Sink workers check
 * periodically to see if the app is still alive if no
 * messages have arrived.
 */
void TSQueueSink::sinkWorker() {
    bool healthy = true;
    while(appLive() && healthy) {
        try {
            processQueue();
            // Sleep until our message queue is notified
            // (or some time has passed)
            const boost::system_time timeout = getFuture(200);
            condLock.timedWait(timeout);
        } catch (...) {
            healthy = false;
        }
    }

    // If we had a health exit empty out all of our queues
    if (healthy) {
        processQueue();
    } else {
        // If unhealthy, quit the entire program (if not already quitting)
        application.lock()->allThreadsQuit();
    }
    application.lock()->threadManager.stopTrackingThread();
}

/*
 * Initialized the sink thread for processing messages asynchronously.
 */
threading::ThreadTrackerPtr TSQueueSink::initSinkThread(const std::string sinkName) {
    boost::function<void()> sinkFunc = boost::bind(&TSQueueSink::sinkWorker, this);
    return application.lock()->threadManager.spawnThread(sinkName, sinkFunc);
}
}
