#pragma once

#include <condition_variable>
#include <mutex>

namespace pr {

class Barrier {
  std::mutex m;
  int n, counter;
  std::condition_variable cv;

 public:
  Barrier(int n);
  void waitFor();
  void done();
};

}  // namespace pr
