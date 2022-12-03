#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <ostream>
#include <iosfwd>
#include <iostream>
#include <string>

namespace pr {

inline std::ostream& operator<<(std::ostream& os, const struct sockaddr_in* addr) {
  return os << inet_ntoa(addr->sin_addr) << ":" << ntohs(addr->sin_port);
}

class Socket {
  int fd;

 public:
  Socket() : fd(-1) {}
  Socket(int fd) : fd(fd) {}

  void connect(const std::string& host, int port) {
    struct hostent* h = gethostbyname(host.c_str());
    if (!h) {
      perror("gethostbyname");
      exit(1);
    }
    connect(*(struct in_addr *)h->h_addr, port);
  }

  void connect(in_addr ipv4, int port) {
    //std::cout << inet_ntoa(ipv4) << " : " << port << std::endl;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }

    struct sockaddr_in t;
    t.sin_addr = ipv4;
    t.sin_family = AF_INET;
    t.sin_port = htons(port);

    if (::connect(fd, (struct sockaddr*)&t, sizeof(t)) < 0) {
      perror("connect");
      exit(1);
    }
  }

  bool isOpen() const { return fd != -1; }
  int getFD() { return fd; }
  void close() { ::close(fd); };

};

}  // namespace pr

#endif /* SRC_SOCKET_H_ */
