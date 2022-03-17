#include "expression.h"
#include "tokenizer.h"

const std::string expression::Equivalence::SYMBOL {tokenize::EQ_SYM};
const std::string expression::Implication::SYMBOL {tokenize::IMP_SYM};
const std::string expression::Disjunction::SYMBOL {tokenize::DIS_SYM};
const std::string expression::Conjunction::SYMBOL {tokenize::CON_SYM};

/*
    BINARY OPERATORS
*/

expression::Bin_Exp::Bin_Exp(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :left{std::move(l)}, right{std::move(r)} {}

expression::Equivalence::Equivalence(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

expression::Implication::Implication(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

expression::Disjunction::Disjunction(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

expression::Conjunction::Conjunction(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

void expression::Bin_Exp::print(bool brackets, std::ostream& os) const {
    if (brackets)
        os << '(';
    left->print(true, os);
    os << this->get_symbol();
    right->print(true, os);
    if (brackets)
        os << ')';
}

std::string expression::Equivalence::get_symbol() const {
    return Equivalence::SYMBOL;
}

std::string expression::Implication::get_symbol() const {
    return Implication::SYMBOL;
}

std::string expression::Disjunction::get_symbol() const {
    return Disjunction::SYMBOL;
}

std::string expression::Conjunction::get_symbol() const {
    return Conjunction::SYMBOL;
}

bool expression::Equivalence::eval(const Env& env) const {
    const bool L = left->eval(env);
    const bool R = right->eval(env); //We need to compute both regardless, no short circuiting possible.
    return L && R || !L && !R;
}

bool expression::Implication::eval(const Env& env) const {
    return !left->eval(env) || right->eval(env);
}

bool expression::Disjunction::eval(const Env& env) const {
    return left->eval(env) || right->eval(env);
}

bool expression::Conjunction::eval(const Env& env) const {
    return left->eval(env) && right->eval(env);
}

/*
    UNARY OPERATORS
*/

expression::Un_Exp::Un_Exp(std::unique_ptr<Expression> e) :exp{std::move(e)} {}

expression::Negation::Negation(std::unique_ptr<Expression> e) : Un_Exp{std::move(e)} {}

void expression::Negation::print(bool, std::ostream& os) const {
    os << tokenize::NEG_SYM;
    exp->print(true, os);
}

bool expression::Negation::eval(const Env& env) const {
    return !exp->eval(env);
}

/*
    CONSTANTS;
*/

void expression::Variable::print(bool, std::ostream& os) const {
    os << name;
}

bool expression::Variable::eval(const Env& env) const {
    return env.at(name);
}

void expression::Constant::print(bool, std::ostream& os) const {
    os << (val ? tokenize::TRUE_SYM : tokenize::FALSE_SYM);
}

bool expression::Constant::eval(const Env&) const {
    return val;
}

/*
    FABRIC FUNCTIONS.
*/

namespace expression::fabric{
    std::unique_ptr<Equivalence> eq(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) { 
        return std::make_unique<Equivalence>(std::move(l), std::move(r));
    }

    std::unique_ptr<Implication>  imp(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) { 
        return std::make_unique<Implication>(std::move(l), std::move(r));
    }

    std::unique_ptr<Disjunction> dis(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) { 
        return std::make_unique<Disjunction>(std::move(l), std::move(r));
    }
    
    std::unique_ptr<Conjunction> con(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) { 
        return std::make_unique<Conjunction>(std::move(l), std::move(r));
    }

    std::unique_ptr<Negation> neg(std::unique_ptr<Expression> e) {
        return  std::make_unique<Negation>(std::move(e));
    }

    std::unique_ptr<Variable> var(const Var_type& name) {
        return std::make_unique<Variable>(name);
    }

    std::unique_ptr<Constant> cons(bool b) {
        return std::make_unique<Constant>(b);
    }

    std::unique_ptr<Constant> t() {
        return cons(true);
    }
    
    std::unique_ptr<Constant> f() {
        return cons(false);
    }
}

namespace expression::test {
    /*
    Question: is it safe to "use namespace Fabric" here?
    Fear: no, because other user might include expression::Test, and it will automatically include Fabric?
    Observation 1: functions from expression::Fabric are not available in main.cpp after including expression::Test.
    Observation 2: When the using declaration is placed inside the .h file, these functions are available.
    Conclusion: It is best to stay away from such subtle techniques, regardless of the answer.
                I will treat it as unsafe. 
    */ 
    bool test_all() {
        return test_eq() && test_imp() && test_dis() && test_con() && test_neg() && test_var() && test_con();
    }

    bool test_eq() {
        return fabric::eq(fabric::f(), fabric::f())->eval()   //0 0 1
            && !fabric::eq(fabric::f(), fabric::t())->eval()  //0 1 0
            && !fabric::eq(fabric::t(), fabric::f())->eval()  //1 0 0
            && fabric::eq(fabric::t(), fabric::t())->eval();  //1 1 1
    }

    bool test_imp(){
        return fabric::imp(fabric::f(), fabric::f())->eval()   //0 0 1
            && fabric::imp(fabric::f(), fabric::t())->eval()   //0 1 1
            && !fabric::imp(fabric::t(), fabric::f())->eval()  //1 0 0
            && fabric::imp(fabric::t(), fabric::t())->eval();  //1 1 1
    }

    bool test_dis(){
        return !fabric::dis(fabric::f(), fabric::f())->eval() //0 0 0
            && fabric::dis(fabric::f(), fabric::t())->eval()  //0 1 1
            && fabric::dis(fabric::t(), fabric::f())->eval()  //1 0 1
            && fabric::dis(fabric::t(), fabric::t())->eval(); //1 1 1
    }

    bool test_con(){
        return !fabric::con(fabric::f(), fabric::f())->eval() //0 0 0
            && !fabric::con(fabric::f(), fabric::t())->eval() //0 1 0
            && !fabric::con(fabric::t(), fabric::f())->eval() //1 0 0
            && fabric::con(fabric::t(), fabric::t())->eval(); //1 1 1
    }
    bool test_neg(){
        return fabric::neg(fabric::f())->eval() && !fabric::neg(fabric::t())->eval();
    }
    
    bool test_var(){
        Env env {{"A", true}, {"B", false}};
        return fabric::var("A")->eval(env) && !fabric::var("B")->eval(env);
    }
    
    bool test_const(){
        return !fabric::f()->eval() && fabric::t()->eval();
    }
}