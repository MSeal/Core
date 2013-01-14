/*
 * ssutil.h
 */
#ifndef STRING_STREAM_UTIL_H_
#define STRING_STREAM_UTIL_H_

#include <vector>
#include <sstream>
#include <boost/variant.hpp>
#include "loops.hpp"
#include "pointers.hpp"

namespace core {
/*
 * Forward declare these for specific template implementations.
 * These templates are implemented in stringutil
 */
template<typename T> inline std::string toString(T castable);
inline std::string toString(const std::type_info& castable);
inline std::string toString(const std::type_info *castable);
template<typename T> inline T stringToType(const std::string& str);
template<typename T> inline std::string operator <<(const std::string& a, const T& b);

class StrStreamEndBuilder;
typedef pointers::smart<StrStreamEndBuilder>::UniquePtr StrStreamEndBuilderPtr;
class StrStreamEndBuilder {
    template<typename T>
    friend StrStreamEndBuilderPtr operator <<
            (const T& convertable, StrStreamEndBuilderPtr builder);
    friend StrStreamEndBuilderPtr operator <<
            (const std::string& convertable, StrStreamEndBuilderPtr builder);
    friend StrStreamEndBuilderPtr operator <<
            (const char *convertable, StrStreamEndBuilderPtr builder);
    template<typename T>
    friend StrStreamEndBuilderPtr operator +
            (const T& convertable, StrStreamEndBuilderPtr builder);
    friend StrStreamEndBuilderPtr operator +
            (const std::string& convertable, StrStreamEndBuilderPtr builder);
    friend StrStreamEndBuilderPtr operator +
            (const char *convertable, StrStreamEndBuilderPtr builder);

private:
    typedef boost::variant<const std::string *, std::string> stringTypes;
    std::vector<stringTypes> stracker;
    // Used in case of error
    static const std::string emptyStr;

    /*
     * Converts stringTypes to a string reference.
     */
    const std::string& getTypeReference(stringTypes& strin);

public:
    StrStreamEndBuilder() : stracker(std::vector<stringTypes>::size_type(128)) {}

    /*
     * Builds the string seen so far by streams. The string is build
     * in reverse order to what was seen to reflect FILO ordering.
     */
    std::string buildString();
};

/*
 * Used to create the strstart object, which is a static const
 * object with no internal state. The object is only used to
 * make syntax of string creation from various starting types
 * easy.
 */
struct StrStreamBeginBuilder {
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
/*
 * Create a constant string starter in our cpp -- no state
 * just a helper for starting string assignments.
 */
extern const StrStreamBeginBuilder strstart;

/*
 * Define all of the stream operators with builders on the right-hand side
 */
template<typename T>
inline StrStreamEndBuilderPtr operator <<(const T& convertable, StrStreamEndBuilderPtr builder) {
    builder->stracker.push_back(toString(convertable));
    return StrStreamEndBuilderPtr(builder.release());
}
inline StrStreamEndBuilderPtr operator <<(const std::string& convertable, StrStreamEndBuilderPtr builder) {
    builder->stracker.push_back(&convertable);
    return StrStreamEndBuilderPtr(builder.release());
}
inline StrStreamEndBuilderPtr operator <<(const char *convertable, StrStreamEndBuilderPtr builder) {
    // We have to count the size of the char* anyway, so just convert
    builder->stracker.push_back(toString(convertable));
    return StrStreamEndBuilderPtr(builder.release());
}
// Allow '+' operators to do the same computation
template<typename T>
inline ::core::StrStreamEndBuilderPtr operator +(const T& convertable, ::core::StrStreamEndBuilderPtr builder) {
    builder->stracker.push_back(toString(convertable));
    return StrStreamEndBuilderPtr(builder.release());
}
inline ::core::StrStreamEndBuilderPtr operator +(const std::string& convertable, ::core::StrStreamEndBuilderPtr builder) {
    builder->stracker.push_back(&convertable);
    return StrStreamEndBuilderPtr(builder.release());
}
inline ::core::StrStreamEndBuilderPtr operator +(const char *convertable, ::core::StrStreamEndBuilderPtr builder) {
    // We have to count the size of the char* anyway, so just convert
    builder->stracker.push_back(toString(convertable));
    return StrStreamEndBuilderPtr(builder.release());
}

}

#endif /* STRING_STREAM_UTIL_H_ */
