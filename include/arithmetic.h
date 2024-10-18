#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>

class TArithmeticExpression {
private:
	std::string infix;
	std::vector<std::string>  postfix;
	std::vector<std::string> lexems;
	static std::map<std::string, int> operator_priority;
private:
	void parse();
	bool is_operator(char c) const;
	bool is_parenthesis(char c) const;
	void remove_spaces(std::string& str) const;
	bool is_correct_infix_expression() const;
public:
	TArithmeticExpression(const std::string& _infix);
	std::string to_postfix();
	double calculate() const;
};

bool TArithmeticExpression::is_correct_infix_expression() const
{
	int count = 0;
	for (char c : infix)
	{
		if (c == '(') count++;
		else if (c == ')') count--;
		if (count < 0) return false;
	}
	return (count == 0);
}

TArithmeticExpression::TArithmeticExpression(const std::string& _infix) {
	if (_infix.empty()) {
		throw("expression is empty");
	}
	infix = _infix;
	remove_spaces(infix);
	if (!(is_correct_infix_expression())) {
		throw("non-correct number of parentheses");
	}
	to_postfix();
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

void TArithmeticExpression::remove_spaces(std::string& str) const {
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

bool TArithmeticExpression::is_operator(char c) const {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool TArithmeticExpression::is_parenthesis(char c) const {
	return (c == '(' || c == ')');
}

void TArithmeticExpression::parse()
{
	std::string currentElement;
	for (char c : infix) {
		if (is_operator(c) || is_parenthesis(c) || c == ' ') {
			if (!currentElement.empty()) {
				lexems.push_back(currentElement);
				currentElement = "";
			}
			lexems.push_back(std::string(1, c));
		}
		else if (isdigit(c)|| c=='.') {
			currentElement += c;
		}
	}
	if (!currentElement.empty()) {
		lexems.push_back(currentElement);
	}
}

std::string TArithmeticExpression::to_postfix() {
	parse();
	std::stack<std::string> st;
	std::string postfixExpression;
	for (std::string item : lexems) {
		if (item == "(") {
			st.push(item);
		}
		else if (item == ")") {
			while (st.top() != "(") {
				postfixExpression += st.top();
				postfix.push_back(st.top());
				st.pop();
			}
			st.pop();
		}
		else if (is_operator(item[0])) {
			while (!st.empty() && operator_priority[item] <= operator_priority[st.top()]) {
				postfixExpression += st.top();
				postfix.push_back(st.top());
				st.pop();
			}
			st.push(item);
		}
		else {
			postfix.push_back(item);
			postfixExpression += item;
		}
	}
	while (!st.empty()) {
		postfixExpression += st.top();
		postfix.push_back(st.top());
		st.pop();
	}
	return postfixExpression;
}

double TArithmeticExpression::calculate() const {
	std::stack<double> st;
	double leftOperand, rightOperand;

	for (const std::string& lexem : postfix) {
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
				throw ("This operation is only for integer numbers.");
			}
			if (rightOperand == 0) {
				throw ("Division by zero");
			}
			st.push(static_cast<long long>(leftOperand) % static_cast<long long>(rightOperand));
		}
		else {
			st.push(stod(lexem));
		}
	}

	return st.top();
}
#endif 