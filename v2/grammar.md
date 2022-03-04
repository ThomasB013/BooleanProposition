<formula>           ::= <equivalence>
<equivalence>       ::= <implication>   | <equivalence> <-> <implication>
<implication>       ::= <disjunction>   | <disjunction> -> <implication>
<disjunction>       ::= <conjunction>   | <conjunction> \/ <disjunction>
<conjunction>       ::= <negation>      | <negation> /\ <conjunction>
<negation>          ::= <value>    | ~ <negation>
<value>             ::= <ident>         | ( <formula> )

<ident>             ::= [_ + letter] [_ + letter + ' + digit]*
<letter>            ::= [a-z + A-Z]
<digit>             ::= [0-9]
