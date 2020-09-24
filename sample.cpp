#include "BigInt.h"
using namespace std;

int main() {
  BigInt num1 = (string)"01189998819991197253";
  BigInt num2 = 21;
  cout << "Num2 is : " << num2.toLL() << "\n";
  
  cout << num1 + num2 << "\n";
  cout << num1 * num2 << "\n";
  cout << num1 - num2 << "\n";
  cout << (num1 ^ num2.toLL()) << "\n";
}