/*
 * testPPTypes.hpp
 *
 * TODO test volatile case
 */

#ifndef TEST_PPTYPES_H_
#define TEST_PPTYPES_H_

#include <boost/test/unit_test.hpp>
#include <vector>
#include "pptypes.hpp"

namespace core {
BOOST_AUTO_TEST_SUITE(PPTypesTests)

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

#define FUNC_MEMBER_PPTEST(testType)                            \
    fCheck = HasMember_test<testType>::value;                   \
    fCheckCompile = HasMember_test<testType>::type::value;      \
    barCheck = HasMember_bar<testType>::value;                  \
    barCheckCompile = HasMember_bar<testType>::type::value
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
    FUNC_MEMBER_PPTEST(int);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(float);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(char);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(char *);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    // Base types without required members
    FUNC_MEMBER_PPTEST(std::string);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(std::vector<short>);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    // Test classes designed to trigger appropriately
    FUNC_MEMBER_PPTEST(With);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithPrivate);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithPrivateInherit);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithInt);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithHidden);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithInherit);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithTemplate);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithTemplateInherit);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithTemplateInheritPrivate);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(Without);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithoutInherit);
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithFromWithout);
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    FUNC_MEMBER_PPTEST(WithMultiple);
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

CREATE_MEMBER_FUNC_CHECK(test);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (int), primitive);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (const int), 1);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (int*), 2);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (long*), 3);
CREATE_MEMBER_FUNC_SIG_CHECK(test, long (long*), 4);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (long*) const, 5);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (const int*), 6);
CREATE_MEMBER_FUNC_SIG_CHECK(test, Intable (const int), 7);
CREATE_MEMBER_FUNC_SIG_CHECK(test, Intable (const int) const, 8);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (Intable), 9);
CREATE_MEMBER_FUNC_SIG_CHECK(test, int& (const int), 10);
CREATE_MEMBER_FUNC_SIG_CHECK(test, void (int), 11);
CREATE_MEMBER_FUNC_SIG_CHECK(test, const void *(const int*), 12);
CREATE_MEMBER_FUNC_SIG_CHECK(test, int (int), 13);

