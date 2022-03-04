# BooleanProposition
Implementation of boolean logic and some operators. Plus a helper for checking satisfiability.

There are two examples in main.cpp. Feel free to run them.

By using Boolean* you can make memory leaks very easy. I suggest that for making propositions you stick to this format:
Proposition [name] = [another proposition or a combination of helper functions defined in Proposition.h]

There are the following classes:
Satisfiable,  this is the helper (see main.cpp)
Boolean: parent class of all booleans.
Atomic: boolean variables like 'A' : {0, 1}.
Constant: TRUE or FALSE
Negation: ~(Some boolean value). Works like the ! operator.
BinaryOperator: all booleans that have a operator that takes two operants, e.g. A AND B.
The operators provided here are AND, OR, IMP, EQ.

Parser: helps to read a well-formatted string into a proposition.

Proposition: As explained on line 6-7 of this .txt file, it helps eliminate naked new/deletes. 

The Boolean classes are linked together in a tree-like matter.
For example A<->B->C would have the following structure:

  <->
 /   \
A    ->
    /   \
   B     C
(Tree will only show in raw format).
