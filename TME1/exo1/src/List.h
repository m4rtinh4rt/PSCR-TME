#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#include <cstddef>
#include <ostream>
#include <string>

namespace pr {

class Chainon {
 public:
  std::string data;
  Chainon* next;
  Chainon(const std::string& data, Chainon* next = nullptr);
  size_t length();
  void print(std::ostream& os) const;
};

class List {
 public:
  Chainon* tete;

  List() : tete(nullptr) {}

  ~List() {
    for (Chainon* c = tete; c;) {
      Chainon* tmp = c->next;
      delete c;
      c = tmp;
    }
  }

  const std::string& operator[](size_t index) const;

  void push_back(const std::string& val);

  // FAUTE : push_front est définit deux fois.
  void push_front(const std::string& val);

  bool empty();

  size_t size() const;
};

std::ostream& operator<<(std::ostream& os, const List& vec);

} /* namespace pr */

#endif /* SRC_LIST_H_ */
