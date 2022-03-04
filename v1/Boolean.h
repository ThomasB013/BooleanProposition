#pragma once

#include <string>
#include <map>

//has to be larger than the largest Type.
const int INIT_INT = 1000;

//this ordering also indicates the order of evaluation.
enum class Type {
	CON = 0, ATOM = 0, NEG, AND, OR, IMP, EQ
};

bool is_binary(Type t);

std::string to_str(Type t);

class Boolean {
public:
	Boolean(Type t);
	virtual bool evaluate(const std::map<std::string, bool>& eval) const = 0;
	virtual ~Boolean();
	Type get_type() const;
	virtual std::string to_string(int parent) const = 0;
	virtual bool equal(const Boolean* b) const = 0;
	virtual Boolean* deep_copy() const = 0;
private:
	Type type;
};









