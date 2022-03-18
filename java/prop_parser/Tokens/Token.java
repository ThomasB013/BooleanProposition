package Tokens;
public class Token {
    public static final String 
        TRUE_SYM = "T",
        FALSE_SYM = "F",
        EQ_SYM = "<->",
        IMP_SYM = "->",
        DIS_SYM = "\\/",
        CON_SYM = "/\\",
        NEG_SYM = "~",
        BR_OPEN = "(",
        BR_CLOSE = ")";


    public static SymbolKindPair[] symbolKindPairs = new SymbolKindPair[] {
        new SymbolKindPair(EQ_SYM, Kind.EQ),
        new SymbolKindPair(IMP_SYM, Kind.IMP),
        new SymbolKindPair(DIS_SYM, Kind.DIS),
        new SymbolKindPair(CON_SYM, Kind.CON),
        new SymbolKindPair(NEG_SYM, Kind.NEG),
        new SymbolKindPair(BR_OPEN, Kind.BR_OPEN),
        new SymbolKindPair(BR_CLOSE, Kind.BR_CLOSE)
    };

    private String name;
    private boolean val;
    private Kind kind;

    private Token(String n, boolean v, Kind k){
        name = n;
        val = v;
        kind = k;
    }

    public Token(String n) {
        this(n, false, Kind.VAR);
    }
    
    public Token(Boolean v) {
        this("", v, Kind.VAL);
    }
    
    public Token(Kind k) {
        this("", false, k);
    }

    @Override
    public String toString() {
        switch(kind){
            case VAL: return "{" + (val ? TRUE_SYM : FALSE_SYM) + "}";
            case VAR: return "{" + name + "}";
            default: return "{" + kind + "}";
        }
    }

    public String getName() throws Exception {
        if (kind != Kind.VAR)
            throw new Exception("Cannot get name of token which has kind " + kind);
        return name;
    }

    public boolean getValue() throws Exception {
        if (kind != Kind.VAL)
            throw new Exception("Cannot get value of token which has kind " + kind);
        return val;
    }

    public Kind getKind() {
        return kind;
    }
}

