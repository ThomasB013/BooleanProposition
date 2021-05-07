#include "Atomic.h"

Atomic::Atomic(std::string str) :Boolean(Type::ATOM), id(str) {}

bool Atomic::evaluate(const std::map<std::string, bool>& eval) const {
	std::map<std::string, bool>::const_iterator it = eval.find(id);
	return (it == eval.end()) ? false : it->second; //return false as default.
}

std::string Atomic::to_string(int parent) const {
	return id;
}

bool Atomic::equal(const Boolean* b) const {
	if (b->get_type() == Type::ATOM) {
		Atomic* a = (Atomic*)b;
		return (id == a->id);
	}
	return false;
}

Boolean* Atomic::deep_copy() const {
	return new Atomic(id);
}