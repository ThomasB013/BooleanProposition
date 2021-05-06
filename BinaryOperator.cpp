#include "BinaryOperator.h"

BinaryOperator::BinaryOperator(std::function<bool(bool, bool)> op, Type t, Boolean* l, Boolean* r)
	:Boolean(t), binOp(op), left(l), right(r) {}

BinaryOperator::~BinaryOperator() {
	delete left;
	delete right;
}
bool BinaryOperator::evaluate(const std::map<std::string, bool>& eval) const {
	return binOp(left->evaluate(eval), right->evaluate(eval));
}

std::string BinaryOperator::to_string(int parent) const {
	bool brackets = false;
	if (parent / 2 < static_cast<int>(get_type()))
		brackets = true;
	else if (parent / 2 == static_cast<int>(get_type())) {
		if (static_cast<int>(get_type()) == 4 && parent % 2 == 1) {//(A->B)->C needs brackets, but A->(B->C) doesn't. So left child needs brackets.   
			brackets = true;
		}
		else if (parent / 2 == static_cast<int>(get_type()) && static_cast<int>(get_type()) == 5 && parent % 2 == 0) {//A<->(B<->C) needs brackets.
			brackets = true;
		}
	}
	return (brackets ? "(" : "") + left->to_string(static_cast<int>(get_type()) * 2 - 1) + to_str(get_type()) +
		right->to_string(static_cast<int>(get_type()) * 2) + (brackets ? ")" : "");
}

bool BinaryOperator::equal(const Boolean* b) const {
	if (get_type() == b->get_type()) {
		BinaryOperator* bin = (BinaryOperator*)b;
		return (left->equal(bin->left) && right->equal(bin->right));
	}
	return false;
}

Boolean* BinaryOperator::deep_copy() const {
	return new BinaryOperator(binOp, get_type(), left->deep_copy(), right->deep_copy());
}

const Boolean* BinaryOperator::get_left() const {
	return left;
}

const Boolean* BinaryOperator::get_right() const{
	return right;
}

