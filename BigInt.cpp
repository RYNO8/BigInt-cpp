#include <vector>
#include <limits>
#include <string>
#include <math.h>
#include <bitset>
#include <algorithm>
#include <iostream>
#include "BigInt.hpp"

using namespace std;
typedef int i31;
typedef unsigned int i32;
typedef long long i63;
typedef unsigned long long i64;

constexpr i31 M31 = numeric_limits<i31>::max();
constexpr i32 M32 = numeric_limits<i32>::max();
constexpr i63 M63 = numeric_limits<i63>::max();
constexpr i64 M64 = numeric_limits<i64>::max();
constexpr i63 SIZE = 1e9;
constexpr unsigned char guess[33] = {
	0, 0, 0, 0, 1, 1, 1, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 4, 5, 5, 5,
	6, 6, 6, 6, 7, 7, 7, 8, 8, 8,
	9, 9, 9
};
constexpr unsigned int tenToThe[] = {
	1, 10, 100, 1000, 10000, 100000,
	1000000, 10000000, 100000000, 1000000000,
};
static string xZeros[] = { // constexpr doesnt work with strings?
	"", "0", "00", "000", "0000", "00000", "000000",
	"0000000", "00000000", "00000000", "000000000"
};

i32 baseTwoDigits(i32 x) {
	return x ? 32 - __builtin_clz(x) : 0;
}

static i32 baseTenDigits(i32 x) {
	if (!x) return 1; // there is 1 digit in '0'
	i32 digits = guess[baseTwoDigits(x)];
	return digits + (x >= tenToThe[digits]);
}

i64 compare(vector<i63>& a, vector<i63>& b) {
	//(a > b) == 1, (a < b) == 0, (a == b) == 2
	if (a.size() != b.size()) return a.size() > b.size();
	for (i63 i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) return a[i] > b[i];
	}
	return 2; // they are equal
}

vector<i63> flatten(vector<i63> &chunks) {
	// ensures that max(chunks) < SIZE
	chunks.push_back(0);
	for (i63 i = 0; i < chunks.size(); ++i) {
		chunks[i + 1] += chunks[i] / SIZE;
		chunks[i] %= SIZE;
	}
	while (chunks.size() > 1 && chunks.back() == 0) chunks.pop_back();
	return chunks;
}

bool isZero(vector<i63>& a) {
	return a.size() == 1 && !a[0];
}

BigInt::BigInt() {
	chunks = { 0 };
	isPos = true;
}
BigInt::BigInt(i31 val) {
	isPos = (val >= 0);
	if (!isPos) val = -val;
	chunks = { val };
}
BigInt::BigInt(i32 val) {
	isPos = (val >= 0);
	if (!isPos) val = -val;
	chunks = { val };
}
BigInt::BigInt(i63 val) {
	isPos = (val >= 0);
	if (!isPos) val = -val;
	chunks = { val % SIZE };
	if (val >= SIZE) chunks.push_back(val / SIZE);
}
BigInt::BigInt(i64 val) {
	isPos = (val >= 0);
	if (!isPos) val = -val;
	chunks = { (i63)val % SIZE };
	if (val >= SIZE) chunks.push_back(val / SIZE);
}
BigInt::BigInt(vector<i63> _chuncks, bool _isPos) {
	isPos = _isPos;
	chunks = _chuncks;
}
BigInt::BigInt(vector<bool> val, bool _isPos) {
	*this = 0;
	BigInt curr = 1;
	for (bool bit : val) {
		if (bit) *this += curr;
		curr *= 2;
	}
	isPos = _isPos;
}
BigInt::BigInt(string val) {
	i63 s = 0;
	isPos = true;
	if (val[0] == '-') isPos = false, ++s;
	while (val[s] == '0') ++s;
	if (s == val.size()) {
		isPos = true;
		chunks = { 0 };
		return;
	}
	for (i63 i = val.size() - 1; i >= s;) {
		i63 chunk = 0;
		for (i63 rep = 0; rep < 9 && i >= s; ++rep, --i) chunk += tenToThe[rep] * (val[i] - '0');
		chunks.push_back(chunk);
	}
}
BigInt::BigInt(const BigInt& cp) : chunks(cp.chunks), isPos(cp.isPos) {}; // thanks Max!

