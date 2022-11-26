#pragma once

#include <fcntl.h>
#include <semaphore.h>

#include <cstring>  // size_t,memset
#include <iostream>

namespace pr {

#define STACKSIZE 100

template <typename T>
class Stack {
  T tab[STACKSIZE];
  size_t sz;
  sem_t s_in, s_out, s_m;

 public:
  Stack() : sz(0) {
    memset(tab, 0, sizeof tab);
    if (sem_init(&s_in, 1, STACKSIZE) < 0) {
      perror("sem_init");
      exit(1);
    }
    if (sem_init(&s_out, 1, 0) < 0) {
      perror("sem_init");
      exit(1);
    }
    if (sem_init(&s_m, 1, 1) < 0) {
      perror("sem_init");
      exit(1);
    }
  }

  T pop() {
    // bloquer si vide
    sem_wait(&s_out);
    sem_wait(&s_m);
    T toret = tab[--sz];
    sem_post(&s_in);
    sem_post(&s_m);
    return toret;
  }

  void push(T elt) {
    // bloquer si plein
    sem_wait(&s_in);
    sem_wait(&s_m);
    tab[sz++] = elt;
    sem_post(&s_out);
    sem_post(&s_m);
  }

  ~Stack() {
    sem_destroy(&s_in);
    sem_destroy(&s_out);
    sem_destroy(&s_m);
  }
};

}  // namespace pr
