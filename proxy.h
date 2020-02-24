#include "function.h"
#include "parse.h"
#include "pthread.h"

class proxy {
 private:
  const char * port_num;

 public:
  proxy(const char * myport) : port_num(myport) {}
  void run();
  static void * handle(void * arg);
  static void handleConnect(int client_fd, int server_fd);
  static void handleGet(int client_fd, int server_fd);
  static std::string sendContentLen(int send_fd, char *server_msg,int mes_len, int content_len);
  static int getLength(char * server_msg, int mes_len);
  static bool findChunk(char *server_msg,int mes_len);
};
