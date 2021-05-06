#pragma once
#include "Boolean.h"

class Atomic : public Boolean {
public:
	Atomic(std::string str);
	virtual ~Atomic();
	virtual bool evaluate(const std::map<std::string, bool>& eval) const;
	virtual std::string to_string(int parent) const;
	virtual bool equal(const Boolean* b) const;
	virtual Boolean* deep_copy() const;
private:
	std::string id;
};
