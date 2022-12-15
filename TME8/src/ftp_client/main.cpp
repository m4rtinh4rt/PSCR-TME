#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Socket.h"

#define BUFFER_SIZE 4096

enum { END, LIST, UPLOAD, DOWNLOAD };

int parse_cmd(char *buffer) {
  switch (*buffer) {
    case 'L':
      if (std::memcmp(buffer, "LIST", 4)) break;
      return LIST;
    case 'U':
      if (std::memcmp(buffer, "UPLOAD ", 7)) break;
      return UPLOAD;
    case 'D':
      if (std::memcmp(buffer, "DOWNLOAD ", 9)) break;
      return DOWNLOAD;
  }
  std::cerr << "error: bad cmd" << std::endl;
  return -1;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << *argv << " IP_ADDRESS PORT" << std::endl;
    exit(1);
  }

  char buffer[BUFFER_SIZE];
  char recv[BUFFER_SIZE];
  while (true) {
    pr::Socket s;
    s.connect(argv[1], std::stoi(argv[2]));
    int cmd = 0;
    std::memset(buffer, 0, BUFFER_SIZE);
    std::cin.getline(buffer, BUFFER_SIZE);
    if ((cmd = parse_cmd(buffer)) < 0) {
      s.close();
      break;
    }
    std::memset(recv, 0, BUFFER_SIZE);
    switch (cmd) {
      case LIST:
        write(s.getFD(), buffer, BUFFER_SIZE);
        read(s.getFD(), recv, BUFFER_SIZE);
        std::cout << recv << std::endl;
        break;
      case UPLOAD: {
        int f;
        if ((f = open(buffer + 7, O_RDONLY)) < 0) {
          perror("open");
          break;
        }
        char fbuffer[BUFFER_SIZE] = {0};
        write(s.getFD(), buffer, BUFFER_SIZE);
        read(f, fbuffer, BUFFER_SIZE);
        write(s.getFD(), fbuffer, BUFFER_SIZE);
        close(f);
        break;
      }
      case DOWNLOAD:
        write(s.getFD(), buffer, BUFFER_SIZE);
        read(s.getFD(), recv, BUFFER_SIZE);
        if (!std::memcmp(buffer, "ERRNAME", 7)) {
          std::cerr << "FILE_NAME do not exist on the server" << std::endl;
          break;
        }
        int f;
        if ((f = open(buffer + 9, O_WRONLY | O_CREAT, 0600)) < 0) {
          perror("open");
          break;
        }
        write(f, recv, BUFFER_SIZE);
        close(f);
        break;
    }
    s.close();
  }
  return 0;
}
