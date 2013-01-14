#include "detail/ssutil.hpp"

namespace core {
const std::string StrStreamEndBuilder::emptyStr = std::string();
const StrStreamBeginBuilder strstart = StrStreamBeginBuilder();

/*
 * Converts stringTypes to a string reference.
 */
const std::string& StrStreamEndBuilder::getTypeReference(stringTypes& strin) {
    if (const std::string *& strout = boost::get<const std::string *>(strin)) {
        return *strout;
    } else {
        try {
            std::string& strout = boost::get<std::string>(strin);
            return strout;
        } catch (boost::bad_get be) {
            return emptyStr;
        }
    }
}

/*
 * Builds the string seen so far by streams. The string is build
 * in reverse order to what was seen to reflect FILO ordering.
 */
std::string StrStreamEndBuilder::buildString() {
    // First get the length of the final string
    std::size_t length = 0;
    reverseForEach(stringTypes& strref, stracker) {
        length += getTypeReference(strref).length();
    }

    // Now build our string
    std::string outstr;
    outstr.reserve(length);
    reverseForEach(stringTypes& strref, stracker) {
        outstr += getTypeReference(strref);
    }

    // Cleanup our tracker
    stracker.clear();
    return outstr;
}
}
