#pragma once
#include "expression.h"


namespace Parse {

    class Proposition {
    public:
        //Constructs a Proposition based on the constant in the parameter.
        Proposition(bool val=false);

        //Constructs a Proposition by parsing the input string. May throw a runtime exception.
        explicit Proposition(const std::string&);
        
        //Evaluates the Proposition based on an (empty) environment.
        bool eval(const Expression::Env& env ={}) const;
        
        //Function to output a proposition. Will always do brackets.
        friend std::ostream& operator<<(std::ostream&, const Proposition&);
    private:
        std::unique_ptr<Expression::Expression> exp;
    };

    //Test parsing by comparing printed (which includes brackets) vs expected bracket placements.
    namespace Test {
        void test_parser(const std::string& input, const std::string& expect);
        void test();
    }
}