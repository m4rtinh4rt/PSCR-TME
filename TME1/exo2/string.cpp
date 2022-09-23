#include "string.hpp"

#include <cstddef>
#include <cstring>

namespace pr {
std::ostream& operator<<(std::ostream& out, const String& s) {
  out << s.str;
  return out;
}

bool operator==(const String& a, const String& b) {
  return pr::strcmp(a.str, b.str) == 0;
}

bool operator!=(const String& a, const String& b) { return !(a == b); }

bool operator<(const String& a, const String& b) {
  return pr::strcmp(a.str, b.str) < 0;
}

bool operator>(const String& a, const String& b) {
  return pr::strcmp(a.str, b.str) > 0;
}

int strcmp(const char* a, const char* b) {
  const char* c1 = a;
  const char* c2 = b;

  while (*c1 && *c2 && *c1++ == *c2++) continue;

  return *c1 - *c2;
}

size_t length(const char* s) {
  const char* ptr = s;

  while (*ptr) ptr++;

  return ptr - s;
}

char* newcopy(const char* s) {
  int len = length(s);

  if (!len) return NULL;

  char* str = new char[len + 1];
  memcpy(str, s, len + 1);

  return str;
}

size_t String::length() const { return pr::length(this->str); }

String::String(const char* s) : str(newcopy(s)) {}

String::String(const String& s) : str(newcopy(s.str)) {}

String::~String() { delete[] this->str; }
}  // namespace pr
