#include "Constant.h"


Constant::Constant(bool val) :Boolean(Type::CON), value(val) {}

Constant::~Constant() {}

bool Constant::evaluate(const std::map<std::string, bool>& eval) const {
	return value;
}

std::string Constant::to_string(int parent) const {
	return value ? "T" : "F";
}

bool Constant::equal(const Boolean* b) const {
	if (b->get_type() == Type::CON) {
		Constant* c = (Constant*)b;
		return (value == c->value);
	}
	return false;
}

Boolean* Constant::deep_copy() const {
	return new Constant(value);
}