package Propositions;

import java.util.Map;
import java.util.HashMap;
import Expressions.Expression;

public class Proposition {
    private Expression exp;

    public Proposition(String prop) throws Exception {
        exp = PropParser.parse(prop);
    }

    @Override
    public String toString() { 
        return exp.skipOutterBrackets();
    }

    public boolean eval(Map<String, Boolean> env) {
        return exp.eval(env);
    }

    public boolean eval() {
        return exp.eval(new HashMap<>()); //Empty map.
    }
}
