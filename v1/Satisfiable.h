#pragma once
#include "Proposition.h"
#include <vector>
#include <iostream>

class Satisfiable
{
public:
	//Start with only p in the false part. If this isn't satisfiable it follows that p is a tautology.
	Satisfiable(const Proposition& p);
	//Check if all propositions in t to true and in f to false is satisfiable.
	//If this isn't satisfiable then f is a logical consequence of t.
	Satisfiable(const std::vector<Proposition>& t, const std::vector<Proposition>& f);
	//String representation of a single branche. "  o  " is used to seperate true from false. Left is true and right is false.
	static std::string to_string(const std::pair<std::vector<Proposition>, std::vector<Proposition>>& branch);

	bool valid_index_r(int i) const;
	bool valid_index_l(int i) const;

	bool neg_r(int i);
	bool neg_l(int i);

	bool and_r(int i);
	bool and_l(int i);

	bool or_r(int i);
	bool or_l(int i);
	
	bool imp_r(int i);
	bool imp_l(int i);

	bool eq_r(int i);
	bool eq_l(int i);

	void clear_r();
	void clear_l();

	bool close_r(int i);
	bool close_l(int i);

	bool close(int left, int right);
	bool open();

	bool focus(int i);

	void display(std::ostream& os = std::cout, std::istream& is = std::cin);

private:
	bool solved = false;
	bool is_satisfiable = false;
	int cur_branch = 0;
	std::pair<std::vector<Proposition>, std::vector<Proposition>> start_state;
	std::vector<std::pair<std::vector<Proposition>, std::vector<Proposition>>> branches;


	void show_all_branches(std::ostream& os) const;
	void execute(std::string command, std::ostream& os, std::istream& is);
	void get_help(std::ostream& os) const;
};


