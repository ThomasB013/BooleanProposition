package Expressions;

import java.util.function.BiFunction;
import java.util.Map;

public class BinExp implements Expression {
    private String symbol;
    private Expression left, right;
    private BiFunction<Boolean, Boolean, Boolean> fun;

    public BinExp(String s, Expression l, Expression r, BiFunction<Boolean, Boolean, Boolean> f) {
        symbol = s;
        left = l;
        right = r;
        fun = f;
    }

    @Override
    public boolean eval(Map<String, Boolean> env) {
        return fun.apply(left.eval(env), right.eval(env));
    }
    
    @Override
    public String toString() {
        return "(" + left + symbol + right + ")";
    }

    @Override
    public String skipOutterBrackets() {
        return left.toString() + symbol + right.toString();
    }
}