bool BigInt::operator==(BigInt o) {
	// O(logn b=SIZE)
	if (!toBool() || !o.toBool()) return toBool() == o.toBool(); // shortcut?
	return isPos == o.isPos && chunks.size() == o.chunks.size() && chunks == o.chunks;
}
bool BigInt::operator!=(BigInt o) {
	// O(logn b=SIZE)
	return !(this->operator==(o));
}
bool BigInt::operator>(BigInt o) {
	// O(logn b=SIZE)
	if (isPos != o.isPos) return (isPos > o.isPos);

	// bigbrain move, equivalent to
	/*if (isPos) return compare(chunks, o.chunks) == 1;
	if (!isPos) return compare(chunks, o.chunks) == 0;*/
	return isPos == compare(chunks, o.chunks);
}
bool BigInt::operator>=(BigInt o) {
	// O(logn b=SIZE)
	return !this->operator<(o);
}
bool BigInt::operator<(BigInt o) {
	// O(logn b=SIZE)

	// bigbrain move, equivalent to
	/*if (isPos) return compare(chunks, o.chunks) == 0;
	if (!isPos) return compare(chunks, o.chunks) == 1;*/
	return !isPos == compare(chunks, o.chunks);
}
bool BigInt::operator<=(BigInt o) {
	// O(logn b=SIZE)
	return !this->operator>(o);
}

BigInt BigInt::operator+(BigInt o) {
	// O(logn b=SIZE)
	if (isPos && !o.isPos) return this->operator-(-o); // (*this) - (-o);
	if (!isPos && o.isPos) return o.operator-(this->operator-()); // o - -(*this);

	vector<i63> output;
	i63 carry = 0;
	for (i63 i = 0; i < max(chunks.size(), o.chunks.size()); ++i) {
		i63 chunk = carry + (i < chunks.size() ? chunks[i] : 0) + (i < o.chunks.size() ? o.chunks[i] : 0);
		output.push_back(chunk % SIZE);
		carry = chunk / SIZE;
	}
	if (carry) output.push_back(carry);
	return { output, isPos };
}


BigInt BigInt::operator-() {
	// negate, O(1)
	return { chunks, !isPos || isZero(chunks) };
}
BigInt BigInt::operator-(BigInt o) {
	// O(2logn b=SIZE)
	if (isPos && !o.isPos) return *this + (-o);
	if (!isPos && o.isPos) return o + -(*this);
	if (isPos && *this < o) return -(o - *this);
	if (!isPos) return -o - -(*this);

	// assume both positive, this > 0
	if (!isPos || !o.isPos || *this < o) throw 20;
	vector<i63> output;
	bool carry = false;
	for (i63 i = 0; i < max(chunks.size(), o.chunks.size()); ++i) {
		i63 chunk = (i < chunks.size() ? chunks[i] : 0) - (i < o.chunks.size() ? o.chunks[i] : 0) - carry;
		carry = (chunk < 0);
		output.push_back(chunk + carry * SIZE);
	}
	return { output, true };
}

BigInt BigInt::operator*(BigInt o) {
	// O(log^2 n b=SIZE)
	vector<i63> output(chunks.size() + o.chunks.size(), 0);
	for (i63 i = 0; i < chunks.size(); ++i) {
		for (i63 j = 0; j < o.chunks.size(); ++j) {
			output[i + j] += chunks[i] * o.chunks[j];
		}
	}
	return { flatten(output), !(isPos ^ o.isPos) || isZero(output) };
}

