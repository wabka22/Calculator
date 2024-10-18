#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
using namespace std;

class TArithmeticExpression {
private:
	string infix; 
	vector<string> postfix;
	vector<string> lexems;
	static map<string, int> operator_priority; 
private:
	void parse();
	bool isOperator(char c) const;
	bool isParenthesis(char c) const;
	bool isDigitOrLetter(char c) const;
	void removeSpaces(string& str) const;
	bool isCorrectInfixExpression();
public:
	TArithmeticExpression(const string& infix); 
	string toPostfix();
	double calculate(); 
};

bool TArithmeticExpression::isCorrectInfixExpression() {
	int count = 0;
	for (char c : infix)
	{
		if (c == '(') count++;
		else if (c == ')') count--;
		if (count < 0) return false;
	}
	return (count == 0);
}

TArithmeticExpression::TArithmeticExpression(const string& _infix) {
	if (_infix.empty()) {
		throw("expression is empty");
	}
	infix = _infix;
	removeSpaces(infix);
	if (!(isCorrectInfixExpression())) {
		throw("non-correct number of parentheses");
	}
}

map<string, int> TArithmeticExpression::operator_priority = {
	{"*", 3},
	{"%", 3},
	{"/", 3},
	{"+", 2},
	{"-", 2},
	{"(", 1},
	{")", 1}
};

void TArithmeticExpression::removeSpaces(string& str) const {
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

bool TArithmeticExpression::isOperator(char c) const {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool TArithmeticExpression::isParenthesis(char c) const {
	return (c == '(' || c == ')');
}

bool TArithmeticExpression::isDigitOrLetter(char c) const {
	return (isdigit(c) || c == '.' || isalpha(c));
}

void TArithmeticExpression::parse(){
	string currentElement;
	for (char c : infix) {
		if (isOperator(c) || isParenthesis(c) || c == ' ') {
			if (!currentElement.empty()) {
				lexems.push_back(currentElement);
				currentElement = "";
			}
			lexems.push_back(string(1, c));
		}
		else if (isDigitOrLetter(c)) {
			currentElement += c;
		}
	}
	if (!currentElement.empty()) {
		lexems.push_back(currentElement);
	}
}

string TArithmeticExpression::toPostfix() {
	parse();
	stack<string> st;
	string postfixExpression;
	for (string item : lexems) {
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
		else if (isOperator(item[0])) {
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

double TArithmeticExpression::calculate(){
	stack<double> st;
	long long leftOperand, rightOperand;
	for (string lexem : postfix) {
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
			if (rightOperand == 0) { throw ("Division by zero"); }
			st.push(static_cast<double>(leftOperand) / static_cast<double>(rightOperand));
		}
		else if (lexem == "%") {
			rightOperand = st.top(); st.pop();
			leftOperand = st.top(); st.pop();
			if (rightOperand == 0) { throw ("Division by zero"); }
			   st.push(static_cast<long long>(leftOperand) % static_cast<long long>(rightOperand));
		}
		else {
			st.push(stod(lexem));
		}
	}
	return st.top();
}
#endif 