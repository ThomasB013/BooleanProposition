#pragma once

#include <map>
#include <string>
#include <memory>
#include <iostream>

/*
We use memory to avoid memory leaks.
This allows to use std::unique_ptr<Expression>.
*/

namespace Expression {
    using Var_type = std::string;
    using Env = std::map<Var_type, bool>;
    //According to grammar.md, this can also be used for the ordering.
    enum Kind { eq=0, imp, dis, con, neg, val}; 

    struct Expression {
        virtual bool eval(const Env& =Env{}) const =0;
        virtual void print(bool brackets, std::ostream& os=std::cout) const =0;
        virtual ~Expression() {}
    };

    struct Bin_Exp : Expression {
        Bin_Exp(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        void print(bool brackets, std::ostream& os=std::cout) const override;
        virtual std::string get_symbol() const =0;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
    };

    struct Equivalence final : Bin_Exp {
        Equivalence(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        static const std::string SYMBOL; 
        std::string get_symbol() const override;
        bool eval(const Env& =Env{}) const override;
    };

    struct Implication final : Bin_Exp {
        Implication(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        static const std::string SYMBOL;
        std::string get_symbol() const override;
        bool eval(const Env& =Env{}) const override;
    };

    struct Disjunction final : Bin_Exp {
        Disjunction(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        static const std::string SYMBOL; 
        std::string get_symbol() const override;
        bool eval(const Env& =Env{}) const override;
    };   

    struct Conjunction final : Bin_Exp {
        Conjunction(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        static const std::string SYMBOL;
        std::string get_symbol() const override;
        bool eval(const Env& =Env{}) const override;
    };


    struct Un_Exp : Expression {
        Un_Exp(std::unique_ptr<Expression> e);
         std::unique_ptr<Expression> exp;
    };

    struct Negation final : Un_Exp {
        Negation( std::unique_ptr<Expression> e);
        void print(bool, std::ostream& os=std::cout) const override;
        bool eval(const Env& =Env{}) const override;
    };

    struct Variable final : Expression {
        Variable(const Var_type& n) :name{n} {}
        void print(bool, std::ostream& os=std::cout) const override;
        bool eval(const Env& =Env{}) const override;
        Var_type name;
    };

    struct Constant final : Expression {
        Constant(bool b=false) :val{b} {}
        void print(bool, std::ostream& os=std::cout) const override;
        bool eval(const Env& =Env{}) const override;
        bool val;
    };

    //Fabric contains helper functions.
    namespace Fabric {
        std::unique_ptr<Equivalence> eq(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        std::unique_ptr<Implication> imp(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        std::unique_ptr<Disjunction> dis(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r); 
        std::unique_ptr<Conjunction> con(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r);
        std::unique_ptr<Negation> neg(std::unique_ptr<Expression> e);
        std::unique_ptr<Variable> var(const Var_type& name);
        std::unique_ptr<Constant> cons(bool b);
        std::unique_ptr<Constant> t();
        std::unique_ptr<Constant> f();
    }

    //Test implementation against truth tables.
    //For this the Fabric functions are used and the eval() member function of Expression.
    namespace Test {
        bool test_all();
        bool test_eq();
        bool test_imp();
        bool test_dis();
        bool test_con();
        bool test_neg();
        bool test_var();
        bool test_const();
    }
}