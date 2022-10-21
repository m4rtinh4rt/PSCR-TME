#include "Banque.h"

#include <chrono>
#include <thread>
#include <iostream>

#define SOLDE_INITIAL 200
#define NB_ACCOUNT 5

using namespace std;

void work(pr::Banque& b) {
  int i, j, m; //, s;
  for (size_t x = 0; x < 1000; ++x) {
    i = rand() % NB_ACCOUNT;
    j = rand() % NB_ACCOUNT;
    m = rand() % 100;
    // s = rand() % 20 + 1;

    b.transfert(i, j, m);
    // std::this_thread::sleep_for(std::chrono::milliseconds(s));
  }
}

void worker(pr::Banque& b) {
  if (b.comptabiliser(NB_ACCOUNT * SOLDE_INITIAL))
    std::cout << "OK" << std::endl;
}

const int NB_THREAD = 10;
int main () {
	vector<thread> threads;
  pr::Banque b(NB_ACCOUNT, SOLDE_INITIAL);

	// TODO : creer des threads qui font ce qui est demandé
  for (int i = 0; i < NB_THREAD; ++i)
    threads.emplace_back(work, std::ref(b));

  std::thread t(worker, std::ref(b));

	for (auto & t : threads) {
		t.join();
	}

  t.join();

	// TODO : tester solde = NB_THREAD * JP

	return 0;
}
