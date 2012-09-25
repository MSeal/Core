#include "logger.h"
#include "application.h"

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
 * logging Worker
 * The logging worker is a thread function which processes and print all
 * queued logs.
 * These are filled by functions as they experience errors or wish to express
 * warnings/information.
 */
bool TSQueueSink::processQueue() {
	unsigned int queued = 0;
	try {
        queued = 0;
        MessageQueue::QListPtr messagesPtr = msgQueue.dequeueAll();
        MessageQueue::QList& messages = *messagesPtr;
        forEach(TSLevelString& msg, messages) {
            processMessage(msg);
        }
	} catch (...) {
		return false;
	}
	return true;
}

void TSQueueSink::sinkWorker() {
	bool healthy = true;
	// Keep working until the program quits
	while(!application->checkThreadsQuiting() && healthy) {
		healthy = processQueue();
		// TODO sleep on msgQueue
	}
	// If we had a health exit empty out all of our queues
	if (healthy) {
	    processQueue();
	} else {
		// If unhealthy, quit the entire program (if not already quiting)
	    application->allThreadsQuit();
	}
	application->stopTrackingThread();
}

}
