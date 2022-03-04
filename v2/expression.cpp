#include "expression.h"
#include "tokenizer.h"

const std::string Expression::Equivalence::SYMBOL {Tokenize::EQ_SYM};
const std::string Expression::Implication::SYMBOL  {Tokenize::IMP_SYM};
const std::string Expression::Disjunction::SYMBOL {Tokenize::DIS_SYM};
const std::string Expression::Conjunction::SYMBOL {Tokenize::CON_SYM};

/*
    BINARY OPERATORS
*/

Expression::Bin_Exp::Bin_Exp(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :left{std::move(l)}, right{std::move(r)} {}

Expression::Equivalence::Equivalence(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

Expression::Implication::Implication(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

Expression::Disjunction::Disjunction(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

Expression::Conjunction::Conjunction(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) :Bin_Exp{std::move(l), std::move(r)} {}

void Expression::Bin_Exp::print(std::ostream& os) const {
    os << '(';
    left->print(os);
    os << this->get_symbol();
    right->print(os);
    os << ')';
}

std::string Expression::Equivalence::get_symbol() const {
    return Equivalence::SYMBOL;
}

std::string Expression::Implication::get_symbol() const {
    return Implication::SYMBOL;
}

std::string Expression::Disjunction::get_symbol() const {
    return Disjunction::SYMBOL;
}

std::string Expression::Conjunction::get_symbol() const {
    return Conjunction::SYMBOL;
}

bool Expression::Equivalence::eval(const Env& env) const {
    const bool L = left->eval(env);
    const bool R = right->eval(env); //We need to compute both regardless, no short circuiting possible.
    return L && R || !L && !R;
}

bool Expression::Implication::eval(const Env& env) const {
    return !left->eval(env) || right->eval(env);
}

bool Expression::Disjunction::eval(const Env& env) const {
    return left->eval(env) || right->eval(env);
}

bool Expression::Conjunction::eval(const Env& env) const {
    return left->eval(env) && right->eval(env);
}

/*
    UNARY OPERATORS
*/

Expression::Un_Exp::Un_Exp(std::unique_ptr<Expression> e) :exp{std::move(e)} {}

Expression::Negation::Negation(std::unique_ptr<Expression> e) : Un_Exp{std::move(e)} {}

void Expression::Negation::print(std::ostream& os) const {
    os << Tokenize::NEG_SYM;
    exp->print(os);
}

bool Expression::Negation::eval(const Env& env) const {
    return !exp->eval(env);
}

/*
    CONSTANTS;
*/

void Expression::Variable::print(std::ostream& os) const {
    os << name;
}

bool Expression::Variable::eval(const Env& env) const {
    return env.at(name);
}

void Expression::Constant::print(std::ostream& os) const {
    os << (val ? Tokenize::TRUE_SYM : Tokenize::FALSE_SYM);
}

bool Expression::Constant::eval(const Env&) const {
    return val;
}

namespace Expression::Fabric{
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

namespace Expression::Test {
    /*
    Question: is it safe to "use namespace Fabric" here?
    Fear: no, because other user might include Expression::Test, and it will automatically include Fabric?
    Observation 1: functions from Expression::Fabric are not available in main.cpp after including Expression::Test.
    Observation 2: When the using declaration is placed inside the .h file, these functions are available.
    Conclusion: It is best to stay away from such subtle techniques, regardless of the answer.
                I will treat it as unsafe. 
    */ 
    bool test_all() {
        return test_eq() && test_imp() && test_dis() && test_con() && test_neg() && test_var() && test_con();
    }

    bool test_eq() {
        return Fabric::eq(Fabric::f(), Fabric::f())->eval({})   //0 0 1
            && !Fabric::eq(Fabric::f(), Fabric::t())->eval({})  //0 1 0
            && !Fabric::eq(Fabric::t(), Fabric::f())->eval({})  //1 0 0
            && Fabric::eq(Fabric::t(), Fabric::t())->eval({});  //1 1 1
    }

    bool test_imp(){
        return Fabric::imp(Fabric::f(), Fabric::f())->eval({})   //0 0 1
            && Fabric::imp(Fabric::f(), Fabric::t())->eval({})   //0 1 1
            && !Fabric::imp(Fabric::t(), Fabric::f())->eval({})  //1 0 0
            && Fabric::imp(Fabric::t(), Fabric::t())->eval({});  //1 1 1
    }

    bool test_dis(){
        return !Fabric::dis(Fabric::f(), Fabric::f())->eval({}) //0 0 0
            && Fabric::dis(Fabric::f(), Fabric::t())->eval({})  //0 1 1
            && Fabric::dis(Fabric::t(), Fabric::f())->eval({})  //1 0 1
            && Fabric::dis(Fabric::t(), Fabric::t())->eval({}); //1 1 1
    }

    bool test_con(){
        return !Fabric::con(Fabric::f(), Fabric::f())->eval({}) //0 0 0
            && !Fabric::con(Fabric::f(), Fabric::t())->eval({}) //0 1 0
            && !Fabric::con(Fabric::t(), Fabric::f())->eval({}) //1 0 0
            && Fabric::con(Fabric::t(), Fabric::t())->eval({}); //1 1 1
    }
    bool test_neg(){
        return Fabric::neg(Fabric::f())->eval({}) && !Fabric::neg(Fabric::t())->eval({});
    }
    
    bool test_var(){
        Env env {{"A", true}, {"B", false}};
        return Fabric::var("A")->eval(env) && !Fabric::var("B")->eval(env);
    }
    
    bool test_const(){
        return !Fabric::f()->eval({}) && Fabric::t()->eval({});
    }
}