#define FUNC_ASSIGN_PPTEST(testName, funcName, testType, testSignature)             \
    testPossible = HasMemberFunc_##funcName                                         \
                <testType, testSignature>::value;                                   \
    testPossibleCompile = HasMemberFunc_##funcName                                  \
                <testType, testSignature>::type::value;                             \
    testSigPossible = BOOST_PP_CAT(HasMemberSigFunc_##funcName, _##testName)        \
                <testType>::value;                                                  \
    testSigPossibleCompile = BOOST_PP_CAT(HasMemberSigFunc_##funcName, _##testName) \
                <testType>::type::value


BOOST_AUTO_TEST_CASE(hasMemberFunc) {
    bool testPossible;
    // For type that is defined as true_type for compile time checks
    bool testPossibleCompile;
    bool testSigPossible;
    // For type that is defined as true_type for compile time checks
    bool testSigPossibleCompile;

    // Primitive types
    FUNC_ASSIGN_PPTEST(primitive, test, int, void (int));
    BOOST_CHECK(!testPossible);
    BOOST_CHECK(!testPossibleCompile);
    BOOST_CHECK(!testSigPossible);
    BOOST_CHECK(!testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(primitive, test, std::string, void (void));
    BOOST_CHECK(!testPossible);
    BOOST_CHECK(!testPossibleCompile);
    BOOST_CHECK(!testSigPossible);
    BOOST_CHECK(!testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(primitive, test, double, void (void));
    BOOST_CHECK(!testPossible);
    BOOST_CHECK(!testPossibleCompile);
    BOOST_CHECK(!testSigPossible);
    BOOST_CHECK(!testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(primitive, test, char *, void (void));
    BOOST_CHECK(!testPossible);
    BOOST_CHECK(!testPossibleCompile);
    BOOST_CHECK(!testSigPossible);
    BOOST_CHECK(!testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(primitive, test, float, void (void));
    BOOST_CHECK(!testPossible);
    BOOST_CHECK(!testPossibleCompile);
    BOOST_CHECK(!testSigPossible);
    BOOST_CHECK(!testSigPossibleCompile);

    // Test classes designed to trigger appropriately
    FUNC_ASSIGN_PPTEST(1, test, TestCallable, void (const int));
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(2, test, TestCallable, void (int*));
    BOOST_CHECK(!testPossible); // Not
    BOOST_CHECK(!testPossibleCompile); // Not
    BOOST_CHECK(!testSigPossible); // Not
    BOOST_CHECK(!testSigPossibleCompile); // Not

    FUNC_ASSIGN_PPTEST(3, test, TestCallable, void (long*));
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(4, test, TestCallable, long (long*));
    BOOST_CHECK(!testPossible); // Not
    BOOST_CHECK(!testPossibleCompile); // Not
    BOOST_CHECK(!testSigPossible); // Not
    BOOST_CHECK(!testSigPossibleCompile); // Not

    FUNC_ASSIGN_PPTEST(5, test, TestCallable, void (long*) const);
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(6, test, TestCallable, void (const int*));
    BOOST_CHECK(!testPossible); // Not
    BOOST_CHECK(!testPossibleCompile); // Not
    BOOST_CHECK(!testSigPossible); // Not
    BOOST_CHECK(!testSigPossibleCompile); // Not

    FUNC_ASSIGN_PPTEST(7, test, TestCallable, Intable (const int));
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(8, test, TestCallable, Intable (const int) const);
    BOOST_CHECK(!testPossible); // Not
    BOOST_CHECK(!testPossibleCompile); // Not
    BOOST_CHECK(!testSigPossible); // Not
    BOOST_CHECK(!testSigPossibleCompile); // Not

    FUNC_ASSIGN_PPTEST(9, test, TestCallable, void (Intable));
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(10, test, TestCallable, int& (const int));
    BOOST_CHECK(!testPossible); // Not
    BOOST_CHECK(!testPossibleCompile); // Not
    BOOST_CHECK(!testSigPossible); // Not
    BOOST_CHECK(!testSigPossibleCompile); // Not

    FUNC_ASSIGN_PPTEST(11, test, TestCallable, void (int));
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(12, test, TestCallableTemplate, const void *(const int*));
    BOOST_CHECK(testPossible);
    BOOST_CHECK(testPossibleCompile);
    BOOST_CHECK(testSigPossible);
    BOOST_CHECK(testSigPossibleCompile);

    FUNC_ASSIGN_PPTEST(13, test, Without, int (int));
    BOOST_CHECK(!testPossible); // Not
    BOOST_CHECK(!testPossibleCompile); // Not
    BOOST_CHECK(!testSigPossible); // Not
    BOOST_CHECK(!testSigPossibleCompile); // Not
}

#define MANY_FUNC_CREATE_CALLS_PPTEST(args)                                 \
    void mcall(args) {}                                                     \
    int mcallret(args) { return 1; }                                        \
    int mcallret(args) const { return 1; }                                  \
    void mcallconst(args) const {}

#define MANY_FUNC_CREATE_PPTEST(args, testnum)                              \
    CREATE_MEMBER_FUNC_SIG_CHECK(mcall, void args, testnum);                \
    CREATE_MEMBER_FUNC_SIG_CHECK(mcall, void args const, const_##testnum);  \
    CREATE_MEMBER_FUNC_SIG_CHECK(mcallret, int args, testnum);              \
    CREATE_MEMBER_FUNC_SIG_CHECK(mcallret, int args const, const_##testnum);\
    CREATE_MEMBER_FUNC_SIG_CHECK(mcallconst, void args, testnum);           \
    CREATE_MEMBER_FUNC_SIG_CHECK(mcallconst, void args const, const_##testnum)

// 1 arg
#define MANY_FUNC_ARGS_PPTEST_1 int
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_1), 1);
MANY_FUNC_CREATE_PPTEST((Without), fail_1); // For failure test
// 2 args
#define MANY_FUNC_ARGS_PPTEST_2 int, long
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_2), 2);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_1, Without), fail_2);
// 3 args
#define MANY_FUNC_ARGS_PPTEST_3 int, long, char
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_3), 3);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_2, Without), fail_3);
// 4 args
#define MANY_FUNC_ARGS_PPTEST_4 int, long, char, bool
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_4), 4);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_3, Without), fail_4);
// 5 args
#define MANY_FUNC_ARGS_PPTEST_5 int, long, char, bool, float
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_5), 5);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_4, Without), fail_5);
// 6 args
#define MANY_FUNC_ARGS_PPTEST_6 int, long, char, bool, float,               \
    short
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_6), 6);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_5, Without), fail_6);
// 7 args
#define MANY_FUNC_ARGS_PPTEST_7 int, long, char, bool, float,               \
    short, const With&
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_7), 7);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_6, Without), fail_7);
// 8 args
#define MANY_FUNC_ARGS_PPTEST_8 int, long, char, bool, float,               \
    short, const With&, double
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_8), 8);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_7, Without), fail_8);
// 9 args
#define MANY_FUNC_ARGS_PPTEST_9 int, long, char, bool, float,               \
    short, const With&, double, std::string
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_9), 9);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_8, Without), fail_9);
// 10 args
#define MANY_FUNC_ARGS_PPTEST_10 int, long, char, bool, float,              \
    short, const With&, double, std::string, int*
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_10), 10);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_9, Without), fail_10);
// 11 args
#define MANY_FUNC_ARGS_PPTEST_11 int, long, char, bool, float,              \
    short, const With&, double, std::string, int*, const char * const
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_11), 11);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_10, Without), fail_11);
// 12 args
#define MANY_FUNC_ARGS_PPTEST_12 int, long, char, bool, float,              \
    short, const With&, double, std::string, int*, const char * const,      \
    int&
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_12), 12);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_11, Without), fail_12);
// 13 args
#define MANY_FUNC_ARGS_PPTEST_13 int, long, char, bool, float,              \
    short, const With&, double, std::string, int*, const char * const,      \
    int&, bool**
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_13), 13);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_12, Without), fail_13);
// 14 args
#define MANY_FUNC_ARGS_PPTEST_14 int, long, char, bool, float,              \
    short, const With&, double, std::string, int*, const char * const,      \
    int&, bool**, std::vector<std::vector<int> >
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_14), 14);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_13, Without), fail_14);
// 15 args
#define MANY_FUNC_ARGS_PPTEST_15 int, long, char, bool, float,              \
    short, const With&, double, std::string, int*, const char * const,      \
    int&, bool**, std::vector<std::vector<int> >, float&
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_15), 15);
MANY_FUNC_CREATE_PPTEST((MANY_FUNC_ARGS_PPTEST_14, Without), fail_15);

