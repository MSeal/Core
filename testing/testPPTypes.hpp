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

// Various classes with or without 'func' member
class With {
public:
    void func(int, long) const {}
};

class WithInherit : public With {};

class WithPrivate {
private:
    void func(int, long) const {}
};

class WithPrivateInherit : private With {};

class WithInt {
    int func;
};

class WithHidden : public With {
public:
    WithHidden(int) : With() {}
};

class WithTemplate {
public:
    template <typename T, typename U>
    void func(T, U){}
};

class WithTemplateInherit : public WithTemplate {};

class WithTemplateInheritPrivate : private WithTemplate {};

class Without {};

struct WithoutInherit : Without {};

class WithFromWithout : Without {
    template <typename T, typename U>
    void func(T, U){}
};

class WithMultiple {
    int func;
    bool bar;
};

// The magic template function generator
CREATE_MEMBER_CHECK(func);
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
    fCheck = HasMember_func<int>::value;
    fCheckCompile = HasMember_func<int>::type::value;
    barCheck = HasMember_bar<int>::value;
    barCheckCompile = HasMember_bar<int>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<float>::value;
    fCheckCompile = HasMember_func<float>::type::value;
    barCheck = HasMember_bar<float>::value;
    barCheckCompile = HasMember_bar<float>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<char>::value;
    fCheckCompile = HasMember_func<char>::type::value;
    barCheck = HasMember_bar<char>::value;
    barCheckCompile = HasMember_bar<char>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    // Base types without required members
    fCheck = HasMember_func<std::string>::value;
    fCheckCompile = HasMember_func<std::string>::type::value;
    barCheck = HasMember_bar<std::string>::value;
    barCheckCompile = HasMember_bar<std::string>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<char *>::value;
    fCheckCompile = HasMember_func<char *>::type::value;
    barCheck = HasMember_bar<char *>::value;
    barCheckCompile = HasMember_bar<char *>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<std::vector<short> >::value;
    fCheckCompile = HasMember_func<std::vector<short> >::type::value;
    barCheck = HasMember_bar<std::vector<short> >::value;
    barCheckCompile = HasMember_bar<std::vector<short> >::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    // Test classes designed to trigger appropriately
    fCheck = HasMember_func<With>::value;
    fCheckCompile = HasMember_func<With>::type::value;
    barCheck = HasMember_bar<With>::value;
    barCheckCompile = HasMember_bar<With>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithPrivate>::value;
    fCheckCompile = HasMember_func<WithPrivate>::type::value;
    barCheck = HasMember_bar<WithPrivate>::value;
    barCheckCompile = HasMember_bar<WithPrivate>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithPrivateInherit>::value;
    fCheckCompile = HasMember_func<WithPrivateInherit>::type::value;
    barCheck = HasMember_bar<WithPrivateInherit>::value;
    barCheckCompile = HasMember_bar<WithPrivateInherit>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithInt>::value;
    fCheckCompile = HasMember_func<WithInt>::type::value;
    barCheck = HasMember_bar<WithInt>::value;
    barCheckCompile = HasMember_bar<WithInt>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithHidden>::value;
    fCheckCompile = HasMember_func<WithHidden>::type::value;
    barCheck = HasMember_bar<WithHidden>::value;
    barCheckCompile = HasMember_bar<WithHidden>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithInherit>::value;
    fCheckCompile = HasMember_func<WithInherit>::type::value;
    barCheck = HasMember_bar<WithInherit>::value;
    barCheckCompile = HasMember_bar<WithInherit>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithTemplate>::value;
    fCheckCompile = HasMember_func<WithTemplate>::type::value;
    barCheck = HasMember_bar<WithTemplate>::value;
    barCheckCompile = HasMember_bar<WithTemplate>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithTemplateInherit>::value;
    fCheckCompile = HasMember_func<WithTemplateInherit>::type::value;
    barCheck = HasMember_bar<WithTemplateInherit>::value;
    barCheckCompile = HasMember_bar<WithTemplateInherit>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithTemplateInheritPrivate>::value;
    fCheckCompile = HasMember_func<WithTemplateInheritPrivate>::type::value;
    barCheck = HasMember_bar<WithTemplateInheritPrivate>::value;
    barCheckCompile = HasMember_bar<WithTemplateInheritPrivate>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<Without>::value;
    fCheckCompile = HasMember_func<Without>::type::value;
    barCheck = HasMember_bar<Without>::value;
    barCheckCompile = HasMember_bar<Without>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithoutInherit>::value;
    fCheckCompile = HasMember_func<WithoutInherit>::type::value;
    barCheck = HasMember_bar<WithoutInherit>::value;
    barCheckCompile = HasMember_bar<WithoutInherit>::type::value;
    BOOST_CHECK(!fCheck); // Not
    BOOST_CHECK(!fCheckCompile); // Not
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithFromWithout>::value;
    fCheckCompile = HasMember_func<WithFromWithout>::type::value;
    barCheck = HasMember_bar<WithFromWithout>::value;
    barCheckCompile = HasMember_bar<WithFromWithout>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(!barCheck); // Not
    BOOST_CHECK(!barCheckCompile); // Not

    fCheck = HasMember_func<WithMultiple>::value;
    fCheckCompile = HasMember_func<WithMultiple>::type::value;
    barCheck = HasMember_bar<WithMultiple>::value;
    barCheckCompile = HasMember_bar<WithMultiple>::type::value;
    BOOST_CHECK(fCheck);
    BOOST_CHECK(fCheckCompile);
    BOOST_CHECK(barCheck);
    BOOST_CHECK(barCheckCompile);
}

