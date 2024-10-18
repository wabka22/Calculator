#include "arithmetic.h"


int main() {
	try {
		std::string str;
		std::cout << "Enter expression:";
		std::getline(cin, str);
		TArithmeticExpression example(str);
		std::cout << "Result:" << example.calculate() << endl;
	}
	catch (char* exp) {
		cout << exp << endl;
	}
	return 0;
}