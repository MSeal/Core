/*
 * exceptionTypes.hpp
 *
 * The basis of all exceptions, this defines the exception macros as well as
 * includes all basic exception types.
 */
#ifndef EXCEPTION_TYPES_H_
#define EXCEPTION_TYPES_H_

#include "detail/exception_enums.hpp"
#include "pointers.hpp"
#include "enum.hpp"

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/exception/all.hpp>
#include <boost/current_function.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {

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
