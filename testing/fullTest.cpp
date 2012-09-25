
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE CoreTestSuite
#include <boost/test/unit_test.hpp>

#include <stdio.h>
#include "testNumUtil.h"
#include "testTSQueue.h"

namespace core {

int add(int i, int j)
{
    return i + j;
}

/*
BOOST_AUTO_TEST_CASE(runAllTests) {
    core::threading::container::TSWrapper<int> intWrapTest;
    intWrapTest.lock();
    intWrapTest.unlock();

    std::cout << "TEST" << "\n";

    BOOST_CHECK(add(2,2) == 4);

    std::cout << std::endl;

    //core::threading::WriteMutex wmutex();
    //core::threading::ReadWriteMutex rwmutex();

    //core::threading::ResourceLocker rLocker();
     *
    std::string hello("\nBoost World!\n");
    forEach(char ch, hello) {
        std::cout << ch;
    }
    reverseForEach(char ch, hello) {
        std::cout << ch;
    }
    unsigned int value, value2;
    std::string teststr( "test" );
    char ch;
    enumerateEach(value, ch, teststr) reverseEnumerateEach(value2, char ch2, teststr)
        std::cout << value << " => " << ch << ":" << ch2 << " => " << value2 << "\n";

    forRepeat(11) {
        std::cout << "Value: " << ++value << "\n";
    }

    onEach( ch, testCh, hello );
    onEach( testCh, hello );
    onEach( ch, testChRef, hello );
    onEach( testChRef, hello );
    reverseOnEach( ch, testCh, hello );
    reverseOnEach( testCh, hello );
    reverseOnEach( ch, testChRef, hello );
    reverseOnEach( testChRef, hello );
    std::cout << '\n' << std::flush;

    getLoggingManager().logDebug("Running All Toolkit Tests...");
    if (!testExceptions()) {
        getLoggingManager().logDebug("Toolkit Exceptions Tests Failed. Testing Stopped.");
        getLoggingManager().flushLogs();
    }

    if (!testTSQueue()) {
        getLoggingManager().logDebug("Toolkit TSQueue Tests Failed. Testing Stopped.");
        getLoggingManager().flushLogs();
    }

    if (!testGenericCommands()) {
        getLoggingManager().logDebug("Toolkit Generic Functions Tests Failed. Testing Stopped.");
        getLoggingManager().flushLogs();
    }

    getLoggingManager().logDebug("All Toolkit Tests Finished With No Errors.");
    getLoggingManager().flushLogs();
}
*/

}
//EOF
