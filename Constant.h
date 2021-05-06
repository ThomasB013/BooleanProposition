#pragma once
#include "Boolean.h"
//Either true or false, is there a way to do this with an enum? Because there are only 2 instances
//known at compile time.


class Constant : public Boolean {
public:
	Constant(bool val);
	virtual ~Constant();
	virtual bool evaluate(const std::map<std::string, bool>& eval) const;
	virtual std::string to_string(int parent) const;
	virtual bool equal(const Boolean* b) const;
	virtual Boolean* deep_copy() const;
private:
	bool value;
};


