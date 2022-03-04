#pragma once

#include "Boolean.h"
#include <functional>

class BinaryOperator : public Boolean {
public:
	BinaryOperator(std::function<bool(bool, bool)> op, Type t, Boolean* l, Boolean* r);
	virtual ~BinaryOperator();
	virtual bool evaluate(const std::map<std::string, bool>& eval) const;
	virtual std::string to_string(int parent) const;
	virtual bool equal(const Boolean* b) const;
	virtual Boolean* deep_copy() const;

	const Boolean* get_left() const;
	const Boolean* get_right() const;
private:
	std::function<bool(bool, bool)> binOp;
	Boolean* left, * right;
};

