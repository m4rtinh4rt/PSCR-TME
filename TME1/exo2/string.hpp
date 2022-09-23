#pragma once

#include <cstddef>
#include <ostream>

namespace pr {
size_t length(const char* s);
int strcmp(const char* a, const char* b);

class String {
  char* str;
  friend std::ostream& operator<<(std::ostream& out, const String& s);
  friend bool operator==(const String& a, const String& b);
  friend bool operator!=(const String& a, const String& b);
  friend bool operator<(const String& a, const String& b);
  friend bool operator>(const String& a, const String& b);

 public:
  size_t length() const;
  String(const char* str);
  String(const String& s);
  ~String();
};
}  // namespace pr
