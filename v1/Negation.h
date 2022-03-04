#pragma once
#include "Boolean.h"
//In boolean logic there is only one unary operator: the negation.
class Negation : public Boolean {
public:
	Negation(Boolean* b_);
	virtual ~Negation();
	virtual bool evaluate(const std::map<std::string, bool>& eval) const;
	virtual std::string to_string(int parent) const;
	virtual bool equal(const Boolean* b) const;
	virtual Boolean* deep_copy() const;
	const Boolean* get_inner() const;
private:
	Boolean* b;
};

