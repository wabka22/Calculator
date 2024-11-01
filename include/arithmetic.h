#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <stack>

class TArithmeticExpression {
private:
	std::string infix_;
	std::vector<std::string> postfix_;
	std::vector<std::string> lexems_;
	std::map<std::string, double> operands;
	static std::map<std::string, int> operator_priority;

private:
	void Parse() noexcept;
	std::string ToPostfix() noexcept;
	bool IsConst(const std::string& st) const noexcept;
	bool IsOperator(const char c) const noexcept;
	bool TArithmeticExpression::IsDigitOrLetter(char c) const noexcept;
	bool IsParenthesis(const char c) const noexcept;
	bool IsFunction(const std::string& str) const noexcept;
	double Calculate(const std::map<std::string, double>& values);
	void RemoveSpaces(std::string& str) const noexcept;
	bool IsCorrectExpression(const std::string str) const noexcept;

public:
	void SetValues() noexcept;
	TArithmeticExpression(const std::string& infix_);
	double Calculate() noexcept;
};

bool TArithmeticExpression::IsCorrectExpression(const std::string str) const noexcept
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

TArithmeticExpression::TArithmeticExpression(const std::string& _infix) {
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

std::map<std::string, int> TArithmeticExpression::operator_priority = {
	{"*", 3},
	{"%", 3},
	{"/", 3},
	{"+", 2},
	{"-", 2},
	{"(", 1},
	{")", 1}
};

void TArithmeticExpression::RemoveSpaces(std::string& str) const noexcept {
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

bool TArithmeticExpression::IsOperator(char c) const  noexcept {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool TArithmeticExpression::IsParenthesis(char c) const noexcept {
	return (c == '(' || c == ')');
}

bool TArithmeticExpression::IsFunction(const std::string& s) const noexcept {
	return (s == "exp" || s == "sin" || s == "cos" || s == "lg" || s == "sqrt");
}

bool TArithmeticExpression::IsConst(const std::string& s) const noexcept {
	for (char c : s) {
		if (!isdigit(c) && c != '.') {
			return false;
		}
	}
	return true;
}

bool TArithmeticExpression::IsDigitOrLetter(char c) const noexcept {
	return (isdigit(c) || c == '.' || isalpha(c));
}

void TArithmeticExpression::SetValues() noexcept {
	double value;
	for (auto& op : operands) {
		if (!IsConst(op.first)) {
			std::cout << "Enter value of " << op.first << ":";
			std::cin >> value;
			operands[op.first] = value;
		}
	}
}

void TArithmeticExpression::Parse() noexcept
{
	std::string currentElement;
	for (char c : infix_) {
		if (IsOperator(c) || IsParenthesis(c) || c == ' ') {
			if (!currentElement.empty()) {
				lexems_.push_back(currentElement);
				currentElement = "";
			}
			lexems_.push_back(std::string(1, c));
		}
		else if (IsDigitOrLetter(c)) {
			currentElement += c;
		}
	}
	if (!currentElement.empty()) {
		lexems_.push_back(currentElement);
	}
}

std::string TArithmeticExpression::ToPostfix() noexcept {
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

double TArithmeticExpression::Calculate(const std::map< std::string, double>& values) {
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

double TArithmeticExpression::Calculate() noexcept {
	return Calculate(operands);
}

#endif 