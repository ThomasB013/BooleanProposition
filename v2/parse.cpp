#include "parse.h"
#include "tokenizer.h"
#include <sstream>

namespace {  
    //See grammar.md:
    //Functions names are p for parse followed by a short notation of the level they are in (e.g. eq for equivalence)
    std::unique_ptr<expression::Expression> p_eq(tokenize::Token_Stream& ts, bool consume);

    //<value>             ::= <ident>         | ( <formula> )
    std::unique_ptr<expression::Expression> p_val(tokenize::Token_Stream& ts, bool consume) {
        if (consume)
            ts.consume();
        
        switch(ts.get().kind) {
        case tokenize::Kind::BR_OPEN: {
            std::unique_ptr<expression::Expression> exp = p_eq(ts, true);
            if (ts.consume().kind != tokenize::Kind::BR_CLOSE) //Consume and check closing bracket.
                throw std::runtime_error {"Expected closing bracket after opening bracket."};
            return exp;
        }
        case tokenize::Kind::VAL:
            return expression::fabric::cons(ts.consume().value);
        case tokenize::Kind::VAR: 
            return expression::fabric::var(ts.consume().name);
        }
        throw std::runtime_error {"Expected a value."};
    }

    //<negation>          ::= <value>    | ~ <negation>
    std::unique_ptr<expression::Expression> p_neg(tokenize::Token_Stream& ts, bool consume) {
        if (consume)
            ts.consume();
        if (ts.get().kind == tokenize::Kind::NEG)
            return expression::fabric::neg(p_neg(ts, true)); //Consume ~ token.
        return p_val(ts, false);
    }

    //<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
    std::unique_ptr<expression::Expression> p_con(tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<expression::Expression> l = p_neg(ts, consume);
        if (ts.get().kind == tokenize::Kind::CON)
            l = expression::fabric::con(std::move(l), p_con(ts, true)); //Right recursion, consume /\ token.
        return l;
    }

    //<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
    std::unique_ptr<expression::Expression> p_dis(tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<expression::Expression> l = p_con(ts, consume);
        if (ts.get().kind == tokenize::Kind::DIS)
            l = expression::fabric::dis(std::move(l), p_dis(ts, true)); //Right recursion, consume \/ token.
        return l;
    }

    //<implication>       ::= <disjunction>   | <disjunction> -> <implication>
    std::unique_ptr<expression::Expression> p_imp(tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<expression::Expression> l = p_dis(ts, consume);
        if (ts.get().kind == tokenize::Kind::IMP)
            l = expression::fabric::imp(std::move(l), p_imp(ts, true)); //Right recursion, consume -> token. A->B->C: A->(B->C)
        return l;
    }

    //<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
    std::unique_ptr<expression::Expression> p_eq(tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<expression::Expression> l = p_imp(ts, consume);
        while (ts.get().kind == tokenize::Kind::EQ) //Left recursion: read implication and apply immediately. A<->B<->C: (A<->B)<->C
            l = expression::fabric::eq(std::move(l), p_imp(ts, true));
        return l;
    }

    //Function to go from a string to an expression.
    std::unique_ptr<expression::Expression> parse_from_string(const std::string& input) {
        tokenize::Token_Stream ts {input};
        return p_eq(ts, false);
    }

    //Test helper functions:
    bool test_parser(const std::string& input, const std::string& expect) {
        std::stringstream ss;
        parse::Proposition p {input};
        ss << p;
        if(ss.str() != expect) {
            std::cout << "Input: " << input 
                << "\nExpected: " << expect 
                << "\nActual: " << ss.str() << '\n';
            return false;
        }
        return true;
    }

     //Places OP between the truth table and checks against expected booleans
    bool test_parse_operator(const std::string OP, bool EXP_FF, bool EXP_FT, bool EXP_TF, bool EXP_TT) {
        return parse::Proposition(tokenize::FALSE_SYM + OP + tokenize::FALSE_SYM).eval() == EXP_FF
            && parse::Proposition(tokenize::FALSE_SYM + OP + tokenize::TRUE_SYM).eval() == EXP_FT
            && parse::Proposition(tokenize::TRUE_SYM + OP + tokenize::FALSE_SYM).eval() == EXP_TF
            && parse::Proposition(tokenize::TRUE_SYM + OP + tokenize::TRUE_SYM).eval() == EXP_TT;
    }

