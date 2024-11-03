#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H
#include <iostream>
#include <stdexcept>
#include <algorithm> 
#include <cmath> 
#include <string>
#include <map>
#include <vector>
#include <stack>

class Calculator {
private:
	std::string infix_;
	std::vector<std::string> postfix_;
	std::vector<std::string> lexems_;
	std::map<std::string, double> operands;
	static std::map<std::string, int> operator_priority;

private:
	void Parse() ;
	std::string ToPostfix() ;
	bool IsConst(const std::string& st) const noexcept;
	bool IsOperator(const char c) const noexcept;
	bool IsDigitOrLetter(char c) const noexcept;
	bool IsParenthesis(const char c) const noexcept;
	bool IsFunction(const std::string& str) const noexcept;
	double Calculate(const std::map<std::string, double>& values);
	void RemoveSpaces(std::string& str) const noexcept;
	bool IsCorrectExpression(const std::string str) const noexcept;
	void SetValues() noexcept;
public:
	Calculator(const std::string& infix_);
	double Calculate();
};

bool Calculator::IsCorrectExpression(const std::string str) const noexcept
{
	int count = 0;
	for (char c : str)
	{
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

bool Calculator::IsOperator(char c) const  noexcept {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool Calculator::IsParenthesis(char c) const noexcept {
	return (c == '(' || c == ')');
}

bool Calculator::IsFunction(const std::string& s) const noexcept {
	return (s == "exp" || s == "sin" || s == "cos" || s == "lg" || s == "sqrt");
}

bool Calculator::IsConst(const std::string& s) const noexcept {
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

void Calculator::SetValues() noexcept {
	double value;
	for (auto& op : operands) {
		if (!IsConst(op.first)) {
			std::cout << "Enter value of " << op.first << ":";
			std::cin >> value;
			operands[op.first] = value;
		}
	}
}

void Calculator::Parse(){
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

double Calculator::Calculate(const std::map< std::string, double>& values) {
	SetValues();

	for (auto& val : values) {
		try {
			operands.at(val.first) = val.second;
		}
		catch (std::out_of_range& e) {}
	}
	std::stack<double> st;
	double leftOperand, rightOperand;

	for (const std::string& lexem : postfix_) {
		if (lexem == "+") {
			rightOperand = st.top(); st.pop();
			leftOperand = st.top(); st.pop();
			st.push(leftOperand + rightOperand);
		}
		else if (lexem == "-") {
			rightOperand = st.top(); st.pop();
			leftOperand = st.top(); st.pop();
			st.push(leftOperand - rightOperand);
		}
		else if (lexem == "*") {
			rightOperand = st.top(); st.pop();
			leftOperand = st.top(); st.pop();
			st.push(leftOperand * rightOperand);
		}
		else if (lexem == "/") {
			rightOperand = st.top(); st.pop();
			leftOperand = st.top(); st.pop();
			if (rightOperand == 0) {
				throw std::runtime_error("Division by zero");
			}
			st.push(leftOperand / rightOperand);
		}
		else if (lexem == "%") {
			rightOperand = st.top(); st.pop();
			leftOperand = st.top(); st.pop();
			if (static_cast<long long>(leftOperand) != leftOperand ||
				static_cast<long long>(rightOperand) != rightOperand) {
				throw std::invalid_argument("This operation is not defined for this type of numbers");
			}
			if (rightOperand == 0) {
				throw std::runtime_error("Division by zero");
			}
			st.push(static_cast<long long>(leftOperand) % static_cast<long long>(rightOperand));
		}
		else if (IsFunction(lexem)) {
			double operand = st.top(); st.pop();
			if (lexem == "exp") {
				st.push(std::exp(operand));
			}
			else if (lexem == "sin") {
				st.push(std::sin(operand));
			}
			else if (lexem == "cos") {
				st.push(std::cos(operand));
			}
			else if (lexem == "lg") {
				if (operand <= 0) {
					throw std::domain_error("Logarithm of non-positive number");
				}
				st.push(std::log10(operand));
			}
			else if (lexem == "sqrt") {
				if (operand < 0) {
					throw std::domain_error("Square root of negative number");
				}
				st.push(std::sqrt(operand));
			}
		}
		else {
			st.push(operands[lexem]);
		}
	}

	return st.top();
}

double Calculator::Calculate() {
	return Calculate(operands);
}

#endif 