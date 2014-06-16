/*
 * testFactory.hpp
 * Tests the capability to encapsulate objects behind a factory.
 */

#ifndef TEST_FACTORY_HPP_
#define TEST_FACTORY_HPP_

#include "factory.hpp"
#include "exceptions.hpp"
#include <boost/test/unit_test.hpp>

namespace core {
BOOST_AUTO_TEST_SUITE(FactoryTests)

struct TestBlob {
    int assigned;
};

class BlobBuilder {
public:
    int assignable;
    BlobBuilder(int value) : assignable(value) {}

    TestBlob *build() {
        TestBlob *tblob = new TestBlob();
        tblob->assigned = assignable;
        return tblob;
    }

    // Used by tracked producer
    TestBlob *build(const int adjustment) {
        TestBlob *tblob = new TestBlob();
        tblob->assigned = assignable + adjustment;
        return tblob;
    }
};
class TestFactory : public Factory<const TestBlob, BlobBuilder> {
public:
    explicit TestFactory(int value) : Factory(BlobBuilder(value)) {}
};

class TestTrackedFactory : public TrackedFactory<const int, TestBlob, BlobBuilder> {
public:
    explicit TestTrackedFactory(int value) : TrackedFactory(BlobBuilder(value)) {}
};
// For easy reference elsewhere
typedef TestTrackedFactory::TPtr BlobPtr;

BOOST_AUTO_TEST_CASE(basicFactory) {
    TestFactory fact1(42);
    TestFactory fact2(314);

    BOOST_CHECK_EQUAL(fact1.produce()->assigned, 42);
    BOOST_CHECK_EQUAL(fact2.produce()->assigned, 314);
    BOOST_CHECK_EQUAL(fact1.produce()->assigned, 42);
}

BOOST_AUTO_TEST_CASE(trackedFactoryProduce) {
    TestTrackedFactory fact(42);

    // Keys can affect produced results
    BlobPtr bptr = fact.produce(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);
    BOOST_CHECK_THROW(fact.produce(5), InsertFailedException);
}

BOOST_AUTO_TEST_CASE(trackedFactoryGet) {
    TestTrackedFactory fact(42);

    BOOST_CHECK_THROW(fact.get(5), AttributeException);

    BlobPtr bptr = fact.produce(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);

    // Ensure that tracking retrieves the same object
    bptr = fact.get(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);
    bptr->assigned = 1;
    bptr = fact.get(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 1);
}

BOOST_AUTO_TEST_CASE(trackedFactoryGetOrProduce) {
    TestTrackedFactory fact(42);

    // getOrProduce works if item is present or not
    BlobPtr bptr = fact.getOrProduce(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);
    bptr->assigned = 5;
    bptr = fact.getOrProduce(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 5);
}

BOOST_AUTO_TEST_CASE(trackedFactoryTryGet) {
    TestTrackedFactory fact(42);

    BlobPtr bptr = fact.tryGet(5);
    BOOST_CHECK(!bptr);

    fact.produce(5);
    bptr = fact.tryGet(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);
}

BOOST_AUTO_TEST_CASE(trackedFactoryDrop) {
    TestTrackedFactory fact(42);

    BOOST_CHECK_THROW(fact.drop(5), AttributeException);

    BlobPtr bptr = fact.produce(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);
    bptr->assigned = 1;
    fact.drop(5);

    bptr = fact.tryGet(5);
    BOOST_CHECK(!bptr);
}

BOOST_AUTO_TEST_CASE(trackedFactoryTryDrop) {
    TestTrackedFactory fact(42);
    BOOST_CHECK(!fact.tryDrop(5));

    BlobPtr bptr = fact.produce(5);
    BOOST_CHECK_EQUAL(bptr->assigned, 47);
    bptr->assigned = 1;
    BOOST_CHECK(fact.tryDrop(5));

    bptr = fact.tryGet(5);
    BOOST_CHECK(!bptr);
}

BOOST_AUTO_TEST_SUITE_END()
}
#endif /* TEST_FACTORY_HPP_ */
