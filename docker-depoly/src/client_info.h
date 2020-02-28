#include <pthread.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

class Client_Info {
 private:
  int id;
  int client_fd;
  //struct sockaddr_storage client_addr;
  std::string ip;

 public:
  void setFd(int my_client_fd) { client_fd = my_client_fd; }
  int getFd() { return client_fd; }
  void setIP(std::string myip) { ip = myip; }
  std::string getIP() { return ip; }
  void setID(int myid) { id = myid; }
  int getID() { return id; }
};
