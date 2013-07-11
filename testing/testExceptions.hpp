/*
 * testExceptions.hpp
 * Tests that the macro generation of exception types is valid.
 */

#ifndef TEST_EXCEPTIONS_HPP_
#define TEST_EXCEPTIONS_HPP_

#include "exceptions.hpp"
#include <boost/test/unit_test.hpp>

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
}

// TODO test other expected outputs
//BOOST_AUTO_TEST_CASE(exceptionMessageStreams) {
//    BOOST_CHECK_THROW(throwBasicException(1 << " single " << typeid(GenericException)), GenericException);
//    try {
//        throwBasicException(1 << " single " << std::string("GenericException"));
//        BOOST_ERROR("Failed to throw GenericException");
//    } catch(GenericException& ge) {
//        BOOST_CHECK_EQUAL(getExceptionMessage(ge), "1 single GenericException");
//    }
//}
//typedef boost::error_info<struct TagMessage, std::string> ThrowErrorMessage;
//const std::string getExceptionMessage(const boost::exception & x);
//
//typedef boost::error_info<struct TagErrorCode, ExceptionCode> ThrowErrorCode;
//ExceptionCode getExceptionCode(const boost::exception& x);
//
//typedef boost::error_info<struct TagErrorSeverity, ExceptionSeverity> ThrowErrorSeverity;
//ExceptionSeverity getExceptionSeverity(const boost::exception& x);
//
//typedef boost::throw_function ThrowErrorFunction;
//const std::string getExceptionFunction(const boost::exception& x);
//
//typedef boost::throw_file ThrowErrorFileName;
//const std::string getExceptionFileName(const boost::exception& x);
//
//typedef boost::throw_line ThrowErrorLineNumber;
//int getExceptionLineNumber(const boost::exception& x);
//
///* Cast exception attachments */
//typedef boost::error_info<struct TagCastSource, const std::type_info*> ThrowErrorCastSource;
//const std::type_info& getExceptionCastSource(const boost::exception& x);
//
//typedef boost::error_info<struct TagCastDest, const std::type_info*> ThrowErrorCastDest;
//const std::type_info& getExceptionCastDestination(const boost::exception& x);

BOOST_AUTO_TEST_SUITE_END()
}


#endif /* TEST_EXCEPTIONS_HPP_ */
