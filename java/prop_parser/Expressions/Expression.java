package Expressions;
import java.util.Map;

public interface Expression {
    @Override
    public String toString();
    public String skipOutterBrackets(); //Slightly easier to read.
    public boolean eval(Map<String, Boolean> env);
};