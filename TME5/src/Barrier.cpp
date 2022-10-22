#include "Barrier.h"

namespace pr {

Barrier::Barrier(int n) : n(n), counter(0) {}

void Barrier::waitFor() {
  std::unique_lock<std::mutex> l(m);
  while (counter < n) cv.wait(l);
}

void Barrier::done() {
  std::unique_lock<std::mutex> l(m);
  ++counter;
  if (counter == n) cv.notify_all();
}

}  // namespace pr
