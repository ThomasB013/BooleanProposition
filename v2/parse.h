#pragma once
#include "expression.h"


namespace parse {

    class Proposition {
    public:
        //Constructs a Proposition based on the constant in the parameter.
        Proposition(bool val=false);

        //Constructs a Proposition by parsing the input string. May throw a runtime exception.
        explicit Proposition(const std::string&);
        explicit Proposition(const char*); //Fixes bug: const char* goes to bool by operator bool() rather than to std::string by constructor.

        //Evaluates the Proposition based on an (empty) environment.
        bool eval(const expression::Env& env ={}) const;
        
        //Function to output a proposition. Will always do brackets.
        friend std::ostream& operator<<(std::ostream&, const Proposition&);
    private:
        std::unique_ptr<expression::Expression> exp;
    };

    //Test parsing by comparing printed (which includes brackets) vs expected bracket placements.
    namespace test {
        bool test_parser_eval();
        bool test_parser();
    }
}