#include <iostream>
#include <fstream>

#include "Boolean.h"
#include "Satisfiable.h"
#include "Proposition.h"

using namespace std;

int main()
{
    /*
    Example from la-lecture-05-semantic-tableaux-ho by Engelbert Hubbers (I don't know if my university is copy-righted).

    There are three suitcases with the following statements:
    1. This suitcase is empty.
    2. The gold is in suitcase 3.
    3. Both other suitcases are empty.
    Furthermore, we know that there is gold in exaclty one suitcase, and on exaclty one suitcase there is a true statement.
    Which suitcase contains the gold and which suitcase has the true statement?
    */

    //Make propositions for each suitcase:
 

    //Then these are the following statements:
    Proposition statement1 = "~G1"; 
    Proposition statement2 = "G3";
    Proposition statement3 = "~G1/\\~G2"; 

    //The gold is in exaclty one suitcase:
    Proposition gold_in_only_sc1 = "G1/\\~G2/\\~G3"; 
    Proposition gold_in_only_sc2 = "~G1/\\G2/\\~G3";
    Proposition gold_in_only_sc3 = "~G1/\\~G2/\\G3";
    Proposition gold_in_exactly_one = Or(gold_in_only_sc1, Or(gold_in_only_sc2, gold_in_only_sc3));

    //There is only one true statement:
    Proposition only_1_true = And(statement1, And(Neg(statement2), Neg(statement3)));
    Proposition only_2_true = And(Neg(statement1), And(statement2, Neg(statement3)));
    Proposition only_3_true = And(Neg(statement1), And(Neg(statement2), statement3));
    Proposition exactly_one_true = Or(only_1_true, Or(only_2_true, only_3_true));

    //Let's see if there is a situation in which both are true: 
    //See example_cmds.txt for the commands that lead to a evaluation.

    Satisfiable s({ gold_in_exactly_one, exactly_one_true }, {});

    ifstream suit_case_in{ "suitcases.txt" };

    s.display(cout, suit_case_in);
    /*
    We get the following output:

    If the following atomics are true:
    G2
    And these are false:
    G1, G3
    Then the leftside of 'o' evaluates to true and the rightside evaluates to false:
    G1/\~G2/\~G3\/~G1/\G2/\~G3\/~G1/\~G2/\G3, ~G1/\~G3/\~(~G1/\~G2)\/~~G1/\G3/\~(~G1/\~G2)\/~~G1/\~G3/\~G1/\~G2  o

    Hence it follows that the gold is in suitcase 2, and the statement on suitcase 1 is true.
    */

    /*
    The Satisfiable class can also be used to proof that something is not satisfiable.
    Let's say we wanted to proof that (A->B)\/(A->C) <-> A->B\/C.
    */

    Proposition p = "(A->B)\\/(A->C)<->A->B\\/C";

    //The way to proof that would be to search for an evaluation of A, B and C such that it doesn't hold.
    //If we can't find that, it does hold!

    Satisfiable theorem(p);//equivalent to Satisfiable theorem1({ {}, {p} });

    //Possible steps are in theorem.txt.
    ifstream theorem_in{ "theorem.txt" };
    theorem.display(cout, theorem_in);

    /*
    We get the following output:
    There is no possible evaluation for the variables such that
    o  (A->B)\/(A->C)<->A->B\/C  can hold.
    It follows that the following propositions:
    (A->B)\/(A->C)<->A->B\/C
    Are a tautology.
    Hence, the theorem holds.
    */
}
