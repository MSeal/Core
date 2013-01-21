/*
 * testPPTypes.hpp
 */

#ifndef TEST_PPTYPES_H_
#define TEST_PPTYPES_H_

#include <boost/test/unit_test.hpp>
#include <vector>
#include "pptypes.hpp"

namespace core {
BOOST_AUTO_TEST_SUITE(PPTypes)

// Various classes with or without 'test' member
class With {
public:
    void test(int, long) const {}
};

class WithInherit : public With {};

class WithPrivate {
private:
    void test(int, long) const {}
};

class WithPrivateInherit : private With {};

class WithInt {
    int test;
};

class WithHidden : public With {
public:
    WithHidden(int) : With() {}
};

class WithTemplate {
public:
    template <typename T, typename U>
    void test(T, U){}
};

class WithTemplateInherit : public WithTemplate {};

class WithTemplateInheritPrivate : private WithTemplate {};

class Without {};

struct WithoutInherit : Without {};

class WithFromWithout : Without {
    template <typename T, typename U>
    void test(T, U){}
};

class WithMultiple {
    int test;
    bool bar;
};

// The magic template function generator
CREATE_MEMBER_CHECK(test);
// Repeated template generator for 'bar' name
CREATE_MEMBER_CHECK(bar);

/*
 * Tests the detector for members of a class.
 * The detector should be able to see private, template, inherited
 * members of any type (function, primitive, enum, etc...).
 */
BOOST_AUTO_TEST_CASE(hasMember) {
    bool fCheck;
    // For type that is defined as true_type for compile time checks
    bool fCheckCompile;
    bool barCheck;
    // For type that is defined as true_type for compile time checks
    bool barCheckCompile;

    // Primitive types
    fCheck = HasMember_test<int>::value;
    fCheckCompile = HasMember_test<int>::type::value;
    barCheck = HasMember_bar<int>::value;
    barCheckCompile = HasMember_bar<int>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<float>::value;
    fCheckCompile = HasMember_test<float>::type::value;
    barCheck = HasMember_bar<float>::value;
    barCheckCompile = HasMember_bar<float>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<char>::value;
    fCheckCompile = HasMember_test<char>::type::value;
    barCheck = HasMember_bar<char>::value;
    barCheckCompile = HasMember_bar<char>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    // Base types without required members
    fCheck = HasMember_test<std::string>::value;
    fCheckCompile = HasMember_test<std::string>::type::value;
    barCheck = HasMember_bar<std::string>::value;
    barCheckCompile = HasMember_bar<std::string>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<char *>::value;
    fCheckCompile = HasMember_test<char *>::type::value;
    barCheck = HasMember_bar<char *>::value;
    barCheckCompile = HasMember_bar<char *>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<std::vector<short> >::value;
    fCheckCompile = HasMember_test<std::vector<short> >::type::value;
    barCheck = HasMember_bar<std::vector<short> >::value;
    barCheckCompile = HasMember_bar<std::vector<short> >::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    // Test classes designed to trigger appropriately
    fCheck = HasMember_test<With>::value;
    fCheckCompile = HasMember_test<With>::type::value;
    barCheck = HasMember_bar<With>::value;
    barCheckCompile = HasMember_bar<With>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithPrivate>::value;
    fCheckCompile = HasMember_test<WithPrivate>::type::value;
    barCheck = HasMember_bar<WithPrivate>::value;
    barCheckCompile = HasMember_bar<WithPrivate>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithPrivateInherit>::value;
    fCheckCompile = HasMember_test<WithPrivateInherit>::type::value;
    barCheck = HasMember_bar<WithPrivateInherit>::value;
    barCheckCompile = HasMember_bar<WithPrivateInherit>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithInt>::value;
    fCheckCompile = HasMember_test<WithInt>::type::value;
    barCheck = HasMember_bar<WithInt>::value;
    barCheckCompile = HasMember_bar<WithInt>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithHidden>::value;
    fCheckCompile = HasMember_test<WithHidden>::type::value;
    barCheck = HasMember_bar<WithHidden>::value;
    barCheckCompile = HasMember_bar<WithHidden>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithInherit>::value;
    fCheckCompile = HasMember_test<WithInherit>::type::value;
    barCheck = HasMember_bar<WithInherit>::value;
    barCheckCompile = HasMember_bar<WithInherit>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithTemplate>::value;
    fCheckCompile = HasMember_test<WithTemplate>::type::value;
    barCheck = HasMember_bar<WithTemplate>::value;
    barCheckCompile = HasMember_bar<WithTemplate>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithTemplateInherit>::value;
    fCheckCompile = HasMember_test<WithTemplateInherit>::type::value;
    barCheck = HasMember_bar<WithTemplateInherit>::value;
    barCheckCompile = HasMember_bar<WithTemplateInherit>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithTemplateInheritPrivate>::value;
    fCheckCompile = HasMember_test<WithTemplateInheritPrivate>::type::value;
    barCheck = HasMember_bar<WithTemplateInheritPrivate>::value;
    barCheckCompile = HasMember_bar<WithTemplateInheritPrivate>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<Without>::value;
    fCheckCompile = HasMember_test<Without>::type::value;
    barCheck = HasMember_bar<Without>::value;
    barCheckCompile = HasMember_bar<Without>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithoutInherit>::value;
    fCheckCompile = HasMember_test<WithoutInherit>::type::value;
    barCheck = HasMember_bar<WithoutInherit>::value;
    barCheckCompile = HasMember_bar<WithoutInherit>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithFromWithout>::value;
    fCheckCompile = HasMember_test<WithFromWithout>::type::value;
    barCheck = HasMember_bar<WithFromWithout>::value;
    barCheckCompile = HasMember_bar<WithFromWithout>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_test<WithMultiple>::value;
    fCheckCompile = HasMember_test<WithMultiple>::type::value;
    barCheck = HasMember_bar<WithMultiple>::value;
    barCheckCompile = HasMember_bar<WithMultiple>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(barCheck);
    BOOST_CHECK(barCheckCompile);
}

struct Intable {
    // Keep non-explicit to allow strange
    // cast conversions of function return values
    Intable(int){};
};

class TestCallable {
public:
    int test(Intable, int* = 0) {
        return int();
    }
    void test(long*) const {}
    int test() {
        return 1;
    }
};

class TestCallableTemplate {
public:
    template <typename T> T& test(T test) const {
        return test;
    }
    template <typename T> T *test(T* test){
        return test;
    }
};

class Stringable {
public:
    std::string toString() const {
        return "";
    }
    int testInt() const {
        return 1;
    }
};

class StringableInherit : public Stringable {};

class BadReturnStringable {
public:
    int toString () {
        return 1;
    }
    std::string testInt() {
        return "";
    }
};

class NotStringable {
public:
    std::string toString;
    int testInt;
};

CREATE_MEMBER_FUNC_CHECK(test);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (void), primitive);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (const int), 1);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (int*), 2);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (long*), 3);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (long*) const, 4);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (const int), 5);
CREATE_MEMBER_FUNC_SIG_CHECK(test, Intable (const int), 6);
CREATE_MEMBER_FUNC_SIG_CHECK(test, Intable (const int) const, 7);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (Intable), 8);
CREATE_MEMBER_FUNC_SIG_CHECK(test, int& (const int), 9);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (int), 10);
CREATE_MEMBER_FUNC_SIG_CHECK(test, const void *(const int*), 11);
CREATE_MEMBER_FUNC_SIG_CHECK(test, int (int), 12);


