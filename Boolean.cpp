#include "Boolean.h"


std::string to_str(Type t) {
	switch (t) {
	case Type::NEG: return "~";
	case Type::AND: return "/\\";
	case Type::OR: return  "\\/";
	case Type::IMP: return "->";
	case Type::EQ: return "<->";
	}
	return "";
}

//this only works because of our current type enum class.
bool is_binary(Type t) {
	return (static_cast<int>(t) > static_cast<int>(Type::NEG));
}

Boolean::Boolean(Type t) :type(t) {}

Boolean::~Boolean() {}

Type Boolean::get_type() const {
	return type;
}