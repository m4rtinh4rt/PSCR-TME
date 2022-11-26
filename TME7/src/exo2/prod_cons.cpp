#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "Stack.h"

using namespace std;
using namespace pr;

void phandler(int sig) {
  if (SIGINT == sig) {
  }
}

void chandler(int sig) {
  if (SIGINT == sig) {
    exit(0);
  }
}

void producteur(Stack<char>* stack) {
  char c;
  while (cin.get(c)) {
    stack->push(c);
  }
}

void consomateur(Stack<char>* stack) {
  while (true) {
    char c = stack->pop();
    cout << c << flush;
  }
}

int main() {
  signal(SIGINT, phandler);
  int fd =
      shm_open("trololopath", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  if (ftruncate(fd, sizeof(Stack<char>)) < 0) {
    perror("ftruncate");
    exit(1);
  }

  Stack<char>* s =
      new (mmap(NULL, sizeof(struct Stack<char>), PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0)) Stack<char>();

  pid_t pp = fork();
  if (pp == 0) {
    signal(SIGINT, chandler);
    producteur(s);
    return 0;
  }

  pid_t pc = fork();
  if (pc == 0) {
    signal(SIGINT, chandler);
    consomateur(s);
    return 0;
  }

  while (wait(nullptr) > 0) continue;

  close(fd);
  shm_unlink("trololopath");
  munmap(s, sizeof(struct Stack<char>));
  return 0;
}
