#include <pthread.h>

#include <cstdio>
#include <cstdlib>

class Client_Info {
 private:
  int id;
  int client_fd;
  struct sockaddr_storage client_addr;

 public:
  void setFd(int my_client_fd) { client_fd = my_client_fd; }
  int getFd() { return client_fd; }
};