    bool test_parse_eq() {
        bool passed = test_parse_operator(tokenize::EQ_SYM, true, false, false, true);
        if (!passed)
            std::cout << "test_parse_eq failed\n";
        return passed;
    }

    bool test_parse_imp(){
        bool passed = test_parse_operator(tokenize::IMP_SYM, true, true, false, true);
        if (!passed)
            std::cout << "test_parse_imp failed\n";
        return passed;
    }

    bool test_parse_dis(){
        bool passed = test_parse_operator(tokenize::DIS_SYM, false, true, true, true);
        if (!passed)
            std::cout << "test_parse_dis failed\n";
        return passed;
    }

    bool test_parse_con(){
        bool passed = test_parse_operator(tokenize::CON_SYM, false, false, false, true);
        if (!passed)
            std::cout << "test_parse_con failed\n";
        return passed;
    
    }

    bool test_parse_neg(){
        bool passed = parse::Proposition(tokenize::NEG_SYM + tokenize::FALSE_SYM).eval() 
            && !parse::Proposition(tokenize::NEG_SYM + tokenize::TRUE_SYM).eval();
        if (!passed)
            std::cout << "test_parse_neg failed\n";
        return passed;
    }
    
    bool test_parse_var(){
        const expression::Env env {{"A", true}, {"B", false}};
        bool passed = parse::Proposition("A").eval(env) && !parse::Proposition("B").eval(env);
        if (!passed)
            std::cout << "test_parse_var failed\n";
        return passed;
    }
    
    bool test_parse_const(){
        bool passed = !parse::Proposition("F").eval() && parse::Proposition("T").eval();
        if (!passed)
            std::cout << "test_parse_const failed\n";
        return passed;
    }

}

parse::Proposition::Proposition(bool b):exp{expression::fabric::cons(b)} {}

parse::Proposition::Proposition(const std::string& input) :exp{parse_from_string(input)} {}

parse::Proposition::Proposition(const char* input) :Proposition{std::string{input}} {}

bool parse::Proposition::eval(const expression::Env& env) const {
    return exp->eval(env);
}

namespace parse {
    std::ostream& operator<<(std::ostream& os, const Proposition& p) {
        p.exp->print(false, os);
        return os;
    }
}

bool parse::test::test_parser() {
    return ::test_parser("   A<->B   <->C", "(A<->B)<->C")    //<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
        && ::test_parser("A->B  -> C", "A->(B->C)")           //<implication>       ::= <disjunction>   | <disjunction> -> <implication>
        && ::test_parser("A\\/B  \\/C", "A\\/(B\\/C)")        //<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
        && ::test_parser("   A/\\B    /\\C", "A/\\(B/\\C)")   //<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
        && ::test_parser("~~A    ", "~~A")                    //<negation>          ::= <value>         | ~ <negation>
        && ::test_parser("(  (  (  A ))   )", "A")            //<value>             ::= <ident>         | ( <formula> )
        && ::test_parser("_abcdefghijklmnopqrstuvwxyz'ABCDEFGHIJKLMNOPQRSTUVWXYZ_012345679         ", "_abcdefghijklmnopqrstuvwxyz'ABCDEFGHIJKLMNOPQRSTUVWXYZ_012345679")
        && ::test_parser("A->B<->C->D", "(A->B)<->(C->D)")
        && ::test_parser("A\\/B->C\\/D", "(A\\/B)->(C\\/D)")
        && ::test_parser("A/\\B\\/C/\\D", "(A/\\B)\\/(C/\\D)")
        && ::test_parser("~A/\\~B", "~A/\\~B")
        && ::test_parser("((~(((A))/\\((B)))))", "~(A/\\B)")
        && ::test_parser("A->B\\/C", "A->(B\\/C)")
        && ::test_parser("(A->B)\\/C", "(A->B)\\/C")
        && ::test_parser("A/\\B/\\C\\/D", "(A/\\(B/\\C))\\/D")
        && ::test_parser("A->A<->B->C->D", "(A->A)<->(B->(C->D))");
}

bool parse::test::test_parser_eval(){
    return ::test_parse_eq() 
        && ::test_parse_imp() 
        && ::test_parse_dis() 
        && ::test_parse_con() 
        && ::test_parse_neg() 
        && ::test_parse_var() 
        && ::test_parse_con();
}