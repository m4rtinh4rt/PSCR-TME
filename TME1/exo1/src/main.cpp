#include <cstring>
#include <iostream>
#include <string>

#include "List.h"

int main() {
  std::string abc = "abc";
  char *str = new char[4];
  str[0] = 'a';
  str[1] = 'b';
  str[2] = 'c';
  str[3] = '\0';  // FAUTE : Ajout d'un caractère de fin de chaine.
  int i = 0;      // FAUTE : size_t overflow quand zéro est décrémenté.

  if (!strcmp(str, abc.c_str())) {
    std::cout << "Equal !";
  }

  pr::List list;
  list.push_front(abc);
  list.push_front(abc);

  std::cout << "Liste : " << list << std::endl;
  std::cout << "Taille : " << list.size() << std::endl;

  // Affiche à l'envers
  for (i = list.size() - 1; i >= 0; i--) {
    std::cout << "elt " << i << ": " << list[i] << std::endl;
  }

  // FAUTE : La mémoire dynamique alloué a la chaine de caractère str
  // est free deux fois.
  /*
    // libérer les char de la chaine
    for (char *cp = str ; *cp ; cp++) {
      delete cp;
    }
  */

  // et la chaine elle même
  delete[] str;
}