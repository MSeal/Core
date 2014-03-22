#include "stringutil.hpp"

namespace core {
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
