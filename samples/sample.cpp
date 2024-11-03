#include "calculator.h"

int main() {
	try {
		std::string str;
		std::cout << "Enter expression:";
		std::getline(std::cin, str);
		Calculator example(str);
		std::cout << "Result:" << example.Calculate() << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;  
	}
	return 0;
}