package Expressions;

import java.util.Map;
import java.util.HashMap;

public class Test {
    private static void expectTrue(Expression exp, String message) {
        if (!exp.eval(env))
            System.out.println("Test failed: " + message);
    }

    private static void expectFalse(Expression exp, String message) {
        if (exp.eval(env))
            System.out.println("Test failed: " + message);
    }

    private static void expectEqual(Expression exp, String str) {
        if (!exp.toString().equals(str))
            System.out.println("Test failed: " + exp + " vs " + str);
    }

    private static final Expression T = Fabric.t();
    private static final Expression F = Fabric.f();

    private static final Map<String, Boolean> env = new HashMap<String, Boolean>() {{
        put("A", true);
        put("B", false);
    }};


    public static void main(String[] args) {
        expectTrue(Fabric.eq(F, F), "F<->F");
        expectFalse(Fabric.eq(F, T), "F<->T");
        expectFalse(Fabric.eq(T, F), "T<->F");
        expectTrue(Fabric.eq(T, T), "T<->T");

        expectTrue(Fabric.imp(F, F), "F->F");
        expectTrue(Fabric.imp(F, T), "F->T");
        expectFalse(Fabric.imp(T, F), "T->F");
        expectTrue(Fabric.imp(T, T), "T->T");

        expectTrue(Fabric.eq(F, F), "F<->F");
        expectFalse(Fabric.eq(F, T), "F<->T");
        expectFalse(Fabric.eq(T, F), "T<->F");
        expectTrue(Fabric.eq(T, T), "T<->T");


        expectFalse(Fabric.dis(F, F), "F\\/F");
        expectTrue(Fabric.dis(F, T), "F\\/T");
        expectTrue(Fabric.dis(T, F), "T\\/F");
        expectTrue(Fabric.dis(T, T), "T\\/T");

        expectFalse(Fabric.con(F, F), "F/\\F");
        expectFalse(Fabric.con(F, T), "F/\\T");
        expectFalse(Fabric.con(T, F), "T/\\F");
        expectTrue(Fabric.con(T, T), "T/\\T");
        
        expectTrue(Fabric.neg(F), "~F");
        expectFalse(Fabric.neg(T), "~T");

        expectFalse(F, "F");
        expectTrue(T, "T");

        expectTrue(Fabric.var("A"), "A");
        expectFalse(Fabric.var("B"), "B");

        expectEqual(Fabric.eq(F, F), "(F<->F)");
        expectEqual(Fabric.eq(F, T), "(F<->T)");
        expectEqual(Fabric.eq(T, F), "(T<->F)");
        expectEqual(Fabric.eq(T, T), "(T<->T)");
        expectEqual(Fabric.imp(F, F), "(F->F)");
        expectEqual(Fabric.imp(F, T), "(F->T)");
        expectEqual(Fabric.imp(T, F), "(T->F)");
        expectEqual(Fabric.imp(T, T), "(T->T)");
        expectEqual(Fabric.eq(F, F), "(F<->F)");
        expectEqual(Fabric.eq(F, T), "(F<->T)");
        expectEqual(Fabric.eq(T, F), "(T<->F)");
        expectEqual(Fabric.eq(T, T), "(T<->T)");
        expectEqual(Fabric.dis(F, F), "(F\\/F)");
        expectEqual(Fabric.dis(F, T), "(F\\/T)");
        expectEqual(Fabric.dis(T, F), "(T\\/F)");
        expectEqual(Fabric.dis(T, T), "(T\\/T)");
        expectEqual(Fabric.con(F, F), "(F/\\F)");
        expectEqual(Fabric.con(F, T), "(F/\\T)");
        expectEqual(Fabric.con(T, F), "(T/\\F)");
        expectEqual(Fabric.con(T, T), "(T/\\T)");
        expectEqual(Fabric.neg(F), "~F");
        expectEqual(Fabric.neg(T), "~T");
        expectEqual(F, "F");
        expectEqual(T, "T");
        expectEqual(Fabric.var("A"), "A");
        expectEqual(Fabric.var("B"), "B");

        System.out.println("Expressions.Test executed");
    }   
}
