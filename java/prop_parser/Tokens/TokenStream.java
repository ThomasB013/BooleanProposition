package Tokens;

import java.util.LinkedList;

public class TokenStream {
    private LinkedList<Token> tokens = new LinkedList<>();
    private int index = 0;

    public Token consume() throws Exception {
        Token t = tokens.getFirst();
        if (t.getKind() == Kind.END)
            throw new Exception("TokenStream exhausted, cannot consume END token.");
        tokens.remove(t);
        return t;
    }

    public Token get() {
        return tokens.getFirst();
    }

    public TokenStream(String input) throws Exception {
        consume_space(input);
        while (index < input.length()) {
            if (!extract_symbol(input) && !extract_name(input))
                throw new Exception("Unexpected character (index " + index + ") in " + input);
            consume_space(input);
        }
        tokens.add(new Token(Kind.END));
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        tokens.forEach(t -> sb.append(t));
        return sb.toString();
    }


    private static boolean matches(String source, String match, int index) {
        int end = Math.min(source.length(), index + match.length());
        return source.substring(index, end).equals(match);
    }

    private boolean extract_symbol(String source) {
        for (SymbolKindPair p : Token.symbolKindPairs) {
            if (matches(source, p.symbol, index)) {
                index += p.symbol.length();
                tokens.add(new Token(p.kind));
                return true;
            }
        }
        return false;
    }

    private boolean valid_first_char(char c) {
        return Character.isLetter(c) || c == '_';
    }

    private boolean valid_char(char c) {
        return valid_first_char(c) || c == '\'' || Character.isDigit(c);
    }

    private boolean extract_name(String source) {
        StringBuilder sb = new StringBuilder();
        if (index >= source.length() || !valid_first_char(source.charAt(index)))
            return false;
        
        while(index < source.length() && valid_char(source.charAt(index))){
            sb.append(source.charAt(index));
            index++;
        }
        
        if (sb.toString().equals(Token.TRUE_SYM))
            tokens.add(new Token(true));
        else if (sb.toString().equals(Token.FALSE_SYM))
            tokens.add(new Token(false));
        else
            tokens.add(new Token(sb.toString()));
        return true;
    }

    private void consume_space(String input) {
        while (index < input.length() && Character.isWhitespace(input.charAt(index)))
            index++;
    }

}