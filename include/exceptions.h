#ifndef EXCEPTIONS_UTIL_H_
#define EXCEPTIONS_UTIL_H_

#include "stringutil.h"
#include "detail/exceptionTypes.hpp"

namespace core {
/* Throw an exception for a non-specific reason */
#define throwBasicException(message) \
	throw ExceptionBasis(message, ::core::UNKNOWN_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::Exception)


/* Throw a (non-error) exception for information gathering purposes */
#define throwInfoException(message) \
	throw ExceptionBasis(message, ::core::UNKNOWN_EXCEPTION, \
			::core::EXCEP_SEVERITY_NOTICE, ::core::Exception)


/* Throw a warning exception */
#define throwWarningException(message) \
	throw ExceptionBasis(message, ::core::UNKNOWN_EXCEPTION, \
			::core::EXCEP_SEVERITY_WARNING, ::core::Exception)


/* Throw a NULL pointer exception */
#define throwNullPointerException(message) \
	throw ExceptionBasis(message, ::core::NULL_POINTER_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::Exception)


/* Throw a cast exception */
#define throwCastException(message, sourceType, destType) \
	throw ExceptionBasis(message, ::core::CAST_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::Exception) \
			 << ::core::ThrowErrorCastSource(&sourceType) \
			 << ::core::ThrowErrorCastDest(&destType)


/* Throw an initialization exception */
#define throwInitializationException(message) \
    throw ExceptionBasis(message, ::core::INITIALIZATION_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::Exception)


/* Throw a call once exception */
#define throwCallOnceException(message) \
    throw ExceptionBasis(message, ::core::CALL_ONCE_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::Exception)


/* IO exception struct */
struct IOException: virtual Exception { };
/* Throws an IO exception */
#define throwIOException(message) \
	throw ExceptionBasis(message, ::core::IO_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::IOException)


/* Throws an file open exception */
#define throwFileOpenException(message) \
	throw ExceptionBasis(message, ::core::FILE_OPEN_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::IOException)


/* Parameter exception struct */
struct ParameterException: virtual Exception { };
/* Throw a parameter exception */
#define throwParameterException(message) \
    throw ExceptionBasis(message, ::core::PARAMETER_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::ParameterException)


/* Throw an attribute exception */
#define throwAttributeException(message) \
    throw ExceptionBasis(message, ::core::ATTRIBUTE_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::ParameterException)


/* Throw an insert failed exception */
#define throwInsertFailedException(message) \
    throw ExceptionBasis(message, ::core::INSERT_FAILED_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::ParameterException)


/* Concurrency exception struct */
struct ConcurrencyException: virtual Exception { };
/* Throws a concurrency exception */
#define throwConcurrecyException(message) \
	throw ExceptionBasis(message, ::core::CONCURRENCY_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::ConcurrencyException)


/* Throws a race condition exception */
#define throwRaceConditionException(message) \
	throw ExceptionBasis(message, ::core::RACE_CONDITION_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::ConcurrencyException)


/* Throws a deadlock exception */
#define throwDeadlockException(message) \
	throw ExceptionBasis(message, ::core::DEADLOCK_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::ConcurrencyException)
}

#endif
