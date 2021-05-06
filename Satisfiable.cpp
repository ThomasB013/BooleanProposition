#include "Satisfiable.h"

Satisfiable::Satisfiable(const Proposition& p) : start_state({ {}, {p} }) {
	branches.push_back(start_state);
}

Satisfiable::Satisfiable(const std::vector<Proposition>& t, const std::vector<Proposition>& f) : start_state({ t, f }) {
	branches.push_back(start_state);
}

std::string Satisfiable::to_string(const std::pair<std::vector<Proposition>, std::vector<Proposition>>& branch) {
	std::string ans;
	for (const Proposition& p : branch.first) {
		ans += p.to_string() + ", ";
	}
	if (!branch.first.empty())
		ans[ans.size() - 2] = ' ';
	ans += "o  ";
	for (const Proposition& p : branch.second) {
		ans += p.to_string() + ", ";
	}
	if (!branch.second.empty())
		ans[ans.size() - 2] = 0;
	return ans;
}

bool Satisfiable::valid_index_r(int i) const {
	return (i >= 0 && i < branches[cur_branch].second.size());
}
bool Satisfiable::valid_index_l(int i) const {
	return (i >= 0 && i < branches[cur_branch].first.size());
}

bool Satisfiable::neg_r(int i) {
	if (!valid_index_r(i) || branches[cur_branch].second[i].get_type() != Type::NEG)
		return false;
	branches[cur_branch].first.push_back(branches[cur_branch].second[i].get_inner());
	branches[cur_branch].second.erase(branches[cur_branch].second.begin() + i);
	return true;
}

bool Satisfiable::neg_l(int i) {
	if (!valid_index_l(i) || branches[cur_branch].first[i].get_type() != Type::NEG)
		return false;
	branches[cur_branch].second.push_back(branches[cur_branch].first[i].get_inner());
	branches[cur_branch].first.erase(branches[cur_branch].first.begin() + i);
	return true;
}

bool Satisfiable::and_r(int i) {
	if (!valid_index_r(i) || branches[cur_branch].second[i].get_type() != Type::AND)
		return false;

	Proposition and_statement = branches[cur_branch].second[i]; //copy the A/\B
	branches[cur_branch].second.erase(branches[cur_branch].second.begin() + i); //remove A/\B

	branches.push_back(branches[cur_branch]); //copy the whole branch over. if A/\B is false, then either
	branches[cur_branch].second.push_back(and_statement.get_left()); //A is false
	branches.back().second.push_back(and_statement.get_right()); //or B is false.
	return true;
}

bool Satisfiable::and_l(int i) {
	if (!valid_index_l(i) || branches[cur_branch].first[i].get_type() != Type::AND)
		return false;
	//if A/\B is true, then
	branches[cur_branch].first.push_back(branches[cur_branch].first[i].get_left()); //A is true
	branches[cur_branch].first.push_back(branches[cur_branch].first[i].get_right()); // and B is true.
	branches[cur_branch].first.erase(branches[cur_branch].first.begin() + i);
	return true;
}

bool Satisfiable::or_r(int i) {
	if (!valid_index_r(i) || branches[cur_branch].second[i].get_type() != Type::OR)
		return false;
	//if A\/B is false, then
	branches[cur_branch].second.push_back(branches[cur_branch].second[i].get_left()); //A is false
	branches[cur_branch].second.push_back(branches[cur_branch].second[i].get_right()); //and B is false
	branches[cur_branch].second.erase(branches[cur_branch].second.begin() + i);
	return true;
}
bool Satisfiable::or_l(int i) {
	if (!valid_index_l(i) || branches[cur_branch].first[i].get_type() != Type::OR)
		return false;

	Proposition or_statement = branches[cur_branch].first[i]; //copy the A\/B
	branches[cur_branch].first.erase(branches[cur_branch].first.begin() + i); //remove A\/B

	branches.push_back(branches[cur_branch]); //copy the whole branch over. if A\/B is true, then either
	branches[cur_branch].first.push_back(or_statement.get_left()); //A is true.
	branches.back().first.push_back(or_statement.get_right()); //or B is true.
	return true;
}