BOOST_AUTO_TEST_CASE(hasMemberFunc) {
    bool funcPossible;
    // For type that is defined as true_type for compile time checks
    bool funcPossibleCompile;
    bool funcSigPossible;
    // For type that is defined as true_type for compile time checks
    bool funcSigPossibleCompile;

    // Primitive types
    funcPossible = IsMemberFunc_test<int, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_test<int, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_primitive<int>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_primitive<int>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<std::string, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_test<std::string, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_primitive<std::string>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_primitive<std::string>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<double, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_test<double, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_primitive<double>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_primitive<double>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<char *, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_test<char *, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_primitive<char *>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_primitive<char *>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<std::vector<float>, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_test<std::vector<float>, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_primitive<std::vector<float> >::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_primitive<std::vector<float> >::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    // Test classes designed to trigger appropriately
    funcPossible = IsMemberFunc_test<TestCallable, void (const int)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallable, void (const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_1<TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_1<TestCallable>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<TestCallable, void (int*)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallable, void (int*)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_2<TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_2<TestCallable>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_test<const TestCallable, void (long*)>::value;
    funcPossibleCompile = IsMemberFunc_test<const TestCallable, void (long*)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_3<const TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_3<const TestCallable>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<const TestCallable, void (long*) const>::value;
    funcPossibleCompile = IsMemberFunc_test<const TestCallable, void (long*) const>::type::value;
    funcSigPossible = IsMemberSigFunc_test_4<const TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_4<const TestCallable>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<const TestCallable, void (const int)>::value;
    funcPossibleCompile = IsMemberFunc_test<const TestCallable, void (const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_5<const TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_5<const TestCallable>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_test<TestCallable, Intable (const int)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallable, Intable (const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_6<TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_6<TestCallable>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<TestCallable, Intable (const int) const>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallable, Intable (const int) const>::type::value;
    funcSigPossible = IsMemberSigFunc_test_7<TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_7<TestCallable>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_test<TestCallable, void (Intable)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallable, void (Intable)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_8<TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_8<TestCallable>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<TestCallable, int&(const int)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallable, int&(const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_9<TestCallable>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_9<TestCallable>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_test<TestCallableTemplate, void (int)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallableTemplate, void (int)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_10<TestCallableTemplate>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_10<TestCallableTemplate>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<TestCallableTemplate,
            const void *(const int*)>::value;
    funcPossibleCompile = IsMemberFunc_test<TestCallableTemplate,
            const void *(const int*)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_11<TestCallableTemplate>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_11<TestCallableTemplate>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_test<Without, int (int)>::value;
    funcPossibleCompile = IsMemberFunc_test<Without, int (int)>::type::value;
    funcSigPossible = IsMemberSigFunc_test_12<Without>::value;
    funcSigPossibleCompile = IsMemberSigFunc_test_12<Without>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_PPTYPES_H_ */
