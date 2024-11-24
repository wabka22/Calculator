#include "calculator.h"

int main() {
    try {
        std::string str;
        std::cout << "Enter expression:";
        std::getline(std::cin, str);
        Calculator example(str);
        std::map<std::string, std::variant<long long, double, Polynomial>> operands = example.GetNameOperands();
        std::variant<long long, double, Polynomial> value;

        for (auto& op : operands) {
            if (!IsConst(op.first)) {
                std::string value;
                std::cout << "Enter value for " << op.first << " (or press no to skip): ";
                std::getline(std::cin, value);
                if (value == "no") value =op.first;
                op.second = ConvertToVariant(value);
            }
        }

        auto v = example.Calculate(operands);
        if (std::holds_alternative<double>(v)) {
            std::cout << "Result:" << std::get<double>(v) << std::endl;
        }
        if (std::holds_alternative<long long>(v)) {
            std::cout << "Result:" << std::get<long long>(v) << std::endl;
        }
        if (std::holds_alternative<Polynomial>(v)) {
            std::cout << "Result:" << std::get<Polynomial>(v) << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