bool BigInt::isEven() {
	return chunks[0] % 2 == 0;
}
BigInt BigInt::pow(i64 o) {
	// O(logo b=2)
	if (o < 0) throw 20;
	BigInt curr = *this, total = 1;
	for (i64 p = 1; p <= o; p *= 2) {
		if (p & o) total *= curr;
		curr *= curr;
	}
	total.isPos = isPos || (o % 2) == 0;
	return total;
}
BigInt BigInt::pow(BigInt o) {
	// O(logo b=2)
	if (o < 0) throw 20;
	if (o <= SIZE * SIZE) return pow(o.toLL());
	
	BigInt curr = *this, total = 1;
	bool totalIsPos = isPos || o.isEven();
	for (; o != 0; o = o >> 1) {
		if (!o.isEven()) total *= curr;
		curr *= curr;
	}
	total.isPos = totalIsPos;
	return total;
}
BigInt BigInt::operator<<(BigInt o) {
	// O(logo b=2)
	return *this * BigInt(2).pow(o);
}
BigInt BigInt::operator>>(BigInt o) {
	// O(o logn b=SIZE) whoops
	if (o <= 0) {
		if (o == 0) return *this;
		else throw 20;
	}

	vector<i63> output = chunks; // hopefully makes a copy
	for (; o != 0; o--) {
		for (i63 i = output.size() - 1; i > 0; --i) {
			if (output[i] % 2) output[i - 1] += SIZE / 2;
			output[i] /= 2;
		}
		output[0] /= 2;
		if (output.size() > 1 && output.back() == 0) output.pop_back();
	}
	return { output, isPos };
}

BigInt BigInt::abs() {
	return { chunks, true };
}
i63 BigInt::toLL() {
	// if (chunks.size() >= 2 || *this >= SIZE * SIZE) cout << "WARNING: narrowing conversion of 'BigInt' to 'i63'";
	i63 total = chunks[0] + (chunks.size() > 1 ? SIZE * chunks[1] : 0);
	return (isPos ? total : -total);
}
bool BigInt::toBool() {
	return !isZero(chunks);
}
vector<bool> BigInt::toBin() {
	// O(logn b=2) vector<bool> instead of bitset for dynamic size
	// returns bin(abs(this))
	if (!this->toBool()) return { 0 };
	vector<bool> output;
	for (BigInt cp = this->abs(); cp != 0; cp = cp >> 1) output.push_back(!cp.isEven());
	return output;
}
BigInt BigInt::operator|(BigInt o) {
	// O(logn b=2)
	vector<bool> currBin = toBin(), oBin = o.toBin();
	currBin.resize(max(currBin.size(), oBin.size()));
	bool isZero = true;
	for (i64 i = 0; i < oBin.size(); ++i) isZero &= (currBin[i] = currBin[i] | oBin[i]) == 0;
	return { currBin, !(isPos ^ o.isPos) || isZero };
}
BigInt BigInt::operator^(BigInt o) {
	// O(logn b=2)
	vector<bool> currBin = toBin(), oBin = o.toBin();
	currBin.resize(max(currBin.size(), oBin.size()));
	bool isZero = true;
	for (i64 i = 0; i < oBin.size(); ++i) isZero &= (currBin[i] = currBin[i] ^ oBin[i]) == 0;
	return { currBin, !(isPos ^ o.isPos) || isZero };
}
BigInt BigInt::operator&(BigInt o) {
	// O(logn b=2)
	vector<bool> currBin = toBin(), oBin = o.toBin();
	currBin.resize(max(currBin.size(), oBin.size()));
	bool isZero = true;
	for (i64 i = 0; i < oBin.size(); ++i) isZero &= (currBin[i] = currBin[i] & oBin[i]) == 0;
	return { currBin, !(isPos ^ o.isPos) || isZero };
}


// I/O
ostream& operator<<(ostream& out, BigInt obj) {
	if (!obj.isPos) out << '-';
	out << obj.chunks.back();
	for (i63 i = obj.chunks.size() - 2; i >= 0; --i) out << xZeros[9 - baseTenDigits(obj.chunks[i])] << obj.chunks[i];
	return out;
}
istream& operator>>(istream& in, BigInt &obj) {
	string val;
	in >> val;
	obj = BigInt(val);
	return in;
}