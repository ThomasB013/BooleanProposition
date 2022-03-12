#include "parse.h"
#include "tokenizer.h"
#include <sstream>

namespace {  
    //See grammar.md:
    //Functions names are p for parse followed by a short notation of the level they are in (e.g. eq for equivalence)
    std::unique_ptr<Expression::Expression> p_eq(Tokenize::Token_Stream& ts, bool consume);

    //<value>             ::= <ident>         | ( <formula> )
    std::unique_ptr<Expression::Expression> p_val(Tokenize::Token_Stream& ts, bool consume) {
        if (consume)
            ts.consume();
        
        switch(ts.get().kind) {
        case Tokenize::Kind::BR_OPEN: {
            std::unique_ptr<Expression::Expression> exp = p_eq(ts, true);
            if (ts.consume().kind != Tokenize::Kind::BR_CLOSE) //Consume and check closing bracket.
                throw std::runtime_error {"Expected closing bracket after opening bracket."};
            return exp;
        }
        case Tokenize::Kind::VAL:
            return Expression::Fabric::cons(ts.consume().value);
        case Tokenize::Kind::VAR: 
            return Expression::Fabric::var(ts.consume().name);
        }
        throw std::runtime_error {"Expected a value."};
    }

    //<negation>          ::= <value>    | ~ <negation>
    std::unique_ptr<Expression::Expression> p_neg(Tokenize::Token_Stream& ts, bool consume) {
        if (consume)
            ts.consume();
        if (ts.get().kind == Tokenize::Kind::NEG)
            return Expression::Fabric::neg(p_neg(ts, true)); //Consume ~ token.
        return p_val(ts, false);
    }

    //<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
    std::unique_ptr<Expression::Expression> p_con(Tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<Expression::Expression> l = p_neg(ts, consume);
        if (ts.get().kind == Tokenize::Kind::CON)
            l = Expression::Fabric::con(std::move(l), p_con(ts, true)); //Right recursion, consume /\ token.
        return l;
    }

    //<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
    std::unique_ptr<Expression::Expression> p_dis(Tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<Expression::Expression> l = p_con(ts, consume);
        if (ts.get().kind == Tokenize::Kind::DIS)
            l = Expression::Fabric::dis(std::move(l), p_dis(ts, true)); //Right recursion, consume \/ token.
        return l;
    }

    //<implication>       ::= <disjunction>   | <disjunction> -> <implication>
    std::unique_ptr<Expression::Expression> p_imp(Tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<Expression::Expression> l = p_dis(ts, consume);
        if (ts.get().kind == Tokenize::Kind::IMP)
            l = Expression::Fabric::imp(std::move(l), p_imp(ts, true)); //Right recursion, consume -> token. A->B->C: A->(B->C)
        return l;
    }

    //<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
    std::unique_ptr<Expression::Expression> p_eq(Tokenize::Token_Stream& ts, bool consume) {
        std::unique_ptr<Expression::Expression> l = p_imp(ts, consume);
        while (ts.get().kind == Tokenize::Kind::EQ) //Left recursion: read implication and apply immediately. A<->B<->C: (A<->B)<->C
            l = Expression::Fabric::eq(std::move(l), p_imp(ts, true));
        return l;
    }

    //Function to go from a string to an expression.
    std::unique_ptr<Expression::Expression> parse(const std::string& input) {
        Tokenize::Token_Stream ts {input};
        return p_eq(ts, false);
    }

}

Parse::Proposition::Proposition(bool b):exp{Expression::Fabric::cons(b)} {}

Parse::Proposition::Proposition(const std::string& input) :exp{parse(input)} {}

bool Parse::Proposition::eval(const Expression::Env& env) const {
    return exp->eval(env);
}

namespace Parse {
    std::ostream& operator<<(std::ostream& os, const Proposition& p) {
        p.exp->print(false, os);
        return os;
    }
}

bool Parse::Test::test_parser(const std::string& input, const std::string& expect) {
    std::stringstream ss;
    Proposition p {input};
    ss << p;
    if(ss.str() != expect) {
        std::cout << "Input: " << input 
            << "\nExpected: " << expect 
            << "\nActual: " << ss.str() << '\n';
        return false;
    }
    return true;
}

bool Parse::Test::test() {
    return test_parser("   A<->B   <->C", "(A<->B)<->C")    //<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
        && test_parser("A->B  -> C", "A->(B->C)")           //<implication>       ::= <disjunction>   | <disjunction> -> <implication>
        && test_parser("A\\/B  \\/C", "A\\/(B\\/C)")        //<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
        && test_parser("   A/\\B    /\\C", "A/\\(B/\\C)")   //<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
        && test_parser("~~A    ", "~~A")                    //<negation>          ::= <value>         | ~ <negation>
        && test_parser("(  (  (  A ))   )", "A")            //<value>             ::= <ident>         | ( <formula> )
        && test_parser("_abcdefghijklmnopqrstuvwxyz'ABCDEFGHIJKLMNOPQRSTUVWXYZ_012345679         ", "_abcdefghijklmnopqrstuvwxyz'ABCDEFGHIJKLMNOPQRSTUVWXYZ_012345679")
        && test_parser("A->B<->C->D", "(A->B)<->(C->D)")
        && test_parser("A\\/B->C\\/D", "(A\\/B)->(C\\/D)")
        && test_parser("A/\\B\\/C/\\D", "(A/\\B)\\/(C/\\D)")
        && test_parser("~A/\\~B", "~A/\\~B")
        && test_parser("((~(((A))/\\((B)))))", "~(A/\\B)")
        && test_parser("A->B\\/C", "A->(B\\/C)")
        && test_parser("(A->B)\\/C", "(A->B)\\/C")
        && test_parser("A/\\B/\\C\\/D", "(A/\\(B/\\C))\\/D")
        && test_parser("A->A<->B->C->D", "(A->A)<->(B->(C->D))");
}
