#include "arithmetic.h"


int main() {
	try {
		std::string str;
		std::cout << "Enter expression:";
		std::getline(std::cin, str);
		TArithmeticExpression example(str);
		std::cout << "Result:" << example.calculate() << std::endl;
	}
	catch (char* exp) {
		std::cout << exp << std::endl;
	}
	return 0;
}
