/*
 * Tests the performance of TSQueue class. If the class fails it will
 * throw an exception, indicating where failure occured.
 */

#ifndef TEST_ENVIRONMENT_TSQUEUE_H_
#define TEST_ENVIRONMENT_TSQUEUE_H_

#include "threading/container/tsqueue.h"
#include "threading/thread.h"
#include "stringutil.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>
#include <boost/test/unit_test.hpp>

typedef core::threading::container::TSQueue<int> IntQueue;
using core::threading::Thread;
using core::threading::ThreadPtr;

namespace core {
BOOST_AUTO_TEST_SUITE(TSQueue)

/* Enqueues some ints */
void enqueueInts(IntQueue& intQ, unsigned int numEnqueue) {
    int i = 0;
    BOOST_TEST_MESSAGE("Enqueuing " << numEnqueue << " ints to TSQueue");
    for (i = 0; (unsigned int)i < numEnqueue; i++) {
        intQ.enqueue(i);
    }
    BOOST_REQUIRE_EQUAL(intQ.size(), numEnqueue);
}

/* Dequeue some ints */
void dequeueInts(IntQueue& intQ, unsigned int numEnqueued) {
    int i = 0;
    int j = 0;
    BOOST_REQUIRE_EQUAL(intQ.size(), numEnqueued);
    BOOST_TEST_MESSAGE("Dequeuing " << numEnqueued << " ints from TSQueue");
    for (i = 0; (unsigned int)i < numEnqueued; i++) {
        IntQueue::PtrType deq(intQ.dequeue());
        BOOST_REQUIRE_MESSAGE(deq, "No element dequeued");
        j = (*deq);
        BOOST_REQUIRE_MESSAGE(i == j,
                "Failed to return " << i << ", instead returned " << j);
    }
    BOOST_REQUIRE_MESSAGE(intQ.empty(),
            "Dequeue did not reduce Queue size");
    BOOST_REQUIRE_MESSAGE(!intQ.dequeue(),
            "Failed to return empty pointer");
}

/* Tests basic functionality */
BOOST_AUTO_TEST_CASE(tsQueueBasicFunctions) {
    IntQueue intQ;
    enqueueInts(intQ, 250);
    dequeueInts(intQ, 250);
}

/* Enqueues some Ts */
template<typename T>
void enqueueT(threading::container::TSQueue<T>& tester, T& addType, unsigned int numEnqueue) {
    BOOST_TEST_MESSAGE("Enqueuing " << numEnqueue << " T items to TSQueue");
    for (unsigned int i = 0; i < numEnqueue; i++) {
        tester.enqueue(addType);
    }
    BOOST_REQUIRE_EQUAL(tester.size(), numEnqueue);
}

/* Dequeues some Ts */
template<typename T>
void dequeueT(threading::container::TSQueue<T>& tester, unsigned int numEnqueue) {
	BOOST_TEST_MESSAGE("Dequeuing " << numEnqueue << " T items from TSQueue");
    for (unsigned int i = 0; i < numEnqueue; i++) {
        typename threading::container::TSQueue<T>::PtrType deq = tester.dequeue();
        BOOST_REQUIRE_MESSAGE(deq, "No element dequeued");
    }
    BOOST_REQUIRE_MESSAGE(tester.empty(),
                "Dequeue did not reduce Queue size");
    BOOST_REQUIRE_MESSAGE(!tester.dequeue(),
                "Failed to return empty pointer");
}

/* Tests a single Queue type (the type T needs to be copyable and addType not NULL) */
template<typename T>
void testSingleTSQueueType(T& addType) {
    threading::container::TSQueue<T> tester;
	enqueueT<T>(tester, addType, 100);
	dequeueT<T>(tester, 100);
}

/* Tests several types being added to a Queue */
BOOST_AUTO_TEST_CASE(tsQueueFloat) {
    BOOST_TEST_MESSAGE("Testing floats");
    float f = 0.123456F;
    testSingleTSQueueType<float>(f);
}

BOOST_AUTO_TEST_CASE(tsQueueString) {
    BOOST_TEST_MESSAGE("Testing strings");
    std::string test("test");
    testSingleTSQueueType<std::string>(test);
}

BOOST_AUTO_TEST_CASE(tsQueueSharedPtr) {
    BOOST_TEST_MESSAGE("Testing shared pointers");
    boost::shared_ptr<std::string> testPtr(new std::string("test"));
    testSingleTSQueueType<boost::shared_ptr<std::string> >(testPtr);
}

class NoConstruct {
public:
    std::string str;
    NoConstruct(std::string name) {
        str = name;
    }

private:
    NoConstruct() {}
};

BOOST_AUTO_TEST_CASE(tsQueuePrivateStruct) {
    BOOST_TEST_MESSAGE("Testing struct with private base constructor");
    NoConstruct nc = NoConstruct(std::string("test"));
    testSingleTSQueueType<NoConstruct>(nc);
}

/* Tests concurrency functionality */
/* Concurrency Testing */
void tsQueueEnqueueWorker(pointers<IntQueue>::SharedPtr testQInt, int workerNum) {
    // Do NOT use BOOST_TEST_MESSAGE here, it's not thread safe
    for (int i = 0; i < 100; i++) {
        testQInt->enqueue(i);
    }
}

void tsQueueDequeueWorker(pointers<IntQueue>::SharedPtr testQInt, int workerNum) {
    int i = 0;
    while (i < 100) {
        // Do NOT use BOOST_TEST_MESSAGE here, it's not thread safe
        if (testQInt->empty()) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        } else {
            if (testQInt->dequeue()) { i++; }
        }
    }
}

