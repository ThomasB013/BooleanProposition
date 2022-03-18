package Propositions;

import java.util.HashMap;
import Tokens.Token;

public class Test {
   
    public static void main(String[] args) {
        test_parser();
        test_parser_eval();
        System.out.println("Propositions.Test executed");
    }

    private static void test_parser_eval() {
        test_parse_operator(Token.EQ_SYM, true, false, false, true);
        test_parse_operator(Token.IMP_SYM, true, true, false, true);
        test_parse_operator(Token.DIS_SYM, false, true, true, true);
        test_parse_operator(Token.CON_SYM, false, false, false, true);
        
        HashMap<String, Boolean> env = new HashMap<String, Boolean>() {{
            put("A", true);
            put("B", false);
        }};
        
        try {
            boolean passed = (new Proposition(Token.NEG_SYM + Token.FALSE_SYM)).eval(env) && !(new Proposition(Token.NEG_SYM + Token.TRUE_SYM)).eval();
            if (!passed)
                System.out.println("Parsing test failed for Token.NEG.");
                
            passed = (new Proposition("A")).eval(env) && !(new Proposition("B")).eval(env);
            if (!passed)
                System.out.println("Parsing test failed for Variables.");
            
            passed = !(new Proposition(Token.FALSE_SYM)).eval() && (new Proposition(Token.TRUE_SYM)).eval();
            if (!passed)
                System.out.println("Parsing test failed for Constants");
        }
        catch(Exception e) {
            System.out.println("Exception while execution test_parser_eval(): " + e);
        }
    }

    private static void test_parser() {
        test_parser("   A<->B   <->C", "(A<->B)<->C");    //<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
        test_parser("A->B  -> C", "A->(B->C)");           //<implication>       ::= <disjunction>   | <disjunction> -> <implication>
        test_parser("A\\/B  \\/C", "A\\/(B\\/C)");       //<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
        test_parser("   A/\\B    /\\C", "A/\\(B/\\C)");   //<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
        test_parser("~~A    ", "~~A");                    //<negation>          ::= <value>         | ~ <negation>
        test_parser("(  (  (  A ))   )", "A");            //<value>             ::= <ident>         | ( <formula> )
        test_parser("_abcdefghijklmnopqrstuvwxyz'ABCDEFGHIJKLMNOPQRSTUVWXYZ_012345679         ", "_abcdefghijklmnopqrstuvwxyz'ABCDEFGHIJKLMNOPQRSTUVWXYZ_012345679");
        test_parser("A->B<->C->D", "(A->B)<->(C->D)");
        test_parser("A\\/B->C\\/D", "(A\\/B)->(C\\/D)");
        test_parser("A/\\B\\/C/\\D", "(A/\\B)\\/(C/\\D)");
        test_parser("~A/\\~B", "~A/\\~B");
        test_parser("((~(((A))/\\((B)))))", "~(A/\\B)");
        test_parser("A->B\\/C", "A->(B\\/C)");
        test_parser("(A->B)\\/C", "(A->B)\\/C");
        test_parser("A/\\B/\\C\\/D", "(A/\\(B/\\C))\\/D");
        test_parser("A->A<->B->C->D", "(A->A)<->(B->(C->D))");
    }

    private static void test_parser(String input, String expected) {
        try  {
            Proposition p = new Proposition(input);
            if (!p.toString().equals(expected))
                System.out.println("Test failed for input " + input + ", got: " + p + ", but expected: " + expected);
        }
        catch(Exception e) {
            System.out.println("Exception: " + e + " for input: " + input);
        }
    }

    private static void test_parse_operator(String OP, boolean EXP_FF, boolean EXP_FT, boolean EXP_TF, boolean EXP_TT)  {
        try {
            if ((new Proposition(Token.FALSE_SYM + OP + Token.FALSE_SYM)).eval() ^ EXP_FF
                || (new Proposition(Token.FALSE_SYM + OP + Token.TRUE_SYM)).eval() ^ EXP_FT
                || (new Proposition(Token.TRUE_SYM + OP + Token.FALSE_SYM)).eval() ^ EXP_TF
                || (new Proposition(Token.TRUE_SYM + OP + Token.TRUE_SYM)).eval() ^ EXP_TT)
                System.out.println("Evaluation test failed for input: " + OP);
        }
        catch (Exception e) {
            System.out.println("Exepcetion: " + e + " for operator " + OP);
        }
    }
}
