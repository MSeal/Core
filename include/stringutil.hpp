/*
 * stringutil.h
 *
 * TODO describe features
 */
#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

#include <boost/lexical_cast.hpp>
#include <boost/locale/encoding_utf.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/if.hpp>
#include <vector>
#include <sstream>
#include <limits.h>
#include "detail/exceptionTypes.hpp"
#include "loops.hpp"
#include "pointers.hpp"
#include "pptypes.hpp"

// For demangling text in gcc
#ifdef __GNUC__ // gcc
#   include <cxxabi.h>
#   define ABI_DEMANGLE
#endif

namespace core {
// Typedefs for various UTF character types
#if CHAR_MAX == 0xFF
typedef char UTF8;
#else
typedef uint8_t UTF8;
#endif
typedef std::basic_string<UTF8> u8string;
#if CHAR_MAX == 0xFFFF
typedef char UTF16;
#else
typedef uint16_t UTF16;
#endif
typedef std::basic_string<UTF16> u16string;
#if CHAR_MAX == 0xFFFFFFFF
typedef char UTF32;
#else
typedef uint32_t UTF32;
#endif
typedef std::basic_string<UTF32> u32string;

// Forward declarations
template<typename T> inline std::string toString(const T& castable);
inline std::string toString(const std::type_info& castable);
inline std::string toString(const std::type_info *castable);
template<typename T> inline T stringToType(const std::string& str);
class StrStreamer;
typedef pointers::smart<StrStreamer>::UniquePtr StrStreamerPtr;

// TODO describe
struct StrStreamEnder{};

// TODO describe
class StrStreamer {
    friend inline std::string operator <<(
            StrStreamerPtr builder, const StrStreamEnder& convertable);
    template<typename T>
    friend inline StrStreamerPtr operator <<(
            StrStreamerPtr builder, const T& convertable);
    friend inline StrStreamerPtr operator <<(
            StrStreamerPtr builder, const std::string& convertable);

private:
    typedef boost::variant<const std::string *, std::string> stringTypes;
    std::vector<stringTypes> stracker;

    /*
     * Converts stringTypes to a string reference.
     */
    const std::string& getStrRef(stringTypes& strin);

public:
    // Used in case of error
    static const std::string emptyStr;
    StrStreamer() : stracker() {
        stracker.reserve(std::vector<stringTypes>::size_type(128));
    }

