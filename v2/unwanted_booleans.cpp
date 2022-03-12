/*
In this project there are trees.
We could abstract away and say every binary function is the same in essence and make a struct/class containing:

left expression
right expression
function pointer (or std::function<bool(bool, bool)>, or a template Op)
get_symbol() function (for printing [left symbol right])

However if we use function pointers then we will call these functions.
If for example I call conjunction as follows:
CON(false, [very big expression]), we are wasting resources as we will first evaluate the big expression, rather than short circuiting.
Therefore I prefer to give each its own struct in the following way:

https://en.cppreference.com/w/cpp/language/eval_order, point 3.

struct Disjunction {
    //Some functions
    bool eval (const Env& env) const override {
        return l->eval(env) || r->eval(env); //Does use short circuiting.
    }
    //Some data
};

*/

constexpr bool EQ(bool a, bool b) {
    return a && b || !a && !b;
}

constexpr bool IMP(bool a, bool b) {
    return !a || b;
}

constexpr bool CON(bool a, bool b) {
    return a && b;
}

constexpr bool DIS(bool a, bool b) {
    return a || b;
}

constexpr bool NEG(bool a) {
    return !a;
}       

namespace {
    template<int L, int R, int O, bool(*Fun)(bool, bool)>
    struct Test_Bin_Func {
        Test_Bin_Func(){
            static_assert(L == 0 || L == 1);
            static_assert(R == 0 || R == 1);
            static_assert(O == 0 || O == 1);
            static_assert(Fun(L, R) == O);
        }
    };

    Test_Bin_Func<1, 1, 1, EQ> t1;
    Test_Bin_Func<1, 0, 0, EQ> t2;
    Test_Bin_Func<0, 1, 0, EQ> t3;
    Test_Bin_Func<0, 0, 1, EQ> t4;

    Test_Bin_Func<1, 1, 1, IMP> t5;
    Test_Bin_Func<1, 0, 0, IMP> t6;
    Test_Bin_Func<0, 1, 1, IMP> t7;
    Test_Bin_Func<0, 0, 1, IMP> t8;
    
    Test_Bin_Func<1, 1, 1, CON> t9;
    Test_Bin_Func<1, 0, 0, CON> t10;
    Test_Bin_Func<0, 1, 0, CON> t11;
    Test_Bin_Func<0, 0, 0, CON> t12;
    
    Test_Bin_Func<1, 1, 1, DIS> t13;
    Test_Bin_Func<1, 0, 1, DIS> t14;
    Test_Bin_Func<0, 1, 1, DIS> t15;
    Test_Bin_Func<0, 0, 0, DIS> t16;
}