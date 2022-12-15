#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "TCPServer.h"

#define BUFFER_SIZE 4096

enum { LIST, UPLOAD, DOWNLOAD };

int parse_cmd(char* buffer) {
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

std::string list_file() {
  int n;
  std::string s;
  struct dirent** namelist;

  if ((n = scandir(".", &namelist, NULL, alphasort)) < 0) {
    perror("scandir");
  } else {
    while (n--) {
      s.append(namelist[n]->d_name);
      s.append("\n");
      free(namelist[n]);
    }
    free(namelist);
  }
  return s;
}

class IncrementServer : public pr::ConnectionHandler {
 public:
  void handleConnection(pr::Socket sc) {
    char buffer[BUFFER_SIZE] = {0};
    int cmd, fd = sc.getFD();

    read(fd, buffer, BUFFER_SIZE);
    if ((cmd = parse_cmd(buffer)) < 0) goto end;

    switch (cmd) {
      case LIST: {
        std::string dirc = list_file();
        write(fd, dirc.c_str(), dirc.length());
        break;
      }
      case UPLOAD: {  // NOT safe: can write everywhere on server fs.
        int f;
        if ((f = open(buffer + 7, O_WRONLY | O_CREAT, 0600)) < 0) {
          goto end;
        }
        char fbuffer[BUFFER_SIZE] = {0};
        read(fd, fbuffer, BUFFER_SIZE);
        write(f, fbuffer, BUFFER_SIZE);
        close(f);
        break;
      }
      case DOWNLOAD: {  // NOT safe
        int f;
        if ((f = open(buffer + 9, O_RDONLY)) < 0) {
          write(f, "ERRNAME", 7);
          break;
        }
        char fbuffer[BUFFER_SIZE] = {0};
        read(f, fbuffer, BUFFER_SIZE);
        write(fd, fbuffer, BUFFER_SIZE);
        close(f);
        break;
      }
    }
  end:
    sc.close();
  }
  ConnectionHandler* clone() const { return new IncrementServer(); }
};

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << *argv << " PORT FILE_DIRECTORY" << std::endl;
    exit(1);
  }
  pr::TCPServer server(new IncrementServer());

  if (chdir(argv[2]) < 0) {
    perror("chdir");
    exit(1);
  }

  server.startServer(std::stoi(argv[1]));

  // attend entree sur la console
  std::string s;
  std::cin >> s;

  std::cout << "Début fin du serveur." << std::endl;
  // on quit
  server.stopServer();
  std::cout << "Ok fin du serveur." << std::endl;

  return 0;
}
