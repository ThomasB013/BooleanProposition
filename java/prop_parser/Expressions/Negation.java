package Expressions;

import Tokens.Token;
import java.util.Map;

public class Negation implements Expression {
    private Expression exp;
    
    public Negation(Expression e) {
        exp = e;
    }
    
    @Override
    public String toString() {
        return Token.NEG_SYM + exp.toString();
    }

    @Override
    public String skipOutterBrackets() {
        return toString();
    }

    @Override
    public boolean eval(Map<String, Boolean> env) {
        return !exp.eval(env);
    }
}
