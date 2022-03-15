#include <iostream>
#include "tokenizer.h"
#include "parse.h"
using namespace std;

istream& get_cmd_line(istream& in, string& to, ostream& out=cout){
	out << '$';
	in >> to;
	return in;
}

bool is_quit(const string& str) {
	return str == "quit" || str == "exit" || str == "exit()";
}

//First execute tests from parsing, then execute some interactively.
void test_parsing(istream& in=cin, ostream& out=cout) {
	if (Parse::Test::test() && Expression::Test::test_all())
		out << "Standard tests passed.\n";

	for (string s; get_cmd_line(cin, s) && !is_quit(s); cout << '\n') {
		try {
			const auto tokens = Tokenize::Token_Stream{s};	
			const auto proposition = Parse::Proposition{s};
			out << "Tokens: " << tokens << '\n';
			out << "Parsed to: " << proposition << '\n';
		}
		catch (runtime_error& e) {
			out << e.what() << '\n';
		}
	}
}

int main() {
	test_parsing();
	return 0;
}
