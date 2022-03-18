package Expressions;

import Tokens.Token;

public class Fabric {
    public static BinExp eq(Expression l, Expression r) {
        return new BinExp(Token.EQ_SYM, l, r, (Boolean a, Boolean b) -> !(a^b));
    }
    
    public static BinExp imp(Expression l, Expression r) {
        return new BinExp(Token.IMP_SYM, l, r, (Boolean a, Boolean b) -> !a || b);
    }
    
    public static BinExp dis(Expression l, Expression r) {
        return new BinExp(Token.DIS_SYM, l, r, (Boolean a, Boolean b) -> a || b);
    }
    
    public static BinExp con(Expression l, Expression r) {
        return new BinExp(Token.CON_SYM, l, r, (Boolean a, Boolean b) -> a && b);
    }

    public static Negation neg(Expression e) {
        return new Negation(e);
    }

    public static Variable var(String n) {
        return new Variable(n);
    }

    public static Constant constant(boolean v) {
        return new Constant(v);
    }

    public static Constant t() {
        return new Constant(true);
    }

    public static Constant f() {
        return new Constant(false);
    }
}