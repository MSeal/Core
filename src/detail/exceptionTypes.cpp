#include "detail/exceptionTypes.hpp"
#include "enum.hpp"

namespace core {
/*
 * Exception Severity Enum Definitions
 */
const std::string EXCEP_SEVERITY_NOTICE_STRING = "Notice";
const std::string EXCEP_SEVERITY_WARNING_STRING = "Warning";
const std::string EXCEP_SEVERITY_ERROR_STRING = "Error";
const std::string EXCEP_SEVERITY_UNKOWN_STRING = "Unknown Exception Severity";

// Optimization for const string&, which does no copying
template<>
const std::string& enumToValue<ExceptionSeverity, const std::string&>(ExceptionSeverity eval) {
    switch(eval) {
    case EXCEP_SEVERITY_NOTICE: return EXCEP_SEVERITY_NOTICE_STRING;
    case EXCEP_SEVERITY_WARNING: return EXCEP_SEVERITY_WARNING_STRING;
    case EXCEP_SEVERITY_ERROR: return EXCEP_SEVERITY_ERROR_STRING;
    default: return EXCEP_SEVERITY_UNKOWN_STRING;
    }
}

ServityStringMap EXCEP_SEVERITY_STRING_BIMAP =
        BI_MAP_BUILDER(ExceptionSeverity, std::string)
        (EXCEP_SEVERITY_NOTICE, EXCEP_SEVERITY_NOTICE_STRING)
        (EXCEP_SEVERITY_WARNING, EXCEP_SEVERITY_WARNING_STRING)
        (EXCEP_SEVERITY_ERROR, EXCEP_SEVERITY_ERROR_STRING);

const std::string EMPTY_EXCEPTION_STRING = "";
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

// Optimization for const string&, which does no copying
template<>
const std::string& enumToValue<ExceptionCode, const std::string&>(ExceptionCode eval) {
    switch (eval) {
    case UNKNOWN_EXCEPTION: return UNKNOWN_EXCEPTION_STRING;
    case ASSERTION_EXCEPTION: return ASSERTION_EXCEPTION_STRING;
    case NULL_POINTER_EXCEPTION: return NULL_POINTER_EXCEPTION_STRING;
    case CAST_EXCEPTION: return CAST_EXCEPTION_STRING;
    case INITIALIZATION_EXCEPTION: return INITIALIZATION_EXCEPTION_STRING;
    case CALL_ONCE_EXCEPTION: return CALL_ONCE_EXCEPTION_STRING;
    case IO_EXCEPTION: return IO_EXCEPTION_STRING;
    case FILE_OPEN_EXCEPTION: return FILE_OPEN_EXCEPTION_STRING;
    case PARAMETER_EXCEPTION: return PARAMETER_EXCEPTION_STRING;
    case ATTRIBUTE_EXCEPTION: return ATTRIBUTE_EXCEPTION_STRING;
    case INSERT_FAILED_EXCEPTION: return INSERT_FAILED_EXCEPTION_STRING;
    case CONCURRENCY_EXCEPTION: return CONCURRENCY_EXCEPTION_STRING;
    case RACE_CONDITION_EXCEPTION: return RACE_CONDITION_EXCEPTION_STRING;
    case DEADLOCK_EXCEPTION: return DEADLOCK_EXCEPTION_STRING;
    default: return UNKNOWN_EXCEPTION_STRING;
    }
}

ExceptCodeStringMap EXCEP_CODE_STRING_BIMAP =
        BI_MAP_BUILDER(ExceptionCode, std::string)
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
        (DEADLOCK_EXCEPTION, DEADLOCK_EXCEPTION_STRING);

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
const std::type_info& getExceptionCastSource(boost::exception& x) {
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
