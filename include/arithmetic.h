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
	map<string, long long> operands; 
private:
	void parse();
	bool isConstant(const string& str) const; 
	bool isOperator(char c) const;
	bool isParenthesis(char c) const;
	bool isDigitOrLetter(char c) const;
	double calculate(const map<string, long long>& values);
	void removeSpaces(string& str) const;
	bool isCorrectInfixExpression();
public:
	TArithmeticExpression(const string& infix); 
	string toPostfix();
	void setValues(); 
	double calculate(); 
};

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

bool TArithmeticExpression::isConstant(const string& s) const {
	for (char c : s) {
		if (!isdigit(c) && c != '.') {
			return false;
		}
	}
	return true;
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

void TArithmeticExpression::setValues() {
	long long  value;
	for (auto& op : operands) {
		if (!isConstant(op.first)) {
			cout << "Enter value of " << op.first << ":";
			cin >> value;
			operands[op.first] = value;
		}
	}
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
			long long  value = isConstant(item) ? stod(item) : 0.0;
			operands.insert({ item, value });
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

double TArithmeticExpression::calculate(const map<string, long long >& values){
	for (auto& val : values) {
		try {
			operands.at(val.first) = val.second;
		}
		catch (out_of_range& e) {}
	}
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
			st.push(operands[lexem]);
		}
	}
	return st.top();
}

double TArithmeticExpression::calculate() {
	return calculate(operands);
}

bool TArithmeticExpression::isCorrectInfixExpression(){
	int count = 0;
	for (char c : infix)
	{
		if (c == '(') count++;
		else if (c == ')') count--;
		if (count < 0) return false;
	}
	return (count == 0);
}

#endif 