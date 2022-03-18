package Expressions;

import java.util.Map;

public class Variable implements Expression {
    private String name;

    public Variable(String n){
        name = n;
    }

    @Override
    public String skipOutterBrackets() {
        return toString();
    }

    @Override
    public String toString() {
        return name;
    }

    @Override
    public boolean eval(Map<String, Boolean> env) {
        return env.get(name);
    }
}