bool Satisfiable::imp_r(int i) {
	if (!valid_index_r(i) || branches[cur_branch].second[i].get_type() != Type::IMP)
		return false;
	//if A -> B is false, then
	branches[cur_branch].first.push_back(branches[cur_branch].second[i].get_left()); //A is true
	branches[cur_branch].second.push_back(branches[cur_branch].second[i].get_right()); //B is false.
	branches[cur_branch].second.erase(branches[cur_branch].second.begin() + i);
	return true;
}
bool Satisfiable::imp_l(int i) {
	if (!valid_index_l(i) || branches[cur_branch].first[i].get_type() != Type::IMP)
		return false;
	Proposition imp_statement = branches[cur_branch].first[i]; //copy A -> B
	branches[cur_branch].first.erase(branches[cur_branch].first.begin() + i); //delete A->B
	
	branches.push_back(branches[cur_branch]); //copy the whole branch. If A->B is true, then either
	branches[cur_branch].first.push_back(imp_statement.get_right()); //B is true
	branches.back().second.push_back(imp_statement.get_left()); //A is false.
	return true;
}

bool Satisfiable::eq_r(int i) {
	if (!valid_index_r(i) || branches[cur_branch].second[i].get_type() != Type::EQ)
		return false;
	Proposition eq_statement = branches[cur_branch].second[i]; //copy A<->B
	branches[cur_branch].second.erase(branches[cur_branch].second.begin() + i); //erase A<->B.


	branches.push_back(branches[cur_branch]); //if A<->B is true, then either
	branches[cur_branch].first.push_back(eq_statement.get_left()); //A is true and
	branches[cur_branch].second.push_back(eq_statement.get_right()); //B is false.

	branches.back().second.push_back(eq_statement.get_left()); //or A is false and
	branches.back().first.push_back(eq_statement.get_right()); //B is true.
	return true;
}
bool Satisfiable::eq_l(int i) {
	if (!valid_index_l(i) || branches[cur_branch].first[i].get_type() != Type::EQ)
		return false;
	Proposition eq_statement = branches[cur_branch].first[i]; //copy A<->B
	branches[cur_branch].first.erase(branches[cur_branch].first.begin() + i); //erase A<->B.

	branches.push_back(branches[cur_branch]); //if A<->B is true, then either
	branches[cur_branch].first.push_back(eq_statement.get_left()); //A is true and
	branches[cur_branch].first.push_back(eq_statement.get_right()); //B is true.

	branches.back().second.push_back(eq_statement.get_left()); //or they're both false.
	branches.back().second.push_back(eq_statement.get_right());
	return true;
}


void Satisfiable::clear_r() {
	for (int i = 0; i < branches[cur_branch].second.size(); ++i) {
		if (branches[cur_branch].second[i] == Proposition(F())) { //A false always evaluates to false.
			branches[cur_branch].second.erase(branches[cur_branch].second.begin() + i);
			--i;
		}
		for (int j = i + 1; j < branches[cur_branch].second.size(); ++j) {
			if (branches[cur_branch].second[i] == branches[cur_branch].second[j]) {
				branches[cur_branch].second.erase(branches[cur_branch].second.begin() + j);
				--j;
			}
		}
	}
}

void Satisfiable::clear_l() {
	for (int i = 0; i < branches[cur_branch].first.size(); ++i) {
		if (branches[cur_branch].first[i] == Proposition(T())) { //A true always evaluates to true.
			branches[cur_branch].first.erase(branches[cur_branch].first.begin() + i);
			--i;
		}
		for (int j = i + 1; j < branches[cur_branch].first.size(); ++j) {
			if (branches[cur_branch].first[i] == branches[cur_branch].first[j]) {
				branches[cur_branch].first.erase(branches[cur_branch].first.begin() + j);
				--j;
			}
		}
	}
}


bool Satisfiable::close_r(int i) {
	if (!valid_index_r(i) || branches[cur_branch].second[i].get_type() != Type::CON)
		return false;
	branches.erase(branches.begin() + cur_branch);
	cur_branch = 0;
	if (branches.empty())
		solved = true;
	return true;
}

bool Satisfiable::close_l(int i) {
	if (!valid_index_l(i) || branches[cur_branch].first[i].get_type() != Type::CON)
		return false;
	branches.erase(branches.begin() + cur_branch);
	cur_branch = 0;
	if (branches.empty())
		solved = true;
	return true;
}

bool Satisfiable::close(int left, int right) {
	if (!valid_index_l(left) || !valid_index_r(right) || !(branches[cur_branch].first[left] == branches[cur_branch].second[right]))
		return false;
	branches.erase(branches.begin() + cur_branch);
	cur_branch = 0;
	if (branches.empty())
		solved = true;
	return true;
}

bool Satisfiable::open() {
	for (const Proposition& p : branches[cur_branch].first)
		if (p.get_type() != Type::ATOM)
			return false;
	for (const Proposition& p : branches[cur_branch].second)
		if (p.get_type() != Type::ATOM)
			return false;
	for (int i = 0; i < branches[cur_branch].first.size(); ++i)
		for (int j = 0; j < branches[cur_branch].second.size(); ++j) 
			if (branches[cur_branch].first[i] == branches[cur_branch].second[i])
				return false;
	solved = true;
	is_satisfiable = true;
	return true;
}


