#ifndef EXCEPTIONS_UTIL_H_
#define EXCEPTIONS_UTIL_H_

#include "stringutil.hpp"
#include "detail/exceptionTypes.hpp"

namespace core {
/*
 * New exceptions need to first be added to exceptionTypes.hpp and
 * exceptionTypes.cpp (see block comment in exceptionTypes.cpp).
 */

/* Throw an exception for a non-specific reason */
#define throwBasicException(message) \
	throw ExceptionBasis(message, ::core::GENERIC_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::GenericException)


/* Throw a (non-error) exception for information gathering purposes */
#define throwInfoException(message) \
	throw ExceptionBasis(message, ::core::GENERIC_EXCEPTION, \
			::core::EXCEP_SEVERITY_NOTICE, ::core::GenericException)


/* Throw a warning exception */
#define throwWarningException(message) \
	throw ExceptionBasis(message, ::core::GENERIC_EXCEPTION, \
			::core::EXCEP_SEVERITY_WARNING, ::core::GenericException)


/* Throw a NULL pointer exception */
#define throwNullPointerException(message) \
	throw ExceptionBasis(message, ::core::NULL_POINTER_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::NullPointerException)


/* Throw a cast exception */
/*
 * NOTE: This is defined in stringutil briefly and changes to this
 * version should be updated there as well. This repeated code is
 * present to avoid circular dependencies.
 */
#define throwCastException(message, sourceType, destType) \
	throw ExceptionBasis(message, ::core::CAST_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::CastException) \
			 << ::core::ThrowErrorCastSource(&sourceType) \
			 << ::core::ThrowErrorCastDest(&destType)


/* Throw an initialization exception */
#define throwInitializationException(message) \
    throw ExceptionBasis(message, ::core::INITIALIZATION_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::InitializationException)


/* Throw a call once exception */
#define throwCallOnceException(message) \
    throw ExceptionBasis(message, ::core::CALL_ONCE_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::CallOnceException)


/* IO exceptions */
/* Throws an IO exception */
#define throwIOException(message) \
	throw ExceptionBasis(message, ::core::IO_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::IOException)


/* Throws an file open exception */
#define throwFileOpenException(message) \
	throw ExceptionBasis(message, ::core::FILE_OPEN_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::FileOpenException)


/* Parameter exception */
/* Throw a parameter exception */
#define throwParameterException(message) \
    throw ExceptionBasis(message, ::core::PARAMETER_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::ParameterException)


/* Throw an attribute exception */
#define throwAttributeException(message) \
    throw ExceptionBasis(message, ::core::ATTRIBUTE_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::AttributeException)


/* Throw an insert failed exception */
#define throwInsertFailedException(message) \
    throw ExceptionBasis(message, ::core::INSERT_FAILED_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::InsertFailedException)


/* Concurrency exception */
/* Throws a concurrency exception */
#define throwConcurrecyException(message) \
	throw ExceptionBasis(message, ::core::CONCURRENCY_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::ConcurrencyException)


/* Throws a race condition exception */
#define throwRaceConditionException(message) \
	throw ExceptionBasis(message, ::core::RACE_CONDITION_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::RaceConditionException)


/* Throws a deadlock exception */
#define throwDeadlockException(message) \
	throw ExceptionBasis(message, ::core::DEADLOCK_EXCEPTION, \
			::core::EXCEP_SEVERITY_ERROR, ::core::DeadlockException)


/* Throws a math exception */
#define throwDeadlockException(message) \
    throw ExceptionBasis(message, ::core::MATH_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::MathException)


/* Throws a divide by zero exception */
#define throwDeadlockException(message) \
    throw ExceptionBasis(message, ::core::DIVIDE_BY_ZERO_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::DivideByZeroException)
}

#endif
