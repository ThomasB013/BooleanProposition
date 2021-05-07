#include "Parser.h"
#include "Proposition.h"

#include <iostream>
//assume well formatted string.
std::vector<std::pair<const char*, const char*>> parse_brackets(const char*& begin, const char*& end) {
	int brack = -1;
	std::vector<std::pair<const char*, const char*>> brackets;
	for (const char* p = begin; p != end; ++p) {
		if (*p == '(') {
			++brack;
			brackets.insert(brackets.begin() + brack, { p, nullptr });
		}
		if (*p == ')') {
			brackets[brack].second = p;
			--brack;
		}
	}
	while (!brackets.empty() && brackets[0].first == begin && brackets[0].second == end - 1) { //Ignore outter brackets (((B))) ->B
		brackets.erase(brackets.begin());
		begin++;
		end--;
	}
	return brackets;
}

//Checks if p is in between or on ( ) given in brackets.
bool skip_brackets(std::vector<std::pair<const char*, const char*>>& brackets, const char* p) {
	for (const auto& pair : brackets)
		if (pair.first <= p && p <= pair.second) {
			p = pair.second;
			return true;
		}
	return false;
}

Boolean* to_atom(const char* begin, const char* const end) {
	std::string id;
	while (begin != end) {
		id += *begin;
		++begin;
	}
	if (id == "T")
		return T();
	return (id == "F") ? F() : Var(id);
}


//assume well formatted string
Boolean* parser(const char* begin, const char* end) {
	auto brackets = parse_brackets(begin, end);
	Type max = Type::ATOM;
	const char* max_p = begin;
	
	for (const char* p = begin; p != end; ++p) {
		if (skip_brackets(brackets, p));
		else if (*p == '~' && max < Type::NEG) {
			max = Type::NEG;
			max_p = p;
		}
		else if (*p == '/' && *++p == '\\' && max < Type::AND) {
			max = Type::AND;
			max_p = p;
		}
		else if (*p == '\\' && *++p == '/' && max < Type::OR) {
			max = Type::OR;
			max_p = p;
		}
		else if (*p == '-' && *++p == '>' && max < Type::IMP) {
			max = Type::IMP;
			max_p = p;
		}
		else if (*p == '<' && *++p == '-' && *++p == '>' && max <= Type::EQ) {
			max = Type::EQ;
			max_p = p; //if we already found an equivalence earlier, we still parse it such that the rightmost gets evaluated as last.
		} //note that  max_p points to the last elements of the type.
	}
	switch (max) {
	case Type::NEG: return Neg(parser(max_p + 1, end)); //[~FORM]  -> ~[FROM]
	case Type::AND: return And(parser(begin, max_p - 1), parser(max_p + 1, end)); // [FORM/\FORM] -> [FORM] /\ [FORM]
	case Type::OR: return Or(parser(begin, max_p - 1), parser(max_p + 1, end)); // [FORM\/FORM] -> [FORM] \/ [FORM]
	case Type::IMP: return Imp(parser(begin, max_p - 1), parser(max_p + 1, end)); // [FORM->FORM] -> [FORM] -> [FORM]
	case Type::EQ: return Eq(parser(begin, max_p - 2), parser(max_p + 1, end));
	case Type::ATOM: return to_atom(begin, end);
	}
	return Var("There is a bug in Parser.cpp");
}

//CON = 0, ATOM = 0, NEG, AND, OR, IMP, EQ