struct A
{
    A(int){}; //not explicit
};

struct Possible
{
    int func(A, int* = 0){return int();}
    void func(long*) const{}
};

struct PossibleTemplate
{
    template <typename T> T* func(T t)const {return &t;}
    template <typename T> T* func(T* t){return t;}
};

CREATE_MEMBER_FUNC_CHECK(func);
CREATE_MEMBER_FUNC_SIG_CHECK(func, void (void), primitive);
CREATE_MEMBER_FUNC_SIG_CHECK(func, void (const int), test1);
CREATE_MEMBER_FUNC_SIG_CHECK(func, void (int*), test2);
CREATE_MEMBER_FUNC_SIG_CHECK(func, void (long*), test3);
CREATE_MEMBER_FUNC_SIG_CHECK(func, void (const int), test4);
CREATE_MEMBER_FUNC_SIG_CHECK(func, A(const int), test5);
CREATE_MEMBER_FUNC_SIG_CHECK(func, int*(const int), test6);
CREATE_MEMBER_FUNC_SIG_CHECK(func, void (int), test7);
CREATE_MEMBER_FUNC_SIG_CHECK(func, const void* (const int*), test8);
CREATE_MEMBER_FUNC_SIG_CHECK(func, int (int), test9);

BOOST_AUTO_TEST_CASE(hasMemberFunc) {
    bool funcPossible;
    // For type that is defined as true_type for compile time checks
    bool funcPossibleCompile;
    bool funcSigPossible;
    // For type that is defined as true_type for compile time checks
    bool funcSigPossibleCompile;

    // Primitive types
    funcPossible = IsMemberFunc_func<int, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_func<int, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_primitive<int>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_primitive<int>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<std::string, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_func<std::string, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_primitive<std::string>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_primitive<std::string>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<double, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_func<double, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_primitive<double>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_primitive<double>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<char *, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_func<char *, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_primitive<char *>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_primitive<char *>::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<std::vector<float>, void (void)>::value;
    funcPossibleCompile = IsMemberFunc_func<std::vector<float>, void (void)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_primitive<std::vector<float> >::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_primitive<std::vector<float> >::type::value;
    BOOST_CHECK(!funcPossible);
    BOOST_CHECK(!funcPossibleCompile);
    BOOST_CHECK(!funcSigPossible);
    BOOST_CHECK(!funcSigPossibleCompile);

    // Test classes designed to trigger appropriately
    funcPossible = IsMemberFunc_func<Possible, void (const int)>::value;
    funcPossibleCompile = IsMemberFunc_func<Possible, void (const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test1<Possible>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test1<Possible>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<Possible, void (int*)>::value;
    funcPossibleCompile = IsMemberFunc_func<Possible, void (int*)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test2<Possible>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test2<Possible>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_func<const Possible, void (long*)>::value;
    funcPossibleCompile = IsMemberFunc_func<const Possible, void (long*)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test3<const Possible>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test3<const Possible>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<const Possible, void (const int)>::value;
    funcPossibleCompile = IsMemberFunc_func<const Possible, void (const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test4<const Possible>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test4<const Possible>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_func<Possible, A(const int)>::value;
    funcPossibleCompile = IsMemberFunc_func<Possible, A(const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test5<Possible>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test5<Possible>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<Possible, int*(const int)>::value;
    funcPossibleCompile = IsMemberFunc_func<Possible, int*(const int)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test6<Possible>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test6<Possible>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not

    funcPossible = IsMemberFunc_func<PossibleTemplate, void (int)>::value;
    funcPossibleCompile = IsMemberFunc_func<PossibleTemplate, void (int)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test7<PossibleTemplate>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test7<PossibleTemplate>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<PossibleTemplate, const void* (const int*)>::value;
    funcPossibleCompile = IsMemberFunc_func<PossibleTemplate, const void* (const int*)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test8<PossibleTemplate>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test8<PossibleTemplate>::type::value;
    BOOST_CHECK(funcPossible);
    BOOST_CHECK(funcPossibleCompile);
    BOOST_CHECK(funcSigPossible);
    BOOST_CHECK(funcSigPossibleCompile);

    funcPossible = IsMemberFunc_func<Without, int (int)>::value;
    funcPossibleCompile = IsMemberFunc_func<Without, int (int)>::type::value;
    funcSigPossible = IsMemberSigFunc_func_test9<Without>::value;
    funcSigPossibleCompile = IsMemberSigFunc_func_test9<Without>::type::value;
    BOOST_CHECK(!funcPossible); // Not
    BOOST_CHECK(!funcPossibleCompile); // Not
    BOOST_CHECK(!funcSigPossible); // Not
    BOOST_CHECK(!funcSigPossibleCompile); // Not
}

BOOST_AUTO_TEST_SUITE_END()
}

#endif /* TEST_PPTYPES_H_ */
