#include "detail/string_builders.hpp"
#include "loops.hpp"

namespace core {
const std::string StrStreamer::emptyStr = std::string();
const StrStreamStarter strstarter = StrStreamStarter();
const StrStreamEnder strender = StrStreamEnder();

class stringsTypeVisitor : public boost::static_visitor<const std::string&> {
public:
    const std::string& operator()(const std::string *& str) const {
        if (str == NULL) {
            return StrStreamer::emptyStr;
        } else {
            return *str;
        }
    }
    const std::string& operator()(std::string& str) const {
        return str;
    }
};

/*
 * Converts StringTypes to a string reference.
 */
const std::string& StrStreamer::getStrRef(StrStreamer::StringTypes& strin) {
    return boost::apply_visitor(stringsTypeVisitor(), strin);
}

/*
 * Builds the string seen so far by streams.
 */
std::string StrStreamer::buildString() {
    // First get the length of the final string
    std::size_t length = 0;
    forEach(StrStreamer::StringTypes& strref, stracker) {
        length += getStrRef(strref).length();
    }

    // Now build our string
    std::string outstr;
    outstr.reserve(length);
    forEach(StrStreamer::StringTypes& strref, stracker) {
        outstr += getStrRef(strref);
    }

    // Cleanup our tracker
    stracker.clear();
    return outstr;
}
}