    /*
     * Builds the string seen so far by streams.
     */
    std::string buildString();
};

// TODO describe
struct StrStreamStarter {
    template <typename T>
    StrStreamerPtr operator <<(const T& b) const {
        return StrStreamerPtr(new StrStreamer()) << b;
    }
};
// TODO describe
extern const StrStreamStarter strstarter;
extern const StrStreamEnder strender;

// TODO describe
class Stringifyable {
public:
    virtual std::string toString() const;
};

/*
 * Do this as a macro so the __LINE__ matches where the error
 * occurred.
 * Not defined in exceptions to avoid circular include logic.
 * Undefined at end of file.
 */
#define throwCastException(message, sourceType, destType) \
    throw ExceptionBasis(message, ::core::CAST_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, ::core::CastException) \
             << ::core::ThrowErrorCastSource(&sourceType) \
             << ::core::ThrowErrorCastDest(&destType)

namespace detail {
// Creates IsMemberSigFunc_toString_check
CREATE_MEMBER_CHECK(toString);

/*
 * This function has no working knowledge of the type, so it just tries
 * a lexical cast.
 */
template<bool hasMethod, typename T>
struct StringImplStruct {
    static std::string toStringImpl(const T& castable) {
        try {
            return boost::lexical_cast<std::string, T>(castable);
        } catch (boost::bad_lexical_cast) {
            throwCastException("Unable to cast " << toString(typeid(T)) << " to std::string",
                               typeid(T), typeid(std::string));
        }
    }
};

/*
 * The object has a 'toString' member, try using it as a 'std::string toString()'
 * method.
 * NOTE: without C++11 it's impossible to check the return type of a no-argument
 * function when it could also just be a member. Everything looks like the correct
 * type and have ambiguous returns because of the ()operator.
 */
template<typename T>
struct StringImplStruct<true, T> {
    static std::string toStringImpl(const T& castable) {
        return castable.toString();
    }
};
}

/*
 * Type: Generic
 */
template<typename T>
inline std::string toString(const T& castable) {
    return detail::StringImplStruct
            <detail::HasMember_toString<T>::value, T>::
                toStringImpl(castable);
}

template<typename T>
inline std::string toStringNoThrow(const T& castable) {
    try {
        return toString(castable);
    } catch (boost::bad_lexical_cast) {
        return std::string();
    } catch (...) {
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
 * Type: Stringifyable
 * For user defined classes which implement toString
 */
inline std::string toString(const Stringifyable& obj) {
    return obj.toString();
}


/*
 * Helper macros that automatically generate the error string,
 * templates, and type identifying for a locale string conversion.
 */
#define LOCALE_MACRO_EXPAND(tok) tok
#define LOCALE_STR_MACRO_EXPAND(tok) #tok
#define LOCALE_STR_MACRO(tok) LOCALE_STR_MACRO_EXPAND(tok)
#define LOCALE_CAST_QUOTE(incharstr, outstring) \
        Unable to convert LOCALE_MACRO_EXPAND(incharstr) to LOCALE_MACRO_EXPAND(outstring);
#define LOCALE_CAST_QUOTE_ILLEGAL invalid character found
#define LOCALE_CAST_QUOTE_CRASH unexpected exception occured (bad string pointer?)
#define LOCALE_CAST_QUOTE_COMBINE(incharstr, outstring) \
        LOCALE_CAST_QUOTE(incharstr, outstring) LOCALE_CAST_QUOTE_ILLEGAL
#define LOCALE_CRASH_QUOTE_COMBINE(incharstr, outstring) \
        LOCALE_CAST_QUOTE(incharstr, outstring) LOCALE_CAST_QUOTE_CRASH

#define LOCALE_CONVERT_FILL(incharstr, inputname, outstring, outchar)                           \
    try {                                                                                       \
        return boost::locale::conv::utf_to_utf<outchar>(inputname, boost::locale::conv::stop);  \
    } catch (std::runtime_error re) {                                                           \
        /* Rethrow as cast exception type */                                                    \
        throwCastException(LOCALE_STR_MACRO(LOCALE_CAST_QUOTE_COMBINE(incharstr, outstring)),   \
                typeid(incharstr), typeid(outstring));                                          \
    } catch (...) {                                                                             \
        throwCastException(LOCALE_STR_MACRO(LOCALE_CRASH_QUOTE_COMBINE(incharstr, outstring)),  \
                typeid(incharstr), typeid(outstring));                                          \
    }
#define LOCALE_CONVERT_FILL_NO_THROW(inputname, outstring, outchar)                             \
    try {                                                                                       \
        return boost::locale::conv::utf_to_utf<outchar>(inputname, boost::locale::conv::skip);  \
    } catch (...) {                                                                             \
        return outstring();                                                                     \
    }

/*
 * Type: wstring
 */
inline std::string toString(const std::wstring& castable) {
    LOCALE_CONVERT_FILL(std::wstring, castable, std::string, char);
}
inline std::string toStringNoThrow(const std::wstring& castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
template<>
inline std::wstring stringToType<std::wstring>(const std::string& str) {
    LOCALE_CONVERT_FILL(std::string, str, std::wstring, wchar_t);
}
template<>
inline std::wstring stringToTypeNoThrow<std::wstring>(const std::string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, std::wstring, wchar_t);
}


/*
 * Type: UTF8 string
 */
#if CHAR_MAX != 0xFF
inline std::string toString(const u8string& castable) {
    LOCALE_CONVERT_FILL(u8string, castable, std::string, char);
}
inline std::string toStringNoThrow(const u8string& castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
template<>
inline u8string stringToType<u8string>(const std::string& str) {
    LOCALE_CONVERT_FILL(std::string, str, u8string, UTF8);
}
template<>
inline u8string stringToTypeNoThrow<u8string>(const std::string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u8string, UTF8);
}
#endif


/*
 * Type: UTF16 string
 */
#if CHAR_MAX != 0xFFFF
inline std::string toString(const u16string& castable) {
    LOCALE_CONVERT_FILL(u16string, castable, std::string, char);
}
inline std::string toStringNoThrow(const u16string& castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
template<>
inline u16string stringToType<u16string>(const std::string& str) {
    LOCALE_CONVERT_FILL(std::string, str, u16string, UTF16);
}
template<>
inline u16string stringToTypeNoThrow<u16string>(const std::string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u16string, UTF16);
}
#endif


/*
 * Type: UTF32 string
 */
#if CHAR_MAX != 0xFFFFFFFF
inline std::string toString(const u32string& castable) {
    LOCALE_CONVERT_FILL(u32string, castable, std::string, char);
}
inline std::string toStringNoThrow(const u32string& castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
template<>
inline u32string stringToType<u32string>(const std::string& str) {
    LOCALE_CONVERT_FILL(std::string, str, u32string, UTF32);
}
template<>
inline u32string stringToTypeNoThrow<u32string>(const std::string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u32string, UTF32);
}
#endif


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
/*
 * Type: char
 */
inline std::string toString(const char castable) {
    return std::string(&castable, 1);
}
inline std::string toStringNoThrow(const char castable) {
    return std::string(&castable, 1);
}


/*
 * Type: wchar_t *
 */
inline std::string toString(const wchar_t *castable) {
    LOCALE_CONVERT_FILL(wchar_t*, castable, std::string, char);
}
inline std::string toStringNoThrow(const wchar_t *castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
/*
 * Type: wchar_t
 */
inline std::string toString(const wchar_t castable) {
    LOCALE_CONVERT_FILL(wchar_t, std::wstring(&castable), std::string, char);
}
inline std::string toStringNoThrow(const wchar_t castable) {
    LOCALE_CONVERT_FILL_NO_THROW(std::wstring(&castable), std::string, char);
}


/*
 * Type: UTF8
 */
#if CHAR_MAX != 0xFF
inline std::string toString(const UTF8 *castable) {
    LOCALE_CONVERT_FILL(UTF8*, castable, std::string, char);
}
inline std::string toStringNoThrow(const UTF8 *castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
#endif


/*
 * Type: UTF16
 */
#if CHAR_MAX != 0xFFFF
inline std::string toString(const UTF16 *castable) {
    LOCALE_CONVERT_FILL(UTF16*, castable, std::string, char);
}
inline std::string toStringNoThrow(const UTF16 *castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
#endif


/*
 * Type: UTF32
 */
#if CHAR_MAX != 0xFFFFFFFF
inline std::string toString(const UTF32 *castable) {
    LOCALE_CONVERT_FILL(UTF32*, castable, std::string, char);
}
inline std::string toStringNoThrow(const UTF32 *castable) {
    LOCALE_CONVERT_FILL_NO_THROW(castable, std::string, char);
}
#endif


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
        // Just copy into a string for ease of memory use.
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
inline std::string toStringNoThrow(const std::type_info& castable) {
    try {
        return toString(castable);
    } catch (...) {
        return std::string("UnknownType");
    }
}
inline std::string toStringNoThrow(const std::type_info *castable) {
    try {
        return toString(*castable);
    } catch (...) {
        return std::string("UnknownType");
    }
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

/*
 * Conversion renaming for ease of use with UTF8.
 */
inline u8string stringToUTF8(const std::string& str) {
    return stringToType<u8string>(str);
}
inline u8string stringToUTF8NoThrow(const std::string& str) {
    return stringToTypeNoThrow<u8string>(str);
}
inline u8string stringToUTF8(const char *str) {
    return stringToType<u8string>(str);
}
inline u8string stringToUTF8NoThrow(const char *str) {
    return stringToTypeNoThrow<u8string>(str);
}

inline u8string stringToUTF8(const std::wstring& str) {
    LOCALE_CONVERT_FILL(std::wstring, str, u8string, UTF8);
}
inline u8string stringToUTF8NoThrow(const std::wstring& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u8string, UTF8);
}
inline u8string stringToUTF8(const wchar_t *str) {
    LOCALE_CONVERT_FILL(wchar_t*, str, u8string, UTF8);
}
inline u8string stringToUTF8NoThrow(const wchar_t *str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u8string, UTF8);
}

inline u8string UTF16ToUTF8(const u16string& str) {
    LOCALE_CONVERT_FILL(u16string, str, u8string, UTF8);
}
inline u8string UTF16ToUTF8NoThrow(const u16string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u8string, UTF8);
}
inline u8string UTF32ToUTF8(const u32string& str) {
    LOCALE_CONVERT_FILL(u32string, str, u8string, UTF8);
}
inline u8string UTF32ToUTF8NoThrow(const u32string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u8string, UTF8);
}

// Mask utf8 to utf8
#if CHAR_MAX != 0xFF
inline u8string stringToUTF8(const u8string& str) {
    return str;
}
inline u8string stringToUTF8NoThrow(const u8string& str) {
    return str;
}
inline u8string stringToUTF8(const UTF8 *str) {
    return u8string(str);
}
inline u8string stringToUTF8NoThrow(const UTF8 *str) {
    return u8string(str);
}
#endif
// Mask utf16 to utf8
#if CHAR_MAX != 0xFFFF
inline u8string stringToUTF8(const u16string& str) {
    return UTF16ToUTF8(str);
}
inline u8string stringToUTF8NoThrow(const u16string& str) {
    return UTF16ToUTF8NoThrow(str);
}
inline u8string stringToUTF8(const UTF16 *str) {
    return UTF16ToUTF8(str);
}
inline u8string stringToUTF8NoThrow(const UTF16 *str) {
    return UTF16ToUTF8NoThrow(str);
}
#endif
// Mask utf32 to utf8
#if CHAR_MAX != 0xFFFFFFFF
inline u8string stringToUTF8(const u32string& str) {
    return UTF32ToUTF8(str);
}
inline u8string stringToUTF8NoThrow(const u32string& str) {
    return UTF32ToUTF8NoThrow(str);
}
inline u8string stringToUTF8(const UTF32 *str) {
    return UTF32ToUTF8(str);
}
inline u8string stringToUTF8NoThrow(const UTF32 *str) {
    return UTF32ToUTF8NoThrow(str);
}
#endif

/*
 * Conversion renaming for ease of use with UTF16.
 */
inline u16string stringToUTF16(const std::string& str) {
    return stringToType<u16string>(str);
}
inline u16string stringToUTF16NoThrow(const std::string& str) {
    return stringToTypeNoThrow<u16string>(str);
}
inline u16string stringToUTF16(const char *str) {
    return stringToType<u16string>(str);
}
inline u16string stringToUTF16NoThrow(const char *str) {
    return stringToTypeNoThrow<u16string>(str);
}

inline u16string stringToUTF16(const std::wstring& str) {
    LOCALE_CONVERT_FILL(std::wstring, str, u16string, UTF16);
}
inline u16string stringToUTF16NoThrow(const std::wstring& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u16string, UTF16);
}
inline u16string stringToUTF16(const wchar_t *str) {
    LOCALE_CONVERT_FILL(wchar_t*, str, u16string, UTF16);
}
inline u16string stringToUTF16NoThrow(const wchar_t *str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u16string, UTF16);
}

inline u16string UTF8ToUTF16(const u8string& str) {
    LOCALE_CONVERT_FILL(u8string, str, u16string, UTF16);
}
inline u16string UTF8ToUTF16NoThrow(const u8string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u16string, UTF16);
}
inline u16string UTF32ToUTF16(const u32string& str) {
    LOCALE_CONVERT_FILL(u32string, str, u16string, UTF16);
}
inline u16string UTF32ToUTF16NoThrow(const u32string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u16string, UTF16);
}

