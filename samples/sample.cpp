#include "calculator.h"

int main() {
	try {
		std::string str;
		std::cout << "Enter expression:";
		std::getline(std::cin, str);
		Calculator example(str);
		std::map< std::string, std::variant<long long, double>> operands= example.GetNameOperands();
		std::variant<long long, double> value;

		for (auto& op : operands) {
			if (!IsConst(op.first)) {
				if(isdigit(op.first[0]))  throw std::invalid_argument("Operand name cannot start with a digit: " + op.first);
				std::string value;
				std::cout << "Enter value " << op.first <<":";
				std::getline(std::cin, value);
				op.second = ConvertToVariant(value);
			}
		}

		auto v = example.Calculate(operands);
		if (std::holds_alternative<double>(v)) {
			std::cout << "Result:" << std::get<double>(v)<< std::endl;
		}
		if (std::holds_alternative<long long>(v)) {
			std::cout << "Result:" << std::get<long long>(v) << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;  
	}
	return 0;
}