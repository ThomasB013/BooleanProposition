#include "Negation.h"

Negation::Negation(Boolean* b_) :Boolean(Type::NEG), b(b_) {}

Negation::~Negation() {
	if (b) {
		b->~Boolean();
		delete b;
	}
}

bool Negation::evaluate(const std::map<std::string, bool>& eval) const {
	return !b->evaluate(eval);
}

std::string Negation::to_string(int parent) const {
	return "~" + b->to_string(static_cast<int>(Type::NEG) * 2);
}

bool Negation::equal(const Boolean* b_) const {
	if (b_->get_type() == Type::NEG) {
		Negation* n = (Negation*)b_;
		return (b->equal(n->b));
	}
	return false;
}

Boolean* Negation::deep_copy() const{
	return new Negation(b->deep_copy());
}

const Boolean* Negation::get_inner() const {
	return b;
}


