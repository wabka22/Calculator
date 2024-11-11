#ifndef _CACLULATOR_H
#define _CACLULATOR_H
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <variant>
#include <cmath>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <stack>

class Calculator {
private:
    std::string infix_;
    std::vector<std::string> postfix_;
    std::vector<std::string> lexems_;
    std::map<std::string, std::variant<long long, double>> operands;
    static std::map<std::string, int> operator_priority;

private:
    void Parse();
    std::string ToPostfix();
    bool IsOperator(const char c) const noexcept;
    bool IsDigitOrLetter(char c) const noexcept;
    bool IsParenthesis(const char c) const noexcept;
    bool IsFunction(const std::string& str) const noexcept;
    void RemoveSpaces(std::string& str) const noexcept;
    bool IsCorrectExpression(const std::string str) const noexcept;

public:
    Calculator(const std::string& infix_);
    std::variant<long long, double> Calculate(const std::map<std::string, std::variant<long long, double>>& values = {});
    std::map<std::string, std::variant<long long, double>>& GetNameOperands() { return operands; }
};

bool Calculator::IsCorrectExpression(const std::string str) const noexcept {
    int count = 0;
    for (char c : str) {
        if (c == '(') count++;
        else if (c == ')') count--;
        if (count < 0) return false;
    }
    return (count == 0);
}

Calculator::Calculator(const std::string& _infix) {
    if (_infix.empty()) {
        throw std::invalid_argument("Expression is empty");
    }
    infix_ = _infix;
    RemoveSpaces(infix_);
    if (!IsCorrectExpression(infix_)) {
        throw std::invalid_argument("Incorrect number of parentheses");
    }
    ToPostfix();
}

std::map<std::string, int> Calculator::operator_priority = {
    {"*", 3},
    {"%", 3},
    {"/", 3},
    {"+", 2},
    {"-", 2},
    {"(", 1},
    {")", 1}
};

