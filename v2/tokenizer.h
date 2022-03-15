#pragma once

#include <string>
#include <list>
#include <iostream>

namespace Tokenize {
    const std::string EQ_SYM = "<->",
        IMP_SYM = "->",
        CON_SYM = "/\\",
        DIS_SYM = "\\/",
        NEG_SYM = "~",
        BR_OPEN = "(",
        BR_CLOSE = ")",
        TRUE_SYM = "T",
        FALSE_SYM = "F";

    enum class Kind {EQ, IMP, CON, DIS, NEG, BR_OPEN, BR_CLOSE, VAL, VAR, END};

    struct Token {
        Token(Kind k, bool b, std::string n);
        Token(Kind k); //Implicit value = false, name = "".
        Token(bool b); //Implicit kind = VAL, name ="";
        Token(std::string n); //Implicit kind = VAR, value = false;
        Kind kind;
        bool value;
        std::string name;
    };

    //Output function for debugging.
    std::ostream& operator<<(std::ostream&, const Token&);
    
    class Token_Stream {
    public:
        /*
        After constructing a Token_Stream with a string (see grammar.md):
        That string will be translated to tokens which can be sequentially entered by the user of this class.
        The arguments true_sym and false_sym are reserved and will be interpreted as constants.
        A runtime_error is thrown if we fail to parse the string.
        */
        Token_Stream(const std::string&);
        
        //Get the current Token.
        Token get() const;
        
        //Return the current and remove it from the list. A runtime_error is thrown when a user tries to consume this last token (which has kind Kind::END).
        Token consume(); 
        
        //Output function for debugging. Prints all tokens in the list. Seperated by newlines.
        friend std::ostream& operator<<(std::ostream&, const Token_Stream&);
    private:
        std::list<Token> tokens;
    };
}