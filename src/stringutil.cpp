#include "stringutil.hpp"

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
 * Converts stringTypes to a string reference.
 */
const std::string& StrStreamer::getStrRef(stringTypes& strin) {
    return boost::apply_visitor(stringsTypeVisitor(), strin);
}

/*
 * Builds the string seen so far by streams.
 */
std::string StrStreamer::buildString() {
    // First get the length of the final string
    std::size_t length = 0;
    forEach(stringTypes& strref, stracker) {
        length += getStrRef(strref).length();
    }

    // Now build our string
    std::string outstr;
    outstr.reserve(length);
    forEach(stringTypes& strref, stracker) {
        outstr += getStrRef(strref);
    }

    // Cleanup our tracker
    stracker.clear();
    return outstr;
}

/*
 * Type: boost::exception
 */
void extractExceptionMessage(const boost::exception& x, std::string& parseMessage) {
    const std::string message = getExceptionMessage(x);

    if (!message.empty()) {
        parseMessage += " \"";
        parseMessage += message;
        parseMessage += "\"";
    }
}
void extractExceptionCode(const boost::exception& x, std::string& parseMessage) {
    ExceptionCode code = getExceptionCode(x);

    parseMessage += " (Code ";
    parseMessage += toString(code);
    parseMessage += " = ";
    parseMessage += toString(code);
    parseMessage += ")";
}

void extractExceptionFunction(const boost::exception& x, std::string& parseMessage) {
    const std::string& function = getExceptionFunction(x);

    if (!function.empty()) {
        parseMessage += " at ";
        parseMessage += function;
    }
}
void extractExceptionFileAndLine(const boost::exception& x, std::string& parseMessage) {
    const std::string file = getExceptionFileName(x);
    const std::string line = toString(getExceptionLineNumber(x));

    if (!file.empty()) {
        parseMessage += " with File ";
        parseMessage += file;

        if (!line.empty()) {
            parseMessage += " (line:";
            parseMessage += line;
            parseMessage += ")";
        }
    }
}

std::string toString(const boost::exception& x) {
    // Severity, code, rest
    ExceptionSeverity severity = getExceptionSeverity(x);
    std::string parseMessage = toString(severity);

    extractExceptionCode(x, parseMessage);
    parseMessage += ":";

    extractExceptionFunction(x, parseMessage);
    extractExceptionMessage(x, parseMessage);
    extractExceptionFileAndLine(x, parseMessage);

    return parseMessage;
}

std::string toString(const std::exception& x) {
    return x.what();
}
}
