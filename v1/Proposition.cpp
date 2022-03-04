#include "Proposition.h"
#include "Boolean.h"
#include "Negation.h"
#include "Atomic.h"
#include "Boolean.h"
#include "Constant.h"
#include "BinaryOperator.h"
#include "Parser.h"

Proposition::Proposition() :b(new Constant(true)) {}

Proposition::Proposition(std::string str) : b(parser(&str[0], &str.back() + 1)) {}

Proposition::Proposition(const char* c) :b(parser(c, c + strlen(c))) {}

Proposition::Proposition(Boolean* b_) : b(b_) {}

Proposition::Proposition(const Proposition& p) {
	b = p.b->deep_copy();
}

void Proposition::operator=(const Proposition& p) {
	if (this != &p) {
		delete b;
		b = p.b->deep_copy();
	}
}

Proposition::Proposition(Proposition&& p) noexcept{
	b = p.b;
	p.b = nullptr;
}

void Proposition::operator=(Proposition&& p) noexcept {
	if (this != &p) { //I think it cannot happen that this == &p, but just to be sure.
		delete b;
		b = p.b;
		p.b = nullptr;
	}
}

Proposition::~Proposition() {
	delete b;
}

Type Proposition::get_type() const {
	return b->get_type();
}

Proposition Proposition::get_inner() const {
	if (get_type() != Type::NEG)
		return Proposition(Var("That's not too smart"));
	Negation* a = (Negation*)b;

	return Proposition(a->get_inner()->deep_copy());
}

Proposition Proposition::get_left() const {
	if (!is_binary(get_type()))
		return Proposition(Var("That's not too smart"));
	BinaryOperator* a = (BinaryOperator*)b;
	return Proposition(a->get_left()->deep_copy());
}

Proposition Proposition::get_right() const {
	if (!is_binary(get_type()))
		return Proposition(Var("That's not too smart"));
	BinaryOperator* a = (BinaryOperator*)b;
	return Proposition(a->get_right()->deep_copy());
}

std::string Proposition::to_string() const{
	return b->to_string(INIT_INT);
}

bool Proposition::operator==(const Proposition& p) const {
	return b->equal(p.b);
}

bool Proposition::evaluate(const std::map<std::string, bool>& m) const {
	return b->evaluate(m);
}


Boolean* Eq(Boolean* l, Boolean* r) {
	return new BinaryOperator([](bool p, bool q) -> bool {return ((p && q) || (!p && !q)); }, Type::EQ, l, r);
}

Boolean* Imp(Boolean* l, Boolean* r) {
	return new BinaryOperator( [](bool p, bool q) -> bool {return !p || q; }, Type::IMP, l, r);
}

Boolean* Or(Boolean* l, Boolean* r) {
	return new BinaryOperator([](bool p, bool q) -> bool {return p || q; }, Type::OR, l, r);
}

Boolean* And(Boolean* l, Boolean* r) {
	return new BinaryOperator([](bool p, bool q) -> bool {return p && q; }, Type::AND, l, r);
}

Boolean* Neg(Boolean* b) {
	return new Negation(b);
}

Boolean* Var(std::string str) {
	return new Atomic(str);
}

Boolean* T() {
	return new Constant(true);
}

Boolean* F() {
	return new Constant(false);
}


Boolean* Eq(const Proposition& l, const Proposition& r) {
	return Eq(l.b->deep_copy(), r.b->deep_copy());
}
Boolean* Imp(const Proposition& l, const Proposition& r) {
	return Imp(l.b->deep_copy(), r.b->deep_copy());
}
Boolean* Or(const Proposition& l, const Proposition& r) {
	return Or(l.b->deep_copy(), r.b->deep_copy());
}
Boolean* And(const Proposition& l, const Proposition& r) {
	return And(l.b->deep_copy(), r.b->deep_copy());
}
Boolean* Neg(const Proposition& p) {
	return Neg(p.b->deep_copy());
}