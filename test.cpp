#include "BigInt.h"
#include <iostream>
#include <time.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
constexpr long long MOD = 1e18;

int main() {
	//cin.tie(0); cout.tie(0); ios::sync_with_stdio(0);
	ifstream fin{ "test.txt" };
	long long output = 0;
	string num1, num2;
	auto start = chrono::steady_clock::now();

	for (; fin >> num1 >> num2; output ^= ((BigInt(num1) + BigInt(num2)).toLL() + MOD) % MOD);
	while (fin >> num1 >> num2) {
		//cout << num1 << " " << num2 << "\n";
		//cout << (BigInt(num1) + BigInt(num2)) << '\n';
	}
	//cout << (BigInt("7756959221") - BigInt("6719612988")).toLL();

	
	auto end = chrono::steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(end - start);
	cout << '\n' << time_span.count() << " " << output << "\n";
}