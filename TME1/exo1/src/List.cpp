#include "List.h" // FAUTE : Oubli d'import du header."

#include <iostream>
#include <string>

namespace pr {

// ******************* Chainon
Chainon::Chainon(const std::string& data, Chainon* next)
    : data(data), next(next){};

size_t Chainon::length() {
  size_t len = 1;
  if (next != nullptr) {
    len += next->length();
  }
  return len;  // FAUTE : Fonction récursive sans fin, ajout d'une condition de
               // sortie.
}

// FAUTE ajout de const
void Chainon::print(std::ostream& os) const {
  os << data;
  if (next != nullptr) {
    os << ", ";
  } else {  // FAUTE : Fonction récursive sans fin, ajout d'une condition de
            // sortie.
    return;
  }
  next->print(os);
}

// ******************  List
const std::string& List::operator[](size_t index) const {
  Chainon* it = tete;
  for (size_t i = 0; i < index; i++) {
    it = it->next;
  }
  return it->data;
}

void List::push_back(const std::string& val) {
  if (tete == nullptr) {
    tete = new Chainon(val);
  } else {
    Chainon* fin = tete;
    while (fin->next) {
      fin = fin->next;
    }
    fin->next = new Chainon(val);
  }
}

void List::push_front(const std::string& val) { tete = new Chainon(val, tete); }

// FAUTE : Oubli du prefix de la class List, rajout de 'List::'
bool List::empty() { return tete == nullptr; }

size_t List::size() const {
  if (tete == nullptr) {
    return 0;
  } else {
    return tete->length();
  }
}

// FAUTE : Fonction en dehors du namespace pr, rajout dans le namespace pr.
std::ostream& operator<<(std::ostream& os, const pr::List& vec) {
  os << "[";
  if (vec.tete != nullptr) {
    vec.tete->print(os);
  }
  os << "]";
  return os;
}

}  // namespace pr
