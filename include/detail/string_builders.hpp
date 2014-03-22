/*
 * stringBuilders.hpp
 *
 * Helper file which defined streaming operators for use in exceptions
 * and aritrary input streams.
 */
#ifndef STRING_BUILDERS_H_
#define STRING_BUILDERS_H_

#include <boost/variant.hpp>
#include "pointers.hpp"

namespace core {

class StrStreamer;
typedef pointers::smart<StrStreamer>::UniquePtr StrStreamerPtr;

// Struct used to trigger buildString methods in StrStreamer.
struct StrStreamEnder{};

/*
 * The StrStreamer class is a string builder which incrementally
 * builds references to string objects until either the buildString
 * method is called or a StrStreamEnder struct is passed into
 * the stream operator. In either of these cases the string is
 * finalized and a string of the appropriate length is constructed
 * and returned.
 *
 * The class relies on the toString function defined in stringutil
 * to convert any type to a string. If the streamed object is already
 * a string, then a pointer to that string is generated instead of
 * copying the original string. Note that this means that if the a
 * string becomes deallocated between being passed into a streamer and
 * triggering to buildString method then undefined behavior will insue.
 */
class StrStreamer {
    // Used to end the stream and build a string
    friend inline std::string operator <<(
            StrStreamerPtr builder, const StrStreamEnder& convertable);
    template<typename T>
    friend inline StrStreamerPtr operator <<(
            StrStreamerPtr builder, const T& convertable);
    friend inline StrStreamerPtr operator <<(
            StrStreamerPtr builder, const std::string& convertable);

private:
    typedef boost::variant<const std::string *, std::string> StringTypes;
    std::vector<StringTypes> stracker;

    // Converts StringTypes to a string reference.
    const std::string& getStrRef(StringTypes& strin);

public:
    // Used in case of error
    static const std::string emptyStr;
    StrStreamer(const size_t reserve=128) : stracker() {
        stracker.reserve(std::vector<StringTypes>::size_type(reserve));
    }

    // Builds the string seen so far by streams.
    std::string buildString();
};

// Used to start a stream without an explicit constructor calls
struct StrStreamStarter {
    template <typename T>
    StrStreamerPtr operator <<(const T& b) const {
        return StrStreamerPtr(new StrStreamer()) << b;
    }
};

inline std::string operator <<(StrStreamerPtr builder, const StrStreamEnder& convertable) {
    return builder->buildString();
}
template<typename T>
inline StrStreamerPtr operator <<(StrStreamerPtr builder, const T& convertable) {
    builder->stracker.push_back(toString(convertable));
    return StrStreamerPtr(builder.release());
}
inline StrStreamerPtr operator <<(StrStreamerPtr builder, const std::string& convertable) {
    builder->stracker.push_back(&convertable);
    return StrStreamerPtr(builder.release());
}

/*
 * Since starters and enders are stateless objects, there are
 * some constant constructed versions available.
 */
extern const StrStreamStarter strstarter;
extern const StrStreamEnder strender;

}

#endif /* STRING_BUILDERS_H_ */