BOOST_AUTO_TEST_CASE(tsQueueConcurrency) {
    pointers<IntQueue>::SharedPtr intQ(new IntQueue());
    boost::posix_time::time_duration wait = boost::posix_time::milliseconds(500);
    // need to make this ugly vector of shared pointers to get around non-copyable interfaces
    pointers<Thread>::containers<>::PtrVector enthrds;
    pointers<Thread>::containers<>::PtrVector dethrds;
    int numEnqueueThreads = 10;
    intQ->clear();
    for(int i = 0; i < numEnqueueThreads; i++) {
        BOOST_TEST_MESSAGE("CHECK 1");
        enthrds.push_back(new Thread(boost::bind(&tsQueueEnqueueWorker, intQ, i)));
        BOOST_TEST_MESSAGE("Enqueue Worker " << i << " started");
        dethrds.push_back(new Thread(boost::bind(&tsQueueDequeueWorker, intQ, i)));
        BOOST_TEST_MESSAGE("Dequeue Worker " << i << " started");
    }
    BOOST_TEST_MESSAGE("CHECK 2");
    for(int j = 0; j < numEnqueueThreads; j++) {
        if (!enthrds[j].timed_join(wait)) {
            BOOST_FAIL("Thread timed out");
        }
        BOOST_TEST_MESSAGE("Enqueue Worker " << j << " finished");
    }
    for(int j = 0; j < numEnqueueThreads; j++) {
        if (!dethrds[j].timed_join(wait)) {
            BOOST_FAIL("Thread timed out");
        }
        BOOST_TEST_MESSAGE("Dequeue Worker " << j << " finished");
    }
    std::cout << intQ->size();

    BOOST_REQUIRE_MESSAGE(intQ->empty(),
            "Dequeue did not reduce Queue size during concurrency test");

    dethrds.push_back(new Thread(boost::bind(&tsQueueDequeueWorker, intQ, numEnqueueThreads)));
    BOOST_TEST_MESSAGE("Dequeue Worker " << numEnqueueThreads << " Started");
    enthrds.push_back(new Thread(boost::bind(&tsQueueEnqueueWorker, intQ, numEnqueueThreads)));
    BOOST_TEST_MESSAGE("Enqueue Worker " << numEnqueueThreads << " Started");
    enthrds.push_back(new Thread(boost::bind(&tsQueueEnqueueWorker, intQ, numEnqueueThreads+1)));
    BOOST_TEST_MESSAGE("Enqueue Worker " << numEnqueueThreads+1 << " Started");
    enthrds.push_back(new Thread(boost::bind(&tsQueueEnqueueWorker, intQ, numEnqueueThreads+2)));
    BOOST_TEST_MESSAGE("Enqueue Worker " << numEnqueueThreads+2 << " Started");

    for(int k = numEnqueueThreads; k < numEnqueueThreads+3; k++) {
        if (!enthrds[k].timed_join(wait)) {
            BOOST_FAIL("Thread timed out");
        }
        BOOST_TEST_MESSAGE("TSQueueWorker " << k << " finished");
    }

    BOOST_REQUIRE_MESSAGE(!intQ->empty(),
            "Enqueue did not increase Queue size during concurrency test");
    intQ->clear();
    BOOST_REQUIRE_MESSAGE(intQ->empty(), "Clear did not reduce Queue size");
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif
