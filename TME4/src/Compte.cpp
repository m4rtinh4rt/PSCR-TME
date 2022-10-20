#include "Compte.h"

using namespace std;

namespace pr {
void Compte::crediter (unsigned int val) {
	unique_lock<recursive_mutex> g(m);
	solde+=val ;
}
bool Compte::debiter (unsigned int val) {
	unique_lock<recursive_mutex> g(m);
	bool doit = (unsigned int)solde >= val;
	if (doit) {
		solde-=val ;
	}
	return doit;
}
int Compte::getSolde() const  {
	unique_lock<recursive_mutex> g(m);
	return solde;
}
// NB : vector exige Copyable, mais recursive_mutex ne l'est pas...
Compte::Compte(const Compte & other) {
	other.m.lock();
  solde = other.solde;
  other.m.unlock();
}

std::recursive_mutex& Compte::getMutex() const {
  return m;
}

}
