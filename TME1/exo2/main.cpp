#include <iostream>

#include "string.hpp"

#define MYSTRING "Hello, World!"

int main() {
  pr::String str(MYSTRING);
  pr::String newStr(str);

  std::cout << str << " length: " << str.length() << '\n';
  std::cout << newStr << " length: " << str.length() << '\n';

  std::cout << pr::strcmp(MYSTRING, MYSTRING) << '\n';
  std::cout << (str == newStr ? "Equal" : "Not equal") << '\n';
  std::cout << (str != newStr ? "Not equal" : "Equal") << '\n';
  std::cout << (str < newStr ? "smaller" : "not smaller") << '\n';
  std::cout << (str > newStr ? "bigger" : "not bigger") << std::endl;

  return 0;
}
