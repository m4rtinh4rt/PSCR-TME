#pragma once

#include <thread>
#include <vector>

#include "Job.h"
#include "Queue.h"

namespace pr {

void poolWorker(Queue<Job>& queue);

class Pool {
  Queue<Job>* queue;
  std::vector<std::thread> threads;

 public:
  Pool(int qsize);
  void submit(Job* job);
  void start(int nbthread);
  void stop();
  ~Pool();
};

}  // namespace pr