// Mask utf8 to utf16
#if CHAR_MAX != 0xFF
inline u16string stringToUTF16(const u8string& str) {
    return UTF8ToUTF16(str);
}
inline u16string stringToUTF16NoThrow(const u8string& str) {
    return UTF8ToUTF16NoThrow(str);
}
inline u16string stringToUTF16(const UTF8 *str) {
    return UTF8ToUTF16(str);
}
inline u16string stringToUTF16NoThrow(const UTF8 *str) {
    return UTF8ToUTF16NoThrow(str);
}
#endif
// Mask utf16 to utf16
#if CHAR_MAX != 0xFFFF
inline u16string stringToUTF16(const u16string& str) {
    return str;
}
inline u16string stringToUTF16NoThrow(const u16string& str) {
    return str;
}
inline u16string stringToUTF16(const UTF16 *str) {
    return u16string(str);
}
inline u16string stringToUTF16NoThrow(const UTF16 *str) {
    return u16string(str);
}
#endif
// Mask utf32 to utf16
#if CHAR_MAX != 0xFFFFFFFF
inline u16string stringToUTF16(const u32string& str) {
    return UTF32ToUTF16(str);
}
inline u16string stringToUTF16NoThrow(const u32string& str) {
    return UTF32ToUTF16NoThrow(str);
}
inline u16string stringToUTF16(const UTF32 *str) {
    return UTF32ToUTF16(str);
}
inline u16string stringToUTF16NoThrow(const UTF32 *str) {
    return UTF32ToUTF16NoThrow(str);
}
#endif

