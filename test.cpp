#include "BigInt.hpp"
#include <iostream>
#include <time.h>
#include <chrono>
#include <fstream>
#include <algorithm>
using namespace std;
using namespace std::chrono;
constexpr long long MOD = 1e18;

int main() {
	//cin.tie(0); cout.tie(0); ios::sync_with_stdio(0);
	ifstream fin{ "test.txt" };
	long long output = 0;
	string num1, num2;
	auto start = chrono::steady_clock::now();


	// stress test
	/*while (fin >> num1 >> num2) {
		//cout << num1 << " " << num2 << "\n";
		output ^= ((BigInt(num1) .pow (BigInt(num2))).toLL() + MOD) % MOD;
		//cout << BigInt(num1).pow(BigInt(num2)) << endl;
	}*/

	// 1 number test
	/*unsigned long long _;
	cin >> _;
	auto a = BigInt(_);
	cout << _ << "\n" << a;*/

	// 2 number test
	cin >> num1 >> num2;
	auto a = BigInt(num1);
	auto b = BigInt(num2);
	cout << (BigInt(num1) ^ BigInt(num2));
	
	



	
	auto end = chrono::steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(end - start);
	cout << '\n' << time_span.count() << " " << output << endl;
}