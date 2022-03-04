#include <iostream>
#include "tokenizer.h"
#include "parse.h"
using namespace std;

int main() {
	Parse::Test::test();
	for (string s; getline(cin, s); ) {
		cout << Tokenize::Token_Stream{s} << '\n';	
		try {
			cout << "Parsed as: " << Parse::Proposition{s} << '\n';
		}
		catch (runtime_error& e) {
			cout << e.what() << '\n';
		}
	}
	return 0;
}