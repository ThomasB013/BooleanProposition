package Expressions;

import java.util.Map;
import Tokens.Token;

public class Constant implements Expression {
    private boolean val;

    public Constant(boolean v) {
        val = v;
    }

    @Override
    public String toString() {
        return (val ? Token.TRUE_SYM : Token.FALSE_SYM);
    }

    @Override
    public String skipOutterBrackets() {
        return toString();
    }
    
    @Override
    public boolean eval(Map<String, Boolean> env) {
        return val;
    }
}
