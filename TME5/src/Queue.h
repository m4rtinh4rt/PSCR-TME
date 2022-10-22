#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <iostream>

namespace pr {

template <typename T>
class Queue {
  T** tab;
  const size_t allocsize;
  size_t begin;
  size_t sz;
  mutable std::mutex m;
  std::condition_variable cv_prod, cv_cons;
  bool block;

  bool empty() const { return sz == 0; }
  bool full() const { return sz == allocsize; }

 public:
  Queue(size_t size) : allocsize(size), begin(0), sz(0) {
    block = true;
    tab = new T*[size];
    memset(tab, 0, size * sizeof(T*));
  }

  size_t size() const {
    std::unique_lock<std::mutex> lg(m);
    return sz;
  }

  T* pop() {
    std::unique_lock<std::mutex> lg(m);
    while (empty() && block) cv_cons.wait(lg);
    if (empty() && !block) return nullptr;
    cv_prod.notify_one();
    auto ret = tab[begin];
    tab[begin] = nullptr;
    sz--;
    begin = (begin + 1) % allocsize;
    return ret;
  }

  bool push(T* elt) {
    std::unique_lock<std::mutex> lg(m);
    while (full() && block) cv_prod.wait(lg);
    if (full() && !block) return false;
    cv_cons.notify_one();
    tab[(begin + sz) % allocsize] = elt;
    sz++;
    return true;
  }

  ~Queue() {
    std::unique_lock<std::mutex> lg(m);
    for (size_t i = 0; i < sz; i++) {
      auto ind = (begin + i) % allocsize;
      delete tab[ind];
    }
    delete[] tab;
  }

  void setBlocking(bool isBlocking) {
    {
      std::unique_lock<std::mutex> l(m);
      block = isBlocking;
    }
    cv_cons.notify_all();
  }
};

}  // namespace pr

#endif /* SRC_QUEUE_H_ */
