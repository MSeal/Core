#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <boost/lexical_cast.hpp>
#include <sstream>
#include "detail/exceptionTypes.hpp"

// For demangling text in gcc
#ifdef __GNUC__ // gcc
#   include <cxxabi.h>
#   define ABI_DEMANGLE
#endif

namespace core {
// Forward declare these for specific template implementations
template<typename T> inline std::string toString(T castable);
inline std::string toString(const std::type_info& castable);
inline std::string toString(const std::type_info *castable);

/*
 * Used to create the strstart object, which is a static const
 * object with no internal state. The object is only used to
 * make syntax of string creation from various starting types
 * easy.
 */
struct StrStarter {
    template <typename T>
    std::string operator <<(const T& b) const {
        return toString(b);
    }

    std::string operator <<(const std::string& b) const {
        return b;
    }

    template <typename T>
    std::string operator +(const T& b) const {
        return toString(b);
    }

    std::string operator +(const std::string& b) const {
        return b;
    }
};
// Created to help with converting elements to strings
template<typename T>
std::string operator <<(const std::string& a, const T& b) {
    return a + toString(b);
}
// Create a constant string starter in our cpp -- no state
// just a helper for starting string assignments.
extern const StrStarter strstart;

// Do this as a macro so the __LINE__ matches where the error
// occurred.
#define throwCastException(message, sourceType, destType) \
    throw ExceptionBasis(message, ::core::CAST_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::Exception) \
             << ::core::ThrowErrorCastSource(&sourceType) \
             << ::core::ThrowErrorCastDest(&destType)

/*
 * Type: Generic
 */
template<typename T>
inline std::string toString(T castable) {
    try {
        return boost::lexical_cast<std::string, T>(castable);
    } catch (boost::bad_lexical_cast) {
        throwCastException("Unable to cast " << toString(typeid(T)) << " to std::string",
                            typeid(T), typeid(std::string));
    }
}
template<typename T>
inline std::string toStringNoThrow(T castable) {
    try {
        return toString(castable);
    } catch (boost::bad_lexical_cast) {
        return std::string();
    }
}
template<typename T>
inline T stringToType(const std::string& str) {
    try {
        return boost::lexical_cast<T, std::string>(str);
    } catch (boost::bad_lexical_cast e) {
        throwCastException("Unable to cast std::string to " << toString(typeid(T)),
                            typeid(std::string), typeid(T));
    }
}
template<typename T>
inline T stringToTypeNoThrow(const std::string& str) {
    try {
        return boost::lexical_cast<T, std::string>(str);
    } catch (boost::bad_lexical_cast) {
        return T();
    }
}


/*
 * Type: char *
 * Unnecessary really, but here for convenience
 */
inline std::string toString(const char *castable) {
    return std::string(castable);
}
inline std::string toStringNoThrow(const char *castable) {
    return std::string(castable);
}
template<>
inline const char *stringToType<const char *>(const std::string& str) {
    return str.c_str();
}
template<>
inline const char *stringToTypeNoThrow<const char *>(const std::string& str) {
    return str.c_str();
}



/*
 * Type: std::string
 * For unknown type conversion when the type is in fact a string.
 * Performs a copy, as the lifetime of castable is unclear.
 */
inline std::string toString(const std::string& castable) {
    return castable;
}
inline std::string toStringNoThrow(const std::string& castable) {
    return castable;
}


/*
 * Type: std::type_info
 */
inline std::string toString(const std::type_info& castable) {
#ifdef ABI_DEMANGLE // gcc
    // The GPP compiler mangles type names, so we need to
    // demangle them to get a cleaner print statement
    int status;
    char *realname = abi::__cxa_demangle(castable.name(), NULL, NULL, &status);
    if (status != 0) {
        // Just copy into a string for easy of memory use.
        // We don't care about inefficiencies here as this
        // is usually only called when an error has occurred.
        std::string tname(realname);
        // We're responsible for freeing
        free(realname);
        return tname;
    } else {
        return std::string("UnknownType");
    }
#else
    return std::string(castable.name());
#endif
}
inline std::string toString(const std::type_info *castable) {
    return toString(*castable);
}


/*
 * Type: ExceptionSeverity
 */
inline const std::string& toString(const ExceptionSeverity severity) {
    // The enumToValue function with const std::string& is
    // specially defined to do a no-copy request for names of
    // severity.
    // Otherwise we could copy with std::string typename
    // instead.
    return enumToValue<ExceptionSeverity, const std::string&>(severity);
}
template<>
inline ExceptionSeverity stringToType<ExceptionSeverity>(const std::string& str) {
    try {
        // Use our predefined bimap to convert for us
        return valueToEnum<ExceptionSeverity, std::string>(str);
    } catch (...) {
        throwCastException("Invalid/Unknown enumeration string name",
                            typeid(std::string), typeid(ExceptionSeverity));
    }
}
template<>
inline ExceptionSeverity stringToTypeNoThrow<ExceptionSeverity>(const std::string& str) {
    try {
        // Use our predefined bimap to convert for us
       return valueToEnum<ExceptionSeverity, std::string>(str);
   } catch (...) {
        return EXCEP_SEVERITY_ERROR;
    }
}


/*
 * Type: ExceptionCode
 */
inline const std::string& toString(const ExceptionCode code) {
    // The enumToValue function with const std::string& is
    // specially defined to do a no-copy request for names of
    // codes.
    // Otherwise we could copy with std::string typename
    // instead.
    return enumToValue<ExceptionCode, const std::string&>(code);
}
template<>
inline ExceptionCode stringToType<ExceptionCode>(const std::string& str) {
    try {
        // Use our predefined bimap to convert for us
        return valueToEnum<ExceptionCode, std::string>(str);
    } catch (...) {
        throwCastException("Invalid/Unknown enumeration string name",
                            typeid(std::string), typeid(ExceptionCode));
    }
}
template<>
inline ExceptionCode stringToTypeNoThrow<ExceptionCode>(const std::string& str) {
    try {
        // Use our predefined bimap to convert for us
        return valueToEnum<ExceptionCode, std::string>(str);
    } catch (...) {
        return UNKNOWN_EXCEPTION;
    }
}


/*
 * Type: boost::exception
 */
void extractExceptionMessage(const boost::exception& x, std::string& parseMessage);
void extractExceptionCode(const boost::exception& x, std::string& parseMessage);
void extractExceptionFunction(const boost::exception& x, std::string& parseMessage);
void extractExceptionFileAndLine(const boost::exception& x, std::string& parseMessage);

std::string toString(const boost::exception& x);
inline std::string toString(const boost::exception *x) {
    return toString(*x);
}

//TODO add more overloads for speedup/removal of exception throwing
}

// Get rid of this macro
#undef throwCastException

#endif /* STRINGUTIL_H_ */
