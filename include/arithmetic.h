#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>

class TArithmeticExpression {
private:
	std::string infix_;
	std::vector<std::string> postfix_;
	std::vector<std::string> lexems_;
	static std::map<std::string, int> operator_priority;

private:
	void Parse();
	std::string ToPostfix();
	bool IsOperator(const char c) const noexcept;
	bool IsParenthesis(const char c) const noexcept;
	bool IsFunction(const std::string& str) const noexcept;
	void RemoveSpaces(std::string& str) const noexcept;
	bool IsCorrectInfixExpression(const std::string str) const noexcept;

public:
	TArithmeticExpression(const std::string& infix_);
	double Calculate() const;
};

bool TArithmeticExpression::IsCorrectInfixExpression(const std::string str) const noexcept
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
	if (!IsCorrectInfixExpression(infix_)) {
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

void TArithmeticExpression::Parse()
{
	std::string currentElement;
	for (size_t i = 0; i < infix_.size(); ++i) {
		char c = infix_[i];

		if (IsParenthesis(c) || IsOperator(c)) {
			if (!currentElement.empty()) {
				if (IsFunction(currentElement)) {
					lexems_.push_back(currentElement);
				}
				else if (isalpha(currentElement[0])) {
					throw std::invalid_argument("Unknown function: " + currentElement);
				}
				else {
					lexems_.push_back(currentElement);
				}
				currentElement.clear();
			}
			lexems_.push_back(std::string(1, c));
		}
		else if (isdigit(c) || c == '.' || isalpha(c)) {
			currentElement += c;
		}
	}

	if (!currentElement.empty()) {
		if (IsFunction(currentElement)) {
			lexems_.push_back(currentElement);
		}
		else if (isalpha(currentElement[0])) {
			throw std::invalid_argument("Unknown function: " + currentElement);
		}
		else {
			lexems_.push_back(currentElement);
		}
	}
}

std::string TArithmeticExpression::ToPostfix() {
	Parse();
	std::stack<std::string> st;
	std::string postfixExpression;
	for (std::string item : lexems_) {
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
		else {
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

double TArithmeticExpression::Calculate() const {
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
			st.push(std::stod(lexem));
		}
	}

	return st.top();
}

#endif 