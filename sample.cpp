#include <iostream>
#include <string>
#include "BigInt.hpp"
using namespace std;

int main() {
	BigInt num1 = (string)"01189998819991197253";
	BigInt num2;

	cin >> num2;
	cout << "Num2 is : " << num2 << "\n";
	cout << num1 + num2 << "\n";
	cout << num1 * num2 << "\n";
	cout << num1 - num2 << "\n";
	cout << num1.pow(num2) << "\n";
}