/*
 * Conversion renaming for ease of use with UTF32.
 */
inline u32string stringToUTF32(const std::string& str) {
    return stringToType<u32string>(str);
}
inline u32string stringToUTF32NoThrow(const std::string& str) {
    return stringToTypeNoThrow<u32string>(str);
}
inline u32string stringToUTF32(const char *str) {
    return stringToType<u32string>(str);
}
inline u32string stringToUTF32NoThrow(const char *str) {
    return stringToTypeNoThrow<u32string>(str);
}

inline u32string stringToUTF32(const std::wstring& str) {
    LOCALE_CONVERT_FILL(std::wstring, str, u32string, UTF32);
}
inline u32string stringToUTF32NoThrow(const std::wstring& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u32string, UTF32);
}
inline u32string stringToUTF32(const wchar_t *str) {
    LOCALE_CONVERT_FILL(wchar_t*, str, u32string, UTF32);
}
inline u32string stringToUTF32NoThrow(const wchar_t *str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u32string, UTF32);
}

inline u32string UTF8ToUTF32(const u8string& str) {
    LOCALE_CONVERT_FILL(u8string, str, u32string, UTF32);
}
inline u32string UTF8ToUTF32NoThrow(const u8string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u32string, UTF32);
}
inline u32string UTF16ToUTF32(const u16string& str) {
    LOCALE_CONVERT_FILL(u16string, str, u32string, UTF32);
}
inline u32string UTF16ToUTF32NoThrow(const u16string& str) {
    LOCALE_CONVERT_FILL_NO_THROW(str, u32string, UTF32);
}

