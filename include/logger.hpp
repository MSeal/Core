/* 
 * logger.h
 * Handles all logging calls used by the program.
 *
 * Note that the #defines for logging values are listed in
 * prelaodEnums.h
 */

#ifndef LOGGING_UTIL_H_
#define LOGGING_UTIL_H_

#include <string>
#include <vector>
#include <boost/thread/thread_time.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "threading/container/tswrapper.hpp"
#include "threading/container/tsqueue.hpp"
#include "exceptions.hpp"
#include "stringutil.hpp"
#include "pointers.hpp"
#include "factory.hpp"
#include "loops.hpp"
#include "enum.hpp"

namespace core {
// Forward declarations
class Application;
class LoggingFactory;

/*
 * Log severity
 *
 * Changes to this enumeration require changing:
 *      LOG_..._STRING (in cpp)
 *      enumToValue<LogLevel, const std::string&> (in cpp)
 *      LOG_STRING_BIMAP (in cpp)
 */
enum LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
};

typedef const boost::bimap<LogLevel, std::string> LogStringMap;
extern LogStringMap LOG_STRING_BIMAP;

template<>
inline LogStringMap& enumMappings<LogLevel, std::string>() {
    return LOG_STRING_BIMAP;
}
// Optimization for const string&, which does no copying
template<>
const std::string& enumToValue<LogLevel, const std::string&>(LogLevel eval);


/*
 * Define the interface LoggingSink used by Loggers to
 * push data to wherever it is needed.
 */
class LoggingSink : boost::noncopyable {
public:
    LoggingSink() {}
    virtual ~LoggingSink();

    // Allows the logger to flush all current sink requests.
    // This defaults to doing nothing
    virtual void flush() {}

    virtual void sinkMessage(LogLevel level, const std::string& msg) = 0;
};


// Forward declare LoggerBuilder
namespace detail { class LoggerBuilder; }
class Logger {
	// To allow our protected constructor to be called
	friend class LoggingFactory;
	friend class detail::LoggerBuilder;

protected:
    boost::ptr_vector<LoggingSink> sinks;

	// Only a factory can produce a Logger
	Logger(const std::string& logname, Application *const app) :
	    sinks(), name(logname), application(app) {}

public:
	const std::string name;
    Application *const application;

	~Logger() {}
	// Forces the log worker to wake up and process anything in
	// the queue.
	void flushLogs();

	/*
	 * WARNING: Don't add sinks in multi-threaded environments
	 * after logging has started.
	 */
	void addSink(LoggingSink *sinkptr) {
	    sinks.push_back(sinkptr);
	}
	template<typename SmartPtr>
	void addSink(SmartPtr sinkptr) {
        sinks.push_back(sinkptr.release());
    }

	/*
	 * Places the information string into each valid queue.
	 * The enqueue function copies the string, so after this
	 * point the information becomes immutable and is thread
	 * safe.
	 */
	template<typename T>
    void logMessage(LogLevel level, const T& msg) {
	    forEach (LoggingSink& logsink, sinks) {
            logsink.sinkMessage(level, toString(msg));
        }
	}

	void logMessage(LogLevel level, const std::string& msg) {
        forEach (LoggingSink& logsink, sinks) {
            logsink.sinkMessage(level, msg);
        }
	}

	// Log commands with a built-in level indicator
	template<typename T>
	void logDebug(const T& debug) { logMessage(LOG_DEBUG, debug); }

	template<typename T>
	void logInfo(const T& info) { logMessage(LOG_INFO, info); }

	template<typename T>
	void logWarning(const T& warning) { logMessage(LOG_WARNING, warning); }

	template<typename T>
    void logError(const T& error) { logMessage(LOG_ERROR, error); }

	// Extracts the LogLevel from the boost exception and calls logMessage
	void logException(const boost::exception &x) {
	    LogLevel loglevel;
        switch(getExceptionSeverity(x)) {
        case EXCEP_SEVERITY_NOTICE:
            loglevel = LOG_INFO;
            break;
        case EXCEP_SEVERITY_WARNING:
            loglevel = LOG_WARNING;
            break;
        case EXCEP_SEVERITY_ERROR:
        default:
            loglevel = LOG_ERROR;
            break;
        }
        logMessage(loglevel, x);
	}
};


namespace detail {
/*
 * Defines a factory for creating loggers.
 */
class LoggerBuilder {
public:
    Application *application;

    // Called by TrackedFactory
    LoggerBuilder() : application(NULL) {}
    explicit LoggerBuilder(Application *app) : application(app) {}

    Logger *build(const std::string& key) {
        if (application == NULL) {
            throwNullPointerException("Logger Builder given NULL pointer for application");
        }
        return new Logger(key, application);
    }
};
}

class LoggingFactory : public TrackedFactory<const std::string, Logger, detail::LoggerBuilder> {
    // To allow our protected constructor to be called
    friend class Application;

protected:
    // Hide the parent getOrProduce
    Logger& getOrProduce(const std::string& key);

    LoggingFactory(Application *app) {
        // builder isn't in the initialization list -- so add app here
        this->builder = detail::LoggerBuilder(app);
    }
};



/*
 * Timestamped message template
 */
template<typename T>
class TimestampedLevelMessage {
public:
    LogLevel loglevel;
    const T message;
    const boost::system_time stamp;

    explicit TimestampedLevelMessage(LogLevel level, const T& msg) :
        loglevel(level),
        message(msg),
        stamp(boost::get_system_time()) {}
};
typedef TimestampedLevelMessage<std::string> TSLevelString;
typedef pointers::smart<TSLevelString>::UniquePtr TSLevelStringPtr;

/*
 * Defines a Thread Safe Queue sink which pushes all inputs
 * through a queue and processes them on the other side
 */
class TSQueueSink : LoggingSink {
protected:
    typedef core::threading::container::TSQueue<TSLevelString> MessageQueue;
    MessageQueue msgQueue;
    Application *const application;

    typedef threading::container::TSWrapper<bool> ConditionLockable;
    ConditionLockable condLock;

    void sinkWorker();
    bool processQueue();

public:
    TSQueueSink(Application *const app) :
        msgQueue(), application(app) {
        //TODO create thread from application's thread factory
    }
    virtual ~TSQueueSink();

    // Allows the logger to flush all current sink requests.
    virtual void flush() {}

    virtual void sinkMessage(LogLevel level, const std::string& msg) {
        msgQueue.enqueue(new TSLevelString(level, msg));
    }

    virtual void processMessage(TSLevelString& msg) = 0;
};

}
#endif
