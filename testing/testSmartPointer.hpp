/*
 * Tests the correctness of various numutil functions.
 */

#ifndef TEST_SMART_POINTER_H_
#define TEST_SMART_POINTER_H_

#include "pointers.hpp"
#include <boost/test/unit_test.hpp>

namespace core {
BOOST_AUTO_TEST_SUITE(SmartPointers)

/*
 * Custom class to to test SharedPtr
 */

class DeleteCheck {
public:
    bool& qbool;
    DeleteCheck(bool& qbool) : qbool(qbool) {}
    ~DeleteCheck() {qbool = true;}
};

class IntrusiveCheck {
public:
    std::size_t counter;
    int object;

    IntrusiveCheck() : counter(0) {}
    ~IntrusiveCheck() {std::cout<<"Deleted";}

    std::size_t refCount() {
        return counter;
    }

    void set(int i) {
        object = i;
    }

    int get() {
        return object;
    }

    void refInc() {
        counter++;
    }

    void refDec() {
        counter--;
    }
};

void intrusive_ptr_add_ref(IntrusiveCheck* ptr) {
    ptr->refInc();
}

void intrusive_ptr_release(IntrusiveCheck* ptr) {
    ptr->refDec();
    if (ptr->refCount() == 0) {
        delete ptr;
    }
}

typedef typename core::pointers::smart<int>::SharedPtr IntPtr;
typedef typename core::pointers::smart<std::string>::SharedPtr StrPtr;
typedef typename core::pointers::smart<DeleteCheck>::SharedPtr CheckPtr;

typedef typename core::pointers::smart<int>::ScopedPtr ScopedIntPtr;
typedef typename core::pointers::smart<std::string>::ScopedPtr ScopedStrPtr;
typedef typename core::pointers::smart<DeleteCheck>::ScopedPtr ScopedCheckPtr;

typedef typename core::pointers::smart<int>::WeakPtr WIntPtr;
typedef typename core::pointers::smart<std::string>::WeakPtr WStrPtr;

typedef typename core::pointers::smart<IntrusiveCheck>::IntrusivePtr IntrusivePtr;




/*
 * Creates a set of smart<int> pointers from the template
 * then confirms those pointers have been properly assigned.
 * Tests both assignment to a new int, and assignment to the
 * target of an existing pointer for aplicable pointer types.
 */

BOOST_AUTO_TEST_CASE(pointerCreation) {
    int num1 = 1;
    IntPtr ptr1(new int);
    *ptr1 = num1;
    BOOST_CHECK_EQUAL(*ptr1, num1);

    int num2 = 2;
    IntPtr ptr2 = ptr1;
    *ptr2 = num2;
    BOOST_CHECK_EQUAL(*ptr2, num2);
    BOOST_CHECK_EQUAL(*ptr1, num2);

    ScopedIntPtr scopedPtr(new int);
    *scopedPtr = 1;
    BOOST_CHECK_EQUAL(*scopedPtr, 1);

    IntPtr sharedPtr(new int);
    WIntPtr weakPtr(sharedPtr);
    *sharedPtr = 1;
    IntPtr sharedPtr1(weakPtr);
    BOOST_CHECK_EQUAL(*sharedPtr1, 1);

    IntrusivePtr intruPtr(new IntrusiveCheck);
    intruPtr->set(1);
    std::size_t count = intruPtr->refCount();
    BOOST_CHECK_EQUAL(count, 1);
    BOOST_CHECK_EQUAL(intruPtr->get(), 1);

    IntrusivePtr intruPtr2(intruPtr);
    intruPtr2->set(2);
    count = intruPtr2->refCount();
    BOOST_CHECK_EQUAL(count, 2);
    BOOST_CHECK_EQUAL(intruPtr->get(), 2);

}
/*
 * Tests the creation and assignment of smart
 * pointers with the string template. Tests for both assignment
 * to a base string and assignment to the target of an existing
 * pointer.
 */

BOOST_AUTO_TEST_CASE(stringPointerCreation) {
    std::string str1 = "This is a string.";
    StrPtr ptr1(new std::string);
    *ptr1 = str1;
    BOOST_CHECK_EQUAL(ptr1->compare(str1), 0);

    std::string str2 = "This is also a string.";
    StrPtr ptr2(new std::string);
    ptr2 = ptr1;
    *ptr2 = str2;
    BOOST_CHECK_EQUAL(ptr2->compare(str2), 0);
    BOOST_CHECK_EQUAL(ptr1->compare(str2), 0);

    ScopedStrPtr ScopedPtr(new std::string);
    std::string str3 = "This is still a string.";
    *ScopedPtr = str3;
    BOOST_CHECK_EQUAL(ScopedPtr->compare(str3), 0);

    StrPtr sharedPtr(new std::string);
    WStrPtr weakPtr(sharedPtr);
    std::string str4 = "This is a string.";
    *sharedPtr = str4;
    StrPtr sharedPtr1(weakPtr);
    BOOST_CHECK_EQUAL(sharedPtr1->compare(str4), 0);

}

/*
 * Helper function to test deleting custom
 * pointers.
 */

void checkDelete (bool& qbool) {
    DeleteCheck ptr1(qbool);
}

/*
 * Helper function to check that pointers
 * are not deleted when they remain in scope.
 */

DeleteCheck checkNoDelete (bool& qbool) {
    DeleteCheck ptr1(qbool);
    return ptr1;
}

/*
 * Checks to ensure that custom pointers delete
 * when out of scope.
 */

BOOST_AUTO_TEST_CASE(checkPointerCreation) {
    bool qbool = false;
    checkDelete(qbool);
    BOOST_CHECK_EQUAL(qbool, true);

    qbool = false;
    DeleteCheck ptr2 = checkNoDelete(qbool);
    BOOST_CHECK_EQUAL(qbool, false);
}

/*
 * Stuff to fill in later....
 */

BOOST_AUTO_TEST_SUITE_END()

}

#endif /* TEST_NUM_UTIL_H_ */