/*
 * Tests cases many arguments from 1 to 15 parameters.
 * TODO check volatile cases.
 */
class ManyTestCallable {
public:
    // 1 arg
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_1)
    // 2 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_2)
    // 3 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_3)
    // 4 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_4)
    // 5 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_5)
    // 6 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_6)
    // 7 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_7)
    // 8 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_8)
    // 9 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_9)
    // 10 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_10)
    // 11 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_11)
    // 12 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_12)
    // 13 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_13)
    // 14 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_14)
    // 15 args
    MANY_FUNC_CREATE_CALLS_PPTEST(MANY_FUNC_ARGS_PPTEST_15)
};

#define MANY_FUNC_ASSIGN_PPTEST(testnum)                                    \
    mcallPossible = HasMemberSigFunc_mcall_##testnum                        \
            <ManyTestCallable>::value;                                      \
    mcallPossibleAsConst =                                                  \
        BOOST_PP_CAT(HasMemberSigFunc_mcall_const, _##testnum)              \
            <ManyTestCallable>::value;                                      \
    mcallRetPossible = HasMemberSigFunc_mcallret_##testnum                  \
            <ManyTestCallable>::value;                                      \
    mcallRetPossibleAsConst =                                               \
        BOOST_PP_CAT(HasMemberSigFunc_mcallret_const, _##testnum)           \
            <ManyTestCallable>::value;                                      \
    mcallConstPossible = HasMemberSigFunc_mcallconst_##testnum              \
            <ManyTestCallable>::value;                                      \
    mcallConstPossibleAsConst =                                             \
        BOOST_PP_CAT(HasMemberSigFunc_mcallconst_const, _##testnum)         \
            <ManyTestCallable>::value

#define MANY_FUNC_CHECK_PPTEST()                                            \
    BOOST_CHECK(mcallPossible);                                             \
    BOOST_CHECK(!mcallPossibleAsConst); /* This should fail */              \
    BOOST_CHECK(mcallRetPossible);                                          \
    BOOST_CHECK(mcallRetPossibleAsConst);                                   \
    BOOST_CHECK(mcallConstPossible);                                        \
    BOOST_CHECK(mcallConstPossibleAsConst)

#define MANY_FUNC_FAIL_CHECK_PPTEST()                                       \
    /* All of these should fail */                                          \
    BOOST_CHECK(!mcallPossible);                                            \
    BOOST_CHECK(!mcallPossibleAsConst);                                     \
    BOOST_CHECK(!mcallRetPossible);                                         \
    BOOST_CHECK(!mcallRetPossibleAsConst);                                  \
    BOOST_CHECK(!mcallConstPossible);                                       \
    BOOST_CHECK(!mcallConstPossibleAsConst)

BOOST_AUTO_TEST_CASE(hasMemberFuncManyArgs) {
    // Use these to check each test result
    bool mcallPossible;
    bool mcallPossibleAsConst;
    bool mcallRetPossible;
    bool mcallRetPossibleAsConst;
    bool mcallConstPossible;
    bool mcallConstPossibleAsConst;

    // 1 arg
    MANY_FUNC_ASSIGN_PPTEST(1);
    MANY_FUNC_CHECK_PPTEST();
    // 1 arg mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_1);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 2 args
    MANY_FUNC_ASSIGN_PPTEST(2);
    MANY_FUNC_CHECK_PPTEST();
    // 2 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_2);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 3 args
    MANY_FUNC_ASSIGN_PPTEST(3);
    MANY_FUNC_CHECK_PPTEST();
    // 3 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_3);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 4 args
    MANY_FUNC_ASSIGN_PPTEST(4);
    MANY_FUNC_CHECK_PPTEST();
    // 4 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_4);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 5 args
    MANY_FUNC_ASSIGN_PPTEST(5);
    MANY_FUNC_CHECK_PPTEST();
    // 5 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_5);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 6 args
    MANY_FUNC_ASSIGN_PPTEST(6);
    MANY_FUNC_CHECK_PPTEST();
    // 6 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_6);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 7 args
    MANY_FUNC_ASSIGN_PPTEST(7);
    MANY_FUNC_CHECK_PPTEST();
    // 7 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_7);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 8 args
    MANY_FUNC_ASSIGN_PPTEST(8);
    MANY_FUNC_CHECK_PPTEST();
    // 8 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_8);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 9 args
    MANY_FUNC_ASSIGN_PPTEST(9);
    MANY_FUNC_CHECK_PPTEST();
    // 9 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_9);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 10 args
    MANY_FUNC_ASSIGN_PPTEST(10);
    MANY_FUNC_CHECK_PPTEST();
    // 10 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_10);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 11 args
    MANY_FUNC_ASSIGN_PPTEST(11);
    MANY_FUNC_CHECK_PPTEST();
    // 11 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_11);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 12 args
    MANY_FUNC_ASSIGN_PPTEST(12);
    MANY_FUNC_CHECK_PPTEST();
    // 12 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_12);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 13 args
    MANY_FUNC_ASSIGN_PPTEST(13);
    MANY_FUNC_CHECK_PPTEST();
    // 13 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_13);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 14 args
    MANY_FUNC_ASSIGN_PPTEST(14);
    MANY_FUNC_CHECK_PPTEST();
    // 14 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_14);
    MANY_FUNC_FAIL_CHECK_PPTEST();

    // 15 args
    MANY_FUNC_ASSIGN_PPTEST(15);
    MANY_FUNC_CHECK_PPTEST();
    // 15 args mismatching args
    MANY_FUNC_ASSIGN_PPTEST(fail_15);
    MANY_FUNC_FAIL_CHECK_PPTEST();
}

class NoDefaultConst {
public:
    NoDefaultConst(int) {}
};

class VoidClass {
public:
    int voidArg() { return 1; }
};

class VoidClassConst {
public:
    int voidArg() const { return 1; }
};

class VoidInherit : public VoidClass {};

class BadVoidClass {
public:
    NoDefaultConst voidArg;
};

class NonMethodVoidClass {
public:
    int voidArg;
};

class VoidRetClass {
public:
    void voidArg() {}
};

class VoidPrivateClass {
private:
    void voidArg() {}
};

class VoidArgsClass {
public:
    int voidArg(int) const { return 1; }
};

CREATE_MEMBER_FUNC_CHECK(voidArg);

/*
 * Tests all cases of no argument function checks.
 * TODO check volatile cases.
 */
BOOST_AUTO_TEST_CASE(voidArgTest) {
    bool intRetPossible;
    bool voidRetPossible;
    intRetPossible = HasMemberFunc_voidArg<VoidClass, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidClass, void (void)>::value;
    BOOST_CHECK(intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidClass, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidClass, void (void) const>::value;
    BOOST_CHECK(!intRetPossible);
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidClassConst, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidClassConst, void (void)>::value;
    BOOST_CHECK(intRetPossible);
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidClassConst, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidClassConst, void (void) const>::value;
    BOOST_CHECK(intRetPossible);
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidInherit, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidInherit, void (void)>::value;
    BOOST_CHECK(intRetPossible);
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidInherit, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidInherit, void (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidClass, std::string (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidClass, std::string (void)>::value;
    BOOST_CHECK(!intRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<BadVoidClass, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<BadVoidClass, void (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<BadVoidClass, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<BadVoidClass, void (void)>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<NonMethodVoidClass, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<NonMethodVoidClass, void (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<NonMethodVoidClass, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<NonMethodVoidClass, void (void)>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidRetClass, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidRetClass, void (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidRetClass, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidRetClass, void (void)>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(voidRetPossible);

    intRetPossible = HasMemberFunc_voidArg<VoidPrivateClass, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidPrivateClass, void (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidPrivateClass, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidPrivateClass, void (void)>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(voidRetPossible); // (Should be Not, but our checkers sees private types...)

    intRetPossible = HasMemberFunc_voidArg<VoidArgsClass, int (void) const>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidArgsClass, void (void) const>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not

    intRetPossible = HasMemberFunc_voidArg<VoidArgsClass, int (void)>::value;
    voidRetPossible = HasMemberFunc_voidArg<VoidArgsClass, void (void)>::value;
    BOOST_CHECK(!intRetPossible); // Not
    BOOST_CHECK(!voidRetPossible); // Not
}

// Test code for casting from private to public
/*
template<typename Tag>
struct result {
  // export it ...
  typedef typename Tag::type type;
  static type ptr;
};

template<typename Tag>
typename result<Tag>::type result<Tag>::ptr;

template<typename Tag, typename Tag::type p>
struct rob : result<Tag> {
  // fill it ...
  struct filler {
    filler() { result<Tag>::ptr = p; }
  };
  static filler filler_obj;
};

template<typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

class BarHider {
private:
    int bar;
};

class BarPublic {
public:
    int bar;
};

struct BarExposer { typedef int (BarHider::*type); };
class rob<BarExposer, &BarHider::bar>;

struct PubBarExposer { typedef int (BarPublic::*type); };
class rob<PubBarExposer, &BarPublic::bar>;

template<typename Cast, typename Exposer, typename Type>
static Cast public_cast(Type *t) {
    return &(t->*result<Exposer>::ptr);
}

template<typename Type, typename Exposer>
struct IsBarPrivateMember {
private:
    typedef char Private;
    typedef long Public;

    static Private publicfinder(...);

public:
    static const bool value = (sizeof(publicfinder((Type*)(NULL))) == sizeof(Public));
};

BOOST_AUTO_TEST_CASE(privateChecker) {

    bool isBarMember = IsBarMember<BarHider>::value;
    bool hasPrivateBar = IsBarPrivateMember<BarHider, BarExposer>::value;
    int *test = public_cast<int *, BarExposer>((BarHider*)NULL);
    BOOST_CHECK(isBarMember);
    BOOST_CHECK(hasPrivateBar);
    isBarMember = IsBarMember<BarPublic>::value;
    hasPrivateBar = IsBarPrivateMember<BarPublic, PubBarExposer>::value;
    BOOST_CHECK(isBarMember);
    BOOST_CHECK(!hasPrivateBar);

}
*/

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_PPTYPES_H_ */
