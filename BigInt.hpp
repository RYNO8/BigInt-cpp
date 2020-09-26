#include <string>
#include <vector>
#include <limits>
typedef int i31;
typedef unsigned int i32;
typedef long long i63;
typedef unsigned long long i64;

// optimised for time, not space
// the upper is 2**M64 or 2**(2**64-1) or _____
// realistically you will run out of memory first, lol
class BigInt {
public:
	bool isPos = true;
	std::vector<i63> chunks;
	

	BigInt();
	BigInt(i31);
	BigInt(i32);
	BigInt(i63);
	BigInt(i64);
	BigInt(std::vector<i63>, bool);
	BigInt(std::string);
	BigInt(std::vector<bool>, bool);
	BigInt(const BigInt& cp);


	bool operator==(BigInt o);
	template<typename T> bool operator==(T o) { return *this == BigInt(o); };
	bool operator!=(BigInt o);
	template<typename T> bool operator!=(T o) { return *this != BigInt(o); };
	bool operator>(BigInt o);
	template<typename T> bool operator>(T o) { return *this > BigInt(o); };
	bool operator>=(BigInt o);
	template<typename T> bool operator>=(T o) { return *this >= BigInt(o); };
	bool operator<(BigInt o);
	template<typename T> bool operator<(T o) { return *this < BigInt(o); };
	bool operator<=(BigInt o);
	template<typename T> bool operator<=(T o) { return *this <= BigInt(o); };


	BigInt operator+(BigInt o);
	template<typename T> BigInt operator+(T o) { return *this + BigInt(o); };
	template<typename T> void operator++(T o) { *this = *this + 1; };
	template<typename T> void operator+=(T o) { *this = *this + BigInt(o); };


	BigInt operator-();
	BigInt operator-(BigInt o);
	template<typename T> BigInt operator-(T o) { return *this - BigInt(o); };
	template<typename T> void operator--(T o) { *this = *this - 1; };
	template<typename T> void operator-=(T o) { *this = *this - BigInt(o); };


	BigInt operator*(BigInt o);
	template<typename T> BigInt operator*(T o) { return *this * BigInt(o); };
	template<typename T> void operator*=(T o) { *this = *this * BigInt(o); };


	bool isEven();
	BigInt pow(i64 o);
	BigInt pow(BigInt o);
	template<typename T> BigInt pow(T o) {
		if (o <= std::numeric_limits<i64>::max()) return pow(i64(o));
		return this->pow(BigInt(o));
	};
	BigInt operator<<(BigInt o);
	template<typename T> BigInt operator<<(T o) { return *this << BigInt(o); };
	template<typename T> void operator<<=(T o) { *this = *this << o; };
	BigInt operator>>(BigInt o);
	template<typename T> BigInt operator>>(T o) { return *this >> BigInt(o) ; };
	template<typename T> void operator>>=(T o) { *this = *this >> o; };


	BigInt abs();
	i63 toLL();
	bool toBool();
	std::vector<bool> toBin();
	BigInt operator|(BigInt o);
	template<typename T> BigInt operator|(T o) { return *this | BigInt(o); };
	template<typename T> void operator|=(T o) { *this = *this | BigInt(o); };
	BigInt operator^(BigInt o);
	template<typename T> BigInt operator^(T o) { return *this ^ BigInt(o); };
	template<typename T> void operator^=(T o) { *this = *this ^ BigInt(o); };
	BigInt operator&(BigInt o);
	template<typename T> BigInt operator&(T o) { return *this & BigInt(o); };
	template<typename T> void operator&=(T o) { *this = *this & BigInt(o); };
};

std::ostream& operator<<(std::ostream& out, BigInt obj);
std::istream& operator>>(std::istream& in, BigInt &obj);