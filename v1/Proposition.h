#pragma once
#include "Boolean.h"

class Proposition
{
public:
	//Default constructor: will set the proposition to True
	Proposition();
	//From string representation to proposition.
	Proposition(std::string str);
	Proposition(const char* c);
	//Boolean* b constructor: use this with the helper functions.
	Proposition(Boolean* b);
	//Copy constructor
	Proposition(const Proposition& p);
	//Copy assignment
	void operator=(const Proposition& p);
	//Move constructor
	Proposition(Proposition&& p) noexcept;
	//Move assignment
	void operator=(Proposition&& p) noexcept;
	//Destructor: clear up the tree
	~Proposition();

	//return the type of the proposition: negation, implication etc...
	Type get_type() const;

	//returns the inner proposition of a negation, if the proposition this is called on is not a negation,
	//it will return an Atomic proposition with id equal to "That's not too smart" ;).
	Proposition get_inner() const;


	//same as get_inner(), but then for a BinaryOperators: AND, OR etc.
	Proposition get_left() const;
	Proposition get_right() const;


	//String representation
	std::string to_string() const;
	
	//Equal operator: doesn't check equivalence, it checks the literal equality. A/\B == A/\B is true, but A/\B == B/\A will evaluate to false.
	bool operator==(const Proposition& p) const;

	//Returns the value of the proposition given a map that connects the variable ID's to truth values. 
	//If they are not present in the map m, false is assumed.
	bool evaluate(const std::map<std::string, bool>& m) const;


	//The following functions will work the same as helper functions, but making a deep copy of the provided propositions.
	friend Boolean* Eq(const Proposition& l, const Proposition& r);
	friend Boolean* Imp(const Proposition& l, const Proposition& r);
	friend Boolean* Or(const Proposition& l, const Proposition& r);
	friend Boolean* And(const Proposition& l, const Proposition& r);
	friend Boolean* Neg(const Proposition& p);
private:
	Boolean* b;
};

//HELPER FUNCTIONS:
Boolean* Eq(Boolean* l, Boolean* r); //l <-> r 
Boolean* Imp(Boolean* l, Boolean* r); //l -> r
Boolean* Or(Boolean* l, Boolean* r); //l \/ r
Boolean* And(Boolean* l, Boolean* r); // l /\ r
Boolean* Neg(Boolean* b); // ~b
Boolean* Var(std::string str); // returns a variable
Boolean* T(); // returns a constant True
Boolean* F(); // returns a constant False

