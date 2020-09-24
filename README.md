# BigInt-cpp
Hahah C++ is BETTER and FASTER than python

Supports addition, subtraction, multiplication for arbitary integers

Exponentiation using `a^b`, where b < 9223372036854775808

## Example
```c++
#include "BigInt.h"
using namespace std;

int main() {
  BigInt num1 = "65190835613095830518236529081543672985102491238905694301573290384612861302893946";
  BigInt num2 = -01189998819991197253LL;
  cout << "Num2 is : " << num2.toLL() << "\n";
  
  cout << num1 + num2 << "\n";
  cout << num1 * num2 << "\n";
  cout << num1 - num2 << "\n";
  cout << num1 ^ num2 << "\n";
}