void Calculator::RemoveSpaces(std::string& str) const noexcept {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

bool Calculator::IsOperator(char c) const noexcept {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool Calculator::IsParenthesis(char c) const noexcept {
    return (c == '(' || c == ')');
}

bool Calculator::IsFunction(const std::string& s) const noexcept {
    return (s == "exp" || s == "sin" || s == "cos" || s == "lg" || s == "log" || s == "sqrt");
}

bool IsConst(const std::string& s) noexcept {
    for (char c : s) {
        if (!isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}

bool Calculator::IsDigitOrLetter(char c) const noexcept {
    return (isdigit(c) || c == '.' || isalpha(c));
}

void Calculator::Parse() {
    std::string currentElement;
    for (char c : infix_) {
        if (IsOperator(c) || IsParenthesis(c)) {
            if (!currentElement.empty()) {
                lexems_.push_back(currentElement);
                currentElement.clear();
            }
            lexems_.push_back(std::string(1, c));
        }
        else if (IsDigitOrLetter(c)) {
            currentElement += c;
        }
        else {
            throw std::invalid_argument("Invalid character in expression");
        }
    }
    if (!currentElement.empty()) {
        lexems_.push_back(currentElement);
    }
}

std::string Calculator::ToPostfix() {
    Parse();
    std::stack<std::string> st;
    std::string postfixExpression;

    for (const std::string& item : lexems_) {
        if (item == "(") {
            st.push(item);
        }
        else if (item == ")") {
            while (st.top() != "(") {
                postfixExpression += st.top();
                postfix_.push_back(st.top());
                st.pop();
            }
            st.pop();
        }
        else if (IsOperator(item[0])) {
            while (!st.empty() && operator_priority[item] <= operator_priority[st.top()]) {
                postfixExpression += st.top();
                postfix_.push_back(st.top());
                st.pop();
            }
            st.push(item);
        }
        else if (IsFunction(item)) {
            st.push(item);
        }
        else {
            operands[item] = IsConst(item) ? stod(item) : 0.0;
            postfix_.push_back(item);
            postfixExpression += item;
        }
    }

    while (!st.empty()) {
        postfixExpression += st.top();
        postfix_.push_back(st.top());
        st.pop();
    }

    return postfixExpression;
}

std::variant<long long, double> Calculator::Calculate(const std::map<std::string, std::variant<long long, double>>& values) {
    for (auto& val : values) {
        try {
            operands.at(val.first) = val.second;
        }
        catch (std::out_of_range& e) {}
    }

    std::stack<std::variant<long long, double>> st;

    auto apply_binary_operation = [](auto a, auto b, const auto& op) -> std::variant<long long, double> {
        return std::visit([&](auto type_a, auto type_b) {
            return std::variant<long long, double>{op(type_a, type_b)};
            }, a, b);
        };

    auto apply_unary_operation = [](std::variant<long long, double> a, const auto& op) -> std::variant<long long, double> {
        return std::visit([&](auto type_a) {
            return std::variant<long long, double>{ op(type_a) };
            }, a);
        };

    for (const std::string& lexem : postfix_) {
        if (lexem == "+" || lexem == "-" || lexem == "*" || lexem == "/" || lexem == "%") {
            auto rightOperand = st.top(); st.pop();
            auto leftOperand = st.top(); st.pop();

            if (lexem == "+") {
                st.push(apply_binary_operation(leftOperand, rightOperand, std::plus<>()));
            }
            else if (lexem == "-") {
                st.push(apply_binary_operation(leftOperand, rightOperand, std::minus<>()));
            }
            else if (lexem == "*") {
                st.push(apply_binary_operation(leftOperand, rightOperand, std::multiplies<>()));
            }
            else if (lexem == "/") {
                if (std::visit([](auto val) { return val == 0; }, rightOperand)) {
                    throw std::runtime_error("Division by zero");
                }
                st.push(apply_binary_operation(leftOperand, rightOperand, std::divides<>()));
            }
            else if (lexem == "%") {
                if (std::visit([](auto val) { return val == 0; }, rightOperand)) {
                    throw std::runtime_error("Division by zero");
                }
                st.push(apply_binary_operation(leftOperand, rightOperand, std::modulus<long long>()));
            }
        }
        else if (IsFunction(lexem)) {
            auto operand = st.top(); st.pop();

            if (lexem == "exp") {
                st.push(apply_unary_operation(operand, [](auto x) { return std::exp(x); }));
            }
            else if (lexem == "sin") {
                st.push(apply_unary_operation(operand, [](auto x) { return std::sin(x); }));
            }
            else if (lexem == "cos") {
                st.push(apply_unary_operation(operand, [](auto x) { return std::cos(x); }));
            }
            else if (lexem == "lg") {
                st.push(apply_unary_operation(operand, [](auto x) { return std::log10(x); }));
            }
            else if (lexem == "log") {
                double degree;
                std::cout << "Enter degree:";
                std::cin >> degree;
                if (degree <= 0 || degree == 1 || std::get<double>(operand) <= 0) {
                    throw std::domain_error("Logarithm base must be greater than 0 and not equal to 1, and operand must be positive");
                }
                st.push(apply_unary_operation(operand, [degree](auto x) { return std::log(x) / std::log(degree); }));
            }
            else if (lexem == "sqrt") {
                st.push(apply_unary_operation(operand, [](auto x) { return std::sqrt(x); }));
            }
        }

        else {
            st.push(operands.at(lexem));
        }
    }

    return st.top();
}

std::variant<long long, double> ConvertToVariant(const std::string& str) {
    if (str.find('.') != std::string::npos) {
        try {
            return std::stod(str);
        }
        catch (const std::exception&) {
            throw std::invalid_argument("Invalid input: not a valid number.");
        }
    }
    else {
        try {
            return std::stoll(str);
        }
        catch (const std::exception&) {
            throw std::invalid_argument("Invalid input: not a valid integer.");
        }
    }
}

#endif // _CACLULATOR_H
