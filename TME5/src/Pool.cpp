#include "Pool.h"

#include <thread>
#include <vector>

#include "Job.h"
#include "Queue.h"

namespace pr {

void poolWorker(Queue<Job>& queue) {
  while (true) {
    Job* j = queue.pop();
    if (j == nullptr) break;
    j->run();
    delete j;
  }
}

Pool::Pool(int qsize) { queue = new Queue<Job>(qsize); }

Pool::~Pool() { delete queue; }

void Pool::submit(Job* job) { queue->push(job); }

void Pool::start(int nbthread) {
  for (int i = 0; i < nbthread; ++i)
    threads.emplace_back(poolWorker, std::ref(*queue));
}

void Pool::stop() {
  queue->setBlocking(false);
  for (auto& t : threads) t.join();
}

}  // namespace pr
