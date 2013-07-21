#include "detail/exceptionTypes.hpp"

namespace core {

const std::string getExceptionMessage(const boost::exception & x) {
    const std::string *result = boost::get_error_info<ThrowErrorMessage>(x);
    if (result == NULL) {
        return EMPTY_EXCEPTION_STRING;
    }
    return *result;
}

ExceptionCode getExceptionCode(const boost::exception & x) {
    const ExceptionCode *result = boost::get_error_info<ThrowErrorCode>(x);
    if (result == NULL) {
        return UNKNOWN_EXCEPTION;
    }
    return *result;
}

ExceptionSeverity getExceptionSeverity(const boost::exception & x) {
    const ExceptionSeverity *result = boost::get_error_info<ThrowErrorSeverity>(x);
    if (result == NULL) {
        return EXCEP_SEVERITY_ERROR;
    }
    return *result;
}

const std::string getExceptionFunction(const boost::exception & x) {
    const char *const *result = boost::get_error_info<ThrowErrorFunction>(x);
    if (result == NULL) {
        return EMPTY_EXCEPTION_STRING;
    }
    return std::string(*result);
}

const std::string getExceptionFileName(const boost::exception& x) {
    const char *const *result = boost::get_error_info<ThrowErrorFileName>(x);
    if (result == NULL) {
        return EMPTY_EXCEPTION_STRING;
    }
    return std::string(*result);
}

int getExceptionLineNumber(const boost::exception& x) {
    const int *result = boost::get_error_info<ThrowErrorLineNumber>(x);
    if (result == NULL) {
        return 0;
    }
    return *result;
}

/* Cast exception attachments */
const std::type_info& getExceptionCastSource(const boost::exception& x) {
    const std::type_info *const *result = boost::get_error_info<ThrowErrorCastSource>(x);
    if (result == NULL) {
        return typeid(void);
    }
    return **result;
}

const std::type_info& getExceptionCastDestination(const boost::exception& x) {
    const std::type_info *const *result = boost::get_error_info<ThrowErrorCastDest>(x);
    if (result == NULL) {
        return typeid(void);
    }
    return **result;
}
}
