package Propositions;

import Expressions.Expression;
import Expressions.Fabric;
import Tokens.TokenStream;
import Tokens.Kind;

public class PropParser {
    public static Expression parse(String input) throws Exception {
        TokenStream ts = new TokenStream(input);
        return equivalence(ts, false);
    }

/*
Recall the grammar:
<formula>           ::= <equivalence>
<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
<implication>       ::= <disjunction>   | <disjunction> -> <implication>
<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
<negation>          ::= <value>         | ~ <negation>
<value>             ::= <ident>         | ( <formula> )

<ident>             ::= [_ + letter] [_ + letter + ' + digit]*
<letter>            ::= [a-z + A-Z]
<digit>             ::= [0-9]
*/


    private static Expression equivalence(TokenStream ts, boolean consume) throws Exception {
        //<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
        Expression l = implication(ts, consume);
        while (ts.get().getKind() == Kind.EQ)
            l = Fabric.eq(l, implication(ts, true));
        return l;
    }

    private static Expression implication(TokenStream ts, boolean consume) throws Exception {
        //<implication>       ::= <disjunction>   | <disjunction> -> <implication>
        Expression l = disjunction(ts, consume);
        if (ts.get().getKind() == Kind.IMP)
            return Fabric.imp(l, implication(ts, true));
        return l;
    }

    private static Expression disjunction(TokenStream ts, boolean consume) throws Exception {
        //<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
        Expression l = conjunction(ts, consume);
        if (ts.get().getKind() == Kind.DIS)
            return Fabric.dis(l, disjunction(ts, true));
        return l;
    }

    private static Expression conjunction(TokenStream ts, boolean consume) throws Exception {
        //<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
        Expression l = negation(ts, consume);
        if (ts.get().getKind() == Kind.CON)
            return Fabric.con(l, conjunction(ts, true));
        return l;
    }

    private static Expression negation(TokenStream ts, boolean consume) throws Exception {
        //<negation>          ::= <value>         | ~ <negation>
        if (consume)
            ts.consume();
        
        if (ts.get().getKind() == Kind.NEG)
            return Fabric.neg(negation(ts, true)); 
        return value(ts, false);
    }

    private static Expression value(TokenStream ts, boolean consume) throws Exception {
        //<value>             ::= <ident>         | ( <formula> )
        if (consume)
            ts.consume();
        
        switch(ts.get().getKind()) {
            case VAR: return Fabric.var(ts.consume().getName());
            case VAL: return Fabric.constant(ts.consume().getValue());
            case BR_OPEN: 
                Expression l = equivalence(ts, true);
                if (ts.consume().getKind() != Kind.BR_CLOSE)
                    throw new Exception("Expected closing bracket after opening bracket.");
                return l;
            default:
                throw new Exception("Expected a value.");
        }


    }

}
