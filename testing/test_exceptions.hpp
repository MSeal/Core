/*
 * testExceptions.hpp
 * Tests that the macro generation of exception types is valid.
 */

#ifndef TEST_EXCEPTIONS_HPP_
#define TEST_EXCEPTIONS_HPP_

#include "exceptions.hpp"

// Disable unreachable code warnings temporarily
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push)
#   pragma warning(disable : 4702)
//  Don't listen to warnings about boost on msvc
#   pragma warning(push, 0)
#endif

#include <boost/test/unit_test.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

namespace core {
BOOST_AUTO_TEST_SUITE(ExceptionTests)

BOOST_AUTO_TEST_CASE(thrownExceptionTypes) {
    BOOST_CHECK_THROW(throwBasicException("An exception"), GenericException);
    BOOST_CHECK_THROW(throwInfoException("An info exception"), GenericException);
    BOOST_CHECK_THROW(throwWarningException("A warning exception"), GenericException);

    BOOST_CHECK_THROW(throwNullPointerException("A null pointer exception"), NullPointerException);
    BOOST_CHECK_THROW(throwNullPointerException("A null pointer exception"), GenericException);

    BOOST_CHECK_THROW(throwCastException("A cast exception", typeid(int*), typeid(std::string)), CastException);
    BOOST_CHECK_THROW(throwCastException("A cast exception", typeid(int*), typeid(std::string)), GenericException);

    BOOST_CHECK_THROW(throwInitializationException("An initialization exception"), InitializationException);
    BOOST_CHECK_THROW(throwInitializationException("An initialization exception"), GenericException);

    BOOST_CHECK_THROW(throwRepeatedCallException("A call once exception"), RepeatedCallException);
    BOOST_CHECK_THROW(throwRepeatedCallException("A call once exception"), GenericException);

    BOOST_CHECK_THROW(throwIOException("An I/O exception"), IOException);
    BOOST_CHECK_THROW(throwFileOpenException("A file open exception"), FileOpenException);
    BOOST_CHECK_THROW(throwFileOpenException("A file open exception"), IOException);

    BOOST_CHECK_THROW(throwParameterException("A parameter exception"), ParameterException);
    BOOST_CHECK_THROW(throwAttributeException("An attribute exception"), AttributeException);
    BOOST_CHECK_THROW(throwAttributeException("An attribute exception"), ParameterException);
}

BOOST_AUTO_TEST_CASE(exceptionMessageStreams) {
    BOOST_CHECK_THROW(throwBasicException(1 << " single " << typeid(GenericException)), GenericException);
    try {
        throwBasicException(1 << " single " << std::string("GenericException"));
        BOOST_ERROR("Failed to throw GenericException");
    } catch(GenericException& ge) {
        BOOST_CHECK_EQUAL(getExceptionMessage(ge), "1 single GenericException");
    }

#ifdef ABI_DEMANGLE // gcc
    // Test typeid format generation if gcc
    try {
        throwBasicException(typeid(GenericException));
        BOOST_ERROR("Failed to throw GenericException");
    } catch(GenericException& ge) {
        BOOST_CHECK_EQUAL(getExceptionMessage(ge), "core::GenericException");
    }
#endif
}

BOOST_AUTO_TEST_CASE(exceptionCode) {
    try {
        throwBasicException("Basic");
        BOOST_ERROR("Failed to throw Basic Exception");
    } catch(GenericException& be) {
        BOOST_CHECK_EQUAL(getExceptionCode(be), GENERIC_EXCEPTION);
    }

    try {
        throwNullPointerException("Null Pointer");
        BOOST_ERROR("Failed to throw Null Pointer Exception");
    } catch(NullPointerException& npe) {
        BOOST_CHECK_EQUAL(getExceptionCode(npe), NULL_POINTER_EXCEPTION);
    }

    try {
        throwInsertFailedException("Insert Failed");
        BOOST_ERROR("Failed to throw Null Pointer Exception");
    } catch(InsertFailedException& ife) {
        BOOST_CHECK_EQUAL(getExceptionCode(ife), INSERT_FAILED_EXCEPTION);
    }
}

BOOST_AUTO_TEST_CASE(exceptionSeverity) {
    try {
        throwInfoException("Info");
        BOOST_ERROR("Failed to throw Info Exception");
    } catch(GenericException& ie) {
        BOOST_CHECK_EQUAL(getExceptionSeverity(ie), EXCEP_SEVERITY_NOTICE);
    }

    try {
        throwWarningException("Warning");
        BOOST_ERROR("Failed to throw Warning Exception");
    } catch(GenericException& we) {
        BOOST_CHECK_EQUAL(getExceptionSeverity(we), EXCEP_SEVERITY_WARNING);
    }

    try {
        throwBasicException("Basic");
        BOOST_ERROR("Failed to throw Basic Exception");
    } catch(GenericException& be) {
        BOOST_CHECK_EQUAL(getExceptionSeverity(be), EXCEP_SEVERITY_ERROR);
    }

    try {
        throwNullPointerException("Null Pointer");
        BOOST_ERROR("Failed to throw Null Pointer Exception");
    } catch(NullPointerException& npe) {
        BOOST_CHECK_EQUAL(getExceptionSeverity(npe), EXCEP_SEVERITY_ERROR);
    }
}

BOOST_AUTO_TEST_CASE(exceptionFunction) {
    try {
        throwBasicException("Basic");
        BOOST_ERROR("Failed to throw Basic Exception");
    } catch(GenericException& be) {
        BOOST_CHECK(getExceptionFunction(be).find("exceptionFunction") != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(exceptionFileName) {
    try {
       throwBasicException("Basic");
       BOOST_ERROR("Failed to throw Basic Exception");
   } catch(GenericException& be) {
       BOOST_CHECK_EQUAL(getExceptionFileName(be), __FILE__);
   }
}

BOOST_AUTO_TEST_CASE(exceptionLineNumber) {
    try {
       throwBasicException("Basic");
       BOOST_ERROR("Failed to throw Basic Exception");
   } catch(GenericException& be) {
       BOOST_CHECK(getExceptionLineNumber(be) > 0);
   }
}

BOOST_AUTO_TEST_CASE(exceptionCast) {
    try {
       throwCastException("Cast test", typeid(std::string), typeid(int));
       BOOST_ERROR("Failed to throw Cast Exception");
   } catch(GenericException& be) {
       BOOST_CHECK(getExceptionCastSource(be) == typeid(std::string));
       BOOST_CHECK(getExceptionCastDestination(be) == typeid(int));
   }
}

BOOST_AUTO_TEST_SUITE_END()
}

// Undo our temporary warning disable
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

#endif /* TEST_EXCEPTIONS_HPP_ */