bool Satisfiable::focus(int i) {
	if (i >= 0 && i < branches.size()) {
		cur_branch = i;
		return true;
	}
	return false;
}

void Satisfiable::display(std::ostream& os, std::istream& is) {
	os	<< "Trying to proof that the following proposition combination is satisfiable:\n"
		<< "\nWhere the propositions to the left of the 'o' should evaluate to true, and to the left side of the 'o' to false."
		<< "\n\nIf you get stuck, consider typing 'help' for help.\n\n";
	std::string command;
	while (!solved) {
		os << "\nCurrent branch (" << cur_branch << "/" << branches.size() - 1 << "):\n"
			<< to_string(branches[cur_branch]) << '\n';
		is >> command;
		execute(command, os, is);
	}
	if (!is_satisfiable) {
		os << "\n\nThere is no possible evaluation for the variables such that\n"
			<< to_string(start_state) << " can hold.\n"
			<< "It follows that the following propositions:\n";
		for (const Proposition& p : start_state.second)
			os << p.to_string() << '\n';
		if (start_state.first.empty())
			os << "Are a tautology.\n";
		else {
			os << "Are a logical consequence of:\n";
			for (const Proposition& p : start_state.first)
				os << p.to_string() << '\n';
		}
	}
	else {
		std::string message = "\n\nIf the following atomics are true:\n";
		for (const Proposition& p : branches[cur_branch].first)
			message +=  p.to_string() + ", ";
		if (!branches[cur_branch].first.empty())
			message.replace(message.end() - 2, message.end(), " ");
		message += "\nAnd these are false:\n";
		for (const Proposition& p : branches[cur_branch].second)
			message += p.to_string() + ", ";
		if (!branches[cur_branch].second.empty())
			message.replace(message.end() - 2, message.end(), " ");
		message += "\nThen the leftside of 'o' evaluates to true and the rightside evaluates to false:\n";
		os << message << to_string(start_state) << "\n";
	}
}


void Satisfiable::show_all_branches(std::ostream& os) const {
	for (int i = 0; i < branches.size(); ++i)
		os << i << ".\t" << to_string(branches[i]) << '\n';
}

bool is_number(std::string str) {
	for (char c : str)
		if (!isdigit(c))
			return false;
	return true;
}


void Satisfiable::execute(std::string command, std::ostream& os, std::istream& is) {
	bool succes = true;
	if (command == "help")
		get_help(os);
	else if (command == "open")
		succes = open();
	else if (command == "clearR")
		clear_r();
	else if (command == "clearL")
		clear_l();
	else if (command == "showall")
		show_all_branches(os);
	else { //all other commands need at least one integer.
		std::string i_str;
		int i = -1; //if this doesn't get changed by i_str (because we failed to read then succes will always be false).
		is >> i_str;
		if (is.fail())
			os << "Unrecognized command. Type 'help' for help.\n";
		else {
			if (is_number(i_str))
				i = std::stoi(i_str);
			if (command == "~R")
				succes = neg_r(i);
			else if (command == "~L")
				succes = neg_l(i);
			else if (command == "/\\R")
				succes = and_r(i);
			else if (command == "/\\L")
				succes = and_l(i);
			else if (command == "\\/R")
				succes = or_r(i);
			else if (command == "\\/L")
				succes = or_l(i);
			else if (command == "<->R")
				succes = eq_r(i);
			else if (command == "<->L")
				succes = eq_l(i);
			else if (command == "->R")
				succes = imp_r(i);
			else if (command == "->L")
				succes = imp_l(i);
			else if (command == "focus")
				succes = focus(i);
			else if (command == "closeR")
				succes = close_r(i);
			else if (command == "closeL")
				succes = close_l(i);
			else {
				std::string j_str;
				int j = -1;
				is >> j_str;
				if (is_number(j_str))
					j = std::stoi(j_str);
				if (is.fail() || command != "close")
					os << "Unrecognized command. Type 'help' for help.\n";
				else {
					succes = close(i, j);
				}
			}
		}
	}
	if (!succes)
		os << "Failed to execute " << command << '\n';
}

void Satisfiable::get_help(std::ostream& os) const {
	os << "The following commands are avalaible:\n\n"
		<< "~R [i], ~L [i], /\\R [i], /\\L [i], \\/R [i], \\/L [i], <->R [i], <->L [i], ->R [i], ->L [i]\n"
		<< "showall, open, clearR, clearL, focus [i], closeR [i], closeL [i], close [i] [j].\n";
}
