#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <string>
#include <math.h>

using namespace std;
typedef int i31;
typedef unsigned int i32;
typedef long long i63;
typedef unsigned long long i64;

constexpr i64 M31 = numeric_limits<i31>::max();
constexpr i64 M32 = numeric_limits<i32>::max();
constexpr i64 M63 = numeric_limits<i63>::max();
constexpr i64 M64 = numeric_limits<i64>::max();
constexpr i64 SIZE = 1e9;
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

unsigned int baseTwoDigits(unsigned int x) {
	return x ? 32 - __builtin_clz(x) : 0;
}

static unsigned int baseTenDigits(unsigned int x) {
	if (!x) return 1; // there is 1 digit in '0'
	unsigned int digits = guess[baseTwoDigits(x)];
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

vector<i63> flatten(vector<i63>& chunks) {
	// ensure max(chunks) < SIZE
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

// optimised for time, not space
class BigInt {
public:
	BigInt(i31 val) {
		chunks = { val };
		isPos = (val >= 0);
	}
	BigInt(i32 val) {
		chunks = { val };
		isPos = (val >= 0);
	}
	BigInt(i63 val) {
		chunks = { val % SIZE, val / SIZE };
		isPos = (val >= 0);
	}
	BigInt(i64 val) {
		chunks = { val % SIZE, val / SIZE };
		isPos = (val >= 0);
	}
	BigInt(vector<i63> _chuncks, bool _isPos = true) {
		chunks = _chuncks;
		isPos = _isPos;
	}
	BigInt(string val) {
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

	BigInt operator~() {
		// TODO: bitwise complement
		// whoops non trivial implementation
	}
	bool operator==(BigInt o) {
		// O(logn b=10)
		return (isPos == o.isPos) && chunks == o.chunks;
	}
	bool operator!=(BigInt o) {
		// O(logn b=10)
		return !(this->operator==(o));
	}
	bool operator>(BigInt o) {
		// O(logn b=10)
		if (isPos != o.isPos) return (isPos > o.isPos);

		// bigbrain move, equivalent to
		/*if (isPos) return compare(chunks, o.chunks) == 1;
		if (!isPos) return compare(chunks, o.chunks) == 0;*/
		return isPos == compare(chunks, o.chunks);
	}
	bool operator>=(BigInt o) {
		// O(logn b=10)
		return !this->operator<(o);
	}
	bool operator<(BigInt o) {
		// O(logn b=10)

		// bigbrain move, equivalent to
		/*if (isPos) return compare(chunks, o.chunks) == 0;
		if (!isPos) return compare(chunks, o.chunks) == 1;*/
		return isPos == !compare(chunks, o.chunks);
	}
	bool operator<=(BigInt o) {
		// O(logn b=10)
		return !this->operator>(o);
	}

	BigInt operator+(BigInt o) {
		// O(logn b=SIZE)
		if (isPos && !o.isPos) return (*this) - (-o);
		if (!isPos && o.isPos) return o - -(*this);

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
	BigInt operator-() {
		// negate, O(1) ??
		return { chunks, !isPos };
	}
	BigInt operator-(BigInt o) {
		// O(logn b=SIZE + logn b=10)
		if (isPos && !o.isPos) return *this + (-o);
		if (!isPos && o.isPos) return o + -(*this);
		if (isPos && this->operator<(o)) return -(o - *this);
		if (!isPos) return -o - -(*this);

		// assume both positive, this > 0
		if (!isPos || !o.isPos || (*this) <= o) throw "yeet";
		vector<i63> output;
		bool carry = false;
		for (i63 i = 0; i < max(chunks.size(), o.chunks.size()); ++i) {
			i63 chunk = (i < chunks.size() ? chunks[i] : 0) - (i < o.chunks.size() ? o.chunks[i] : 0) - carry;
			carry = (chunk < 0);
			output.push_back((chunk + SIZE) % SIZE);
		}
		return { output, true };
	}

	BigInt operator*(BigInt o) {
		// O(log^2 n b=SIZE)
		vector<i63> output(chunks.size() + o.chunks.size(), 0);
		for (i63 i = 0; i < chunks.size(); ++i) {
			for (i63 j = 0; j < o.chunks.size(); ++j) {
				output[i + j] += chunks[i] * o.chunks[j];
			}
		}
		return { flatten(output), !(isPos ^ o.isPos) || isZero(output) };
	}

	BigInt operator^(i63 o) { // this is exp, not xor
		// O(logo b=2) there probably is a faster way
		BigInt curr = *this;
		BigInt total = 1;
		for (i63 p = 0; (1 << p) <= o; ++p) {
			if ((1 << p) & o) total = total * curr;
			curr = curr * curr;
		}
		return total;
	}
	BigInt operator<<(i63 o) {
		return (*this) * (BigInt(2) ^ o);
	}
	friend ostream& operator<<(ostream& os, BigInt _) {
		if (!_.isPos) os << '-';
		for (i63 i = _.chunks.size() - 1; i >= 0; --i) {
			if (i != _.chunks.size() - 1) os << xZeros[9 - baseTenDigits(_.chunks[i])];
			os << _.chunks[i];
		}
		return os;
	}
	i63 toLL() {
		i63 total = chunks[0] + (chunks.size() > 1 ? chunks[1] * SIZE : 0);
		return (isPos ? total : -total);
	}
private:
	bool isPos; // 0 is considered positive, deal with it
	vector<i63> chunks;
};