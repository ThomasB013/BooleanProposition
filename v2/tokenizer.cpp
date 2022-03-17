#include "tokenizer.h"
#include <stdexcept>

namespace {
    bool valid_first_char(char c) {
        return isalpha(c) || c == '_';    
    }

    bool valid_char(char c) {
        return isalnum(c) || c == '_' || c == '\'';
    }

    /*
    Increments i until either
    1. i is at the end of the string, or
    2. i does not point at a whitespace: isspace(input[i]) evaluates to false
    */
    void consume_space(const std::string& input, size_t& i) {
        while (i != input.size() && isspace(input[i]))
            ++i;
    }

    const std::pair<std::string, tokenize::Kind> operators[]{
        {tokenize::EQ_SYM, tokenize::Kind::EQ},
        {tokenize::IMP_SYM, tokenize::Kind::IMP},
        {tokenize::CON_SYM, tokenize::Kind::CON},
        {tokenize::DIS_SYM, tokenize::Kind::DIS},
        {tokenize::NEG_SYM, tokenize::Kind::NEG},
        {tokenize::BR_OPEN, tokenize::Kind::BR_OPEN},
        {tokenize::BR_CLOSE, tokenize::Kind::BR_CLOSE}
    };

    /*
    Tries to match an operator according to grammar.md.
    Precondition:
    1. i <= input.size();
    Effect:
    1. If the input matches any opererator OP at position i then:
    1.1 i points to the position after OP in the string input.
    1.2 tokens is appended with a Token of kind k, where k matches OP.
    1.3 true is returned.
    2. If 1 does not hold then:
    2.1 The inputs tokens and i remain unchanged.
    2.2 false is returned.
    */ 
    bool matches_operator(const std::string& input, size_t& i, std::list<tokenize::Token>& tokens) {
        for(const auto& p : operators){
            if (input.substr(i, p.first.length()) == p.first){
                tokens.push_back(tokenize::Token{p.second});
                i += p.first.length();
                return true;
            }
        }
        return false;
    }

    /*
    Precondition:
    1. i <= input.size().
    2. If i < input.size(), then valid_first_char(input[i]) is true.
    Effect:
    1. Reads and returns a name (ident) as specified by grammard.md and returns the obtained value.
    2. For more details: see the description of extract_variable.
    */
    std::string extract_name(const std::string& input, size_t& i){
        std::string name;
        while (i != input.size() && valid_char(input[i])) {
            name += input[i];
            ++i;
        }
        return name;
    }

    /*
    Subtracts a variable name N according to grammar.md.
    Precondition:
    1. i <= input.size();
    Effect:
    1. If i is out of range or valid_first_char(input[i]) is false then:
    1.1 false is returned.
    1.2 i and tokens remain unchanged.
    2. If 1 does not hold then:
    2.2. N contains input[i] and all successing characters for which valid_char is true.
    2.2. If N matches true_sym a VAL Token with value true is appended to tokens. 
    2.3. If 2.2 does not hold then if N matches false_sym a VAL Token with value false is appended to tokens.
    2.4. If neither 2.2 or 2.3 hold then a VAR Token with name n is appended to tokens. 
    2.5. i points to the position after N in the string input.
    2.6. true is returned.
    */
    bool extract_variable(const std::string& input, size_t& i, std::list<tokenize::Token>& tokens, const std::string& true_sym, const std::string& false_sym) {
        if (i == input.size() || !valid_first_char(input[i]))
            return false;
        
        std::string name = extract_name(input, i);        
        if (name == true_sym)
            tokens.push_back(tokenize::Token{true});
        else if (name == false_sym)
            tokens.push_back(tokenize::Token{false});
        else
            tokens.push_back(tokenize::Token{name});
        return true;
    }

    /*
    A list of tokens representing the input string is returned, see grammar.md.
    A runtime_error is thrown when parsing fails.
    */ 
    std::list<tokenize::Token> to_tokens(const std::string& input, const std::string& true_sym, const std::string& false_sym) {
        std::list<tokenize::Token> tokens;
        size_t i = 0;
        consume_space(input, i);
        for (; i != input.size(); consume_space(input, i))
            if (!matches_operator(input, i, tokens) && !extract_variable(input, i, tokens, true_sym, false_sym))
                throw std::runtime_error{"Unexpected input (char " + std::to_string(i) + ") in " + input};
        return tokens;
    }
}

//Namespace::Struct::Method
tokenize::Token::Token(Kind k, bool b, std::string n) :kind{k}, value{b}, name{n} {}

tokenize::Token::Token(Kind k) :Token{k, false, ""} {}

tokenize::Token::Token(bool b) :Token{Kind::VAL, b, ""} {}

tokenize::Token::Token(std::string n) :Token{Kind::VAR, false, n} {}

std::ostream& tokenize::operator<<(std::ostream& os, const Token& t) {
    os << "{"; //Token{";
    switch(t.kind){
    case Kind::EQ: //Fall through
    case Kind::IMP:
    case Kind::CON:
    case Kind::DIS:
    case Kind::NEG:
    case Kind::BR_OPEN:
    case Kind::BR_CLOSE:
        os << operators[static_cast<int>(t.kind)].first; break; //Relying on the value behind the enum class: sketchy?
    case Kind::VAL:
        os << "VAL (" << (t.value ? "true" : "false") << ')'; break;
    case Kind::VAR:
        os << t.name; break;
    case Kind::END: 
        os << "END"; break;
    default:
        os << "Unrecognized kind: " << static_cast<int>(t.kind);
    }
    os << "}";
    return os;
}

tokenize::Token_Stream::Token_Stream(const std::string& input) 
    :tokens{to_tokens(input, TRUE_SYM, FALSE_SYM)} {
    /*
    Invariant:
    tokens contains all Tokens that are not consumed yet.
    At the end there is a token with Kind::End.
    A runtime_error is thrown when a user tries to consume this last token.
    */
    tokens.push_back(Token{Kind::END});
}

tokenize::Token tokenize::Token_Stream::get() const {
    return tokens.front();
}

tokenize::Token tokenize::Token_Stream::consume() {
    if (get().kind == Kind::END) 
        throw std::runtime_error {"Token_Stream::consume(): cannot consume ending token."};
    Token t = tokens.front();
    tokens.pop_front();
    return t;
}


namespace tokenize {
    std::ostream& operator<<(std::ostream& os, const Token_Stream& ts) {
        for (const auto& token : ts.tokens)
            os << token;
        return os;
    }
}

/*
Why does the above work, but does:
std::ostream& tokenize::operator<<(std::ostream& os, const Token_Stream& ts) {
    for (const auto& token : ts.tokens)
        os << token << '\n';
    return os;
}
Give me the following warnings:
warning: ‘std::ostream& tokenize::operator<<(std::ostream&, const tokenize::Token_Stream&)’ has not been declared within ‘Tokenize’
  171 | std::ostream& tokenize::operator<<(std::ostream& os, const Token_Stream& ts) {

note: only here as a ‘friend’
   47 |         friend std::ostream& operator<<(std::ostream&, const Token_Stream&);
*/