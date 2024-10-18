#include <iostream>
#include "arithmetic.h"

using namespace std;

int main() {
	try {
		string str;
		cout << "Enter expression:";
		getline(cin, str);
		TArithmeticExpression example(str);
		cout << "Postfix:" << example.toPostfix() << endl;;
		example.setValues();
		cout << "Result:" << example.calculate() << endl;
	}
	catch (char* exp) {
		cout << exp << endl;
	}
	return 0;
}