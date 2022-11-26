#include <string.h>
#include <unistd.h>

#include <iostream>

void child(char **argv, int fd0, int fd1, int cfd) {
  int parent;

  if ((parent = fork()) < 0) {
    perror("fork");
    exit(1);
  }

  if (!parent) {
    if (dup2(fd0, cfd) < 0) {
      perror("dup2");
      exit(1);
    }
    close(fd0);
    close(fd1);
    if (execv(*argv, argv) < 0) {
      perror("execv");
      exit(1);
    }
  }
}

int main(int argc, char *argv[]) {
  char **t2_args;
  int i, fd[2] = {0};

  if (argc < 3) {
    std::cerr << "usage: ./a.out /bin/cat pipe.cpp \\| /bin/wc -l" << std::endl;
    exit(1);
  }

  t2_args = argv;
  for (i = 0; i < argc; ++i) {
    if (!strcmp(*t2_args, "|")) {
      *t2_args = nullptr;
      ++t2_args;
      break;
    }
    ++t2_args;
  }
  ++argv;

  if (pipe(fd) < 0) {
    perror("pipe");
    exit(1);
  }

  child(t2_args, fd[0], fd[1], STDIN_FILENO);
  child(argv, fd[1], fd[0], STDOUT_FILENO);

  return 0;
}