// Mask utf8 to utf32
#if CHAR_MAX != 0xFF
inline u32string stringToUTF32(const u8string& str) {
    return UTF8ToUTF32(str);
}
inline u32string stringToUTF32NoThrow(const u8string& str) {
    return UTF8ToUTF32NoThrow(str);
}
inline u32string stringToUTF32(const UTF8 *str) {
    return UTF8ToUTF32(str);
}
inline u32string stringToUTF32NoThrow(const UTF8 *str) {
    return UTF8ToUTF32NoThrow(str);
}
#endif
// Mask utf16 to utf32
#if CHAR_MAX != 0xFFFF
inline u32string stringToUTF32(const u16string& str) {
    return UTF16ToUTF32(str);
}
inline u32string stringToUTF32NoThrow(const u16string& str) {
    return UTF16ToUTF32NoThrow(str);
}
inline u32string stringToUTF32(const UTF16 *str) {
    return UTF16ToUTF32(str);
}
inline u32string stringToUTF32NoThrow(const UTF16 *str) {
    return UTF16ToUTF32NoThrow(str);
}
#endif
// Mask utf32 to utf32
#if CHAR_MAX != 0xFFFFFFFF
inline u32string stringToUTF32(const u32string& str) {
    return str;
}
inline u32string stringToUTF32NoThrow(const u32string& str) {
    return str;
}
inline u32string stringToUTF32(const UTF32 *str) {
    return u32string(str);
}
inline u32string stringToUTF32NoThrow(const UTF32 *str) {
    return u32string(str);
}
#endif

/*
 * These must go at the end of the file.
 */
inline std::string operator <<(
        StrStreamerPtr builder, const StrStreamEnder& convertable) {
    return builder->buildString();
}
template<typename T>
inline StrStreamerPtr operator <<(
        StrStreamerPtr builder, const T& convertable) {
    builder->stracker.push_back(toString(convertable));
    return StrStreamerPtr(builder.release());
}
inline StrStreamerPtr operator <<(
        StrStreamerPtr builder, const std::string& convertable) {
    builder->stracker.push_back(&convertable);
    return StrStreamerPtr(builder.release());
}
}

// Get rid of this macro, it was only temporarily here
#undef throwCastException

#endif /* STRING_UTIL_H_ */
