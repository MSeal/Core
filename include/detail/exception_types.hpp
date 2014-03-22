/*
 * exceptionTypes.hpp
 */
#ifndef EXCEPTION_TYPES_H_
#define EXCEPTION_TYPES_H_

#include <boost/exception/all.hpp>
#include <boost/current_function.hpp>
#include "detail/string_builders.hpp"
#include "pointers.hpp"
#include "enum.hpp"

namespace core {
/* Base exception struct */
struct Exception: virtual std::exception, virtual boost::exception{};

/*
 * Error severity
 *
 * Changes to this enumeration require changing/adding to:
 *      EXCEP_SEVERITY_..._STRING
 *      EXCEP_SEVERITY_STRING_BIMAP
 */
enum ExceptionSeverity {
    EXCEP_SEVERITY_NOTICE   = 0,
    EXCEP_SEVERITY_WARNING  = 5,
    EXCEP_SEVERITY_ERROR    = 10
};

/*
 * Exception Severity Enum Definitions
 */
const std::string EXCEP_SEVERITY_NOTICE_STRING = "Notice";
const std::string EXCEP_SEVERITY_WARNING_STRING = "Warning";
const std::string EXCEP_SEVERITY_ERROR_STRING = "Error";
const std::string EXCEP_SEVERITY_UNKOWN_STRING = "Unknown Exception Severity";

typedef const boost::bimap<ExceptionSeverity, std::string> ServityStringMap;
ServityStringMap EXCEP_SEVERITY_STRING_BIMAP =
        BI_MAP_BUILDER(ExceptionSeverity, std::string)
        (EXCEP_SEVERITY_NOTICE, EXCEP_SEVERITY_NOTICE_STRING)
        (EXCEP_SEVERITY_WARNING, EXCEP_SEVERITY_WARNING_STRING)
        (EXCEP_SEVERITY_ERROR, EXCEP_SEVERITY_ERROR_STRING);

template<>
inline ServityStringMap& enumMappings<ExceptionSeverity, std::string>() {
    return EXCEP_SEVERITY_STRING_BIMAP;
}

/*
 * Implementation is in detail/stringBuilders; forward declared here to
 * avoid circular include
 */
struct StrStreamStarter;
struct StrStreamEnder;
extern const StrStreamStarter strstarter;
extern const StrStreamEnder strender;

/*
 * Defines a generic exception throwing without defaulted code
 * and severity settings. This is used by the exception throws
 * below.
 */
// Creates a core exception from meta data
#define ExceptionBasis(message, code, severity, exceptionType)                          \
    exceptionType()                                                                     \
        << ::core::ThrowErrorMessage(::core::strstarter << message << ::core::strender) \
        << ::core::ThrowErrorFunction(BOOST_CURRENT_FUNCTION)                           \
        << ::core::ThrowErrorFileName(__FILE__)                                         \
        << ::core::ThrowErrorLineNumber(__LINE__)                                       \
        << ::core::ThrowErrorCode(code)                                                 \
        << ::core::ThrowErrorSeverity(severity)

// Extends an existing boost::exception to include core except meta data
#define extendBasicException(message, code, severity, exception)                        \
    exception                                                                           \
        << ::core::ThrowErrorMessage(::core::strstarter << message << ::core::strender) \
        << ::core::ThrowErrorFunction(BOOST_CURRENT_FUNCTION)                           \
        << ::core::ThrowErrorFileName(__FILE__)                                         \
        << ::core::ThrowErrorLineNumber(__LINE__)                                       \
        << ::core::ThrowErrorCode(code)                                                 \
        << ::core::ThrowErrorSeverity(severity)

// Defines an assertion statement that throws an exception upon failure
#define assertAsException(condition, message)                                           \
    if (!condition)                                                                     \
        throw ExceptionBasis(message,                                                   \
                ::core::ASSERTION_EXCEPTION,                                            \
                ::core::EXCEP_SEVERITY_ERROR,                                           \
                ::core::Exception)

/*
 * Exception hierarchy -- for each ExceptionCode there is a struct
 * representing that type of exception. These are used for catch
 * statements to be able to capture groups or specific exception
 * types.
 */
struct GenericException: virtual Exception{};
struct UnknownException: virtual GenericException{};
struct NullPointerException: virtual GenericException{};
struct CastException: virtual GenericException{};
struct InitializationException: virtual GenericException{};
struct RepeatedCallException: virtual GenericException{};
struct IOException: virtual Exception{};
struct FileOpenException: virtual IOException{};
struct ParameterException: virtual Exception{};
struct AttributeException: virtual ParameterException{};
struct InsertFailedException: virtual ParameterException{};
struct ConcurrencyException: virtual Exception{};
struct RaceConditionException: virtual ConcurrencyException{};
struct DeadlockException: virtual ConcurrencyException{};
struct MathException: virtual Exception{};
struct DivideByZeroException: virtual MathException{};

/*
 * Error codes -- These are all of the standard exception types
 *
 * Changes to this enumeration require changing/adding to:
 *      ..._EXCEPTION_STRING
 *      EXCEP_CODE_STRING_BIMAP
 *      throw...Exception (in exceptions.hpp)
 */
enum ExceptionCode {
    GENERIC_EXCEPTION           = 0,
    UNKNOWN_EXCEPTION           = 1,
    ASSERTION_EXCEPTION         = 2,
    NULL_POINTER_EXCEPTION      = 3,
    CAST_EXCEPTION              = 4,
    INITIALIZATION_EXCEPTION    = 5,
    CALL_ONCE_EXCEPTION         = 6,
    IO_EXCEPTION                = 10,
    FILE_OPEN_EXCEPTION         = 11,
    PARAMETER_EXCEPTION         = 20,
    ATTRIBUTE_EXCEPTION         = 21,
    INSERT_FAILED_EXCEPTION     = 22,
    CONCURRENCY_EXCEPTION       = 30,
    RACE_CONDITION_EXCEPTION    = 31,
    DEADLOCK_EXCEPTION          = 32,
    MATH_EXCEPTION              = 40,
    DIVIDE_BY_ZERO_EXCEPTION    = 41
};

const std::string EMPTY_EXCEPTION_STRING = "";
const std::string GENERIC_EXCEPTION_STRING = "Generic Exception";
const std::string UNKNOWN_EXCEPTION_STRING = "Unknown Exception";
const std::string ASSERTION_EXCEPTION_STRING = "Assertion Failed Exception";
const std::string NULL_POINTER_EXCEPTION_STRING = "NULL Pointer Exception";
const std::string CAST_EXCEPTION_STRING = "Cast Exception";
const std::string INITIALIZATION_EXCEPTION_STRING = "Initialization Exception";
const std::string CALL_ONCE_EXCEPTION_STRING = "Call Once Exception";
const std::string IO_EXCEPTION_STRING = "IO Exception";
const std::string FILE_OPEN_EXCEPTION_STRING = "File Open Exception";
const std::string PARAMETER_EXCEPTION_STRING = "Invalid Parameter Exception";
const std::string ATTRIBUTE_EXCEPTION_STRING = "Attribute Exception";
const std::string INSERT_FAILED_EXCEPTION_STRING = "Insert Failed Exception";
const std::string CONCURRENCY_EXCEPTION_STRING = "Concurrency Exception";
const std::string RACE_CONDITION_EXCEPTION_STRING = "Concurrency Race Condition Exception";
const std::string DEADLOCK_EXCEPTION_STRING = "Concurrency Deadlock Exception";
const std::string MATH_EXCEPTION_STRING = "Math Exception";
const std::string DIVIDE_BY_ZERO_EXCEPTION_STRING = "Divide By Zero Exception";

typedef const boost::bimap<ExceptionCode, std::string> ExceptCodeStringMap;
ExceptCodeStringMap EXCEP_CODE_STRING_BIMAP =
    BI_MAP_BUILDER(ExceptionCode, std::string)
    (GENERIC_EXCEPTION, GENERIC_EXCEPTION_STRING)
    (UNKNOWN_EXCEPTION, UNKNOWN_EXCEPTION_STRING)
    (ASSERTION_EXCEPTION, ASSERTION_EXCEPTION_STRING)
    (NULL_POINTER_EXCEPTION, NULL_POINTER_EXCEPTION_STRING)
    (CAST_EXCEPTION, CAST_EXCEPTION_STRING)
    (INITIALIZATION_EXCEPTION, INITIALIZATION_EXCEPTION_STRING)
    (CALL_ONCE_EXCEPTION, CALL_ONCE_EXCEPTION_STRING)
    (IO_EXCEPTION, IO_EXCEPTION_STRING)
    (FILE_OPEN_EXCEPTION, FILE_OPEN_EXCEPTION_STRING)
    (PARAMETER_EXCEPTION, PARAMETER_EXCEPTION_STRING)
    (ATTRIBUTE_EXCEPTION, ATTRIBUTE_EXCEPTION_STRING)
    (INSERT_FAILED_EXCEPTION, INSERT_FAILED_EXCEPTION_STRING)
    (CONCURRENCY_EXCEPTION, CONCURRENCY_EXCEPTION_STRING)
    (RACE_CONDITION_EXCEPTION, RACE_CONDITION_EXCEPTION_STRING)
    (DEADLOCK_EXCEPTION, DEADLOCK_EXCEPTION_STRING)
    (MATH_EXCEPTION, MATH_EXCEPTION_STRING)
    (DIVIDE_BY_ZERO_EXCEPTION, DIVIDE_BY_ZERO_EXCEPTION_STRING);

template<>
inline ExceptCodeStringMap& enumMappings<ExceptionCode, std::string>() {
    return EXCEP_CODE_STRING_BIMAP;
}

/* Typical exception attachments */
typedef boost::error_info<struct TagMessage, std::string> ThrowErrorMessage;
const std::string getExceptionMessage(const boost::exception & x);

typedef boost::error_info<struct TagErrorCode, ExceptionCode> ThrowErrorCode;
ExceptionCode getExceptionCode(const boost::exception& x);

typedef boost::error_info<struct TagErrorSeverity, ExceptionSeverity> ThrowErrorSeverity;
ExceptionSeverity getExceptionSeverity(const boost::exception& x);

typedef boost::throw_function ThrowErrorFunction;
const std::string getExceptionFunction(const boost::exception& x);

typedef boost::throw_file ThrowErrorFileName;
const std::string getExceptionFileName(const boost::exception& x);

typedef boost::throw_line ThrowErrorLineNumber;
int getExceptionLineNumber(const boost::exception& x);

/* Cast exception attachments */
typedef boost::error_info<struct TagCastSource, const std::type_info*> ThrowErrorCastSource;
const std::type_info& getExceptionCastSource(const boost::exception& x);

typedef boost::error_info<struct TagCastDest, const std::type_info*> ThrowErrorCastDest;
const std::type_info& getExceptionCastDestination(const boost::exception& x);

}

#endif /* EXCEPTION_TYPES_H_ */
