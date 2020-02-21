#include "proxy.h"

#include "function.h"

void proxy::run() {
  int temp_fd = build_server(this->port_num);
  int client_fd = server_accept(temp_fd);
  char req_msg[8192] = {0};
  // while(1){
  recv(client_fd, req_msg, sizeof(req_msg), 0);
  std::cout << "The receiver message is: " << req_msg << std::endl;
  int server_fd = build_client("www.google.com", "443");
  if (server_fd == -1) {
    std::cout << "Server Error" << std::endl;
  }
  send(server_fd, req_msg, sizeof(req_msg), 0);
  send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);

  fd_set readfds;
  int nfds = server_fd > client_fd ? server_fd + 1 : client_fd + 1;
  while (1) {
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    FD_SET(client_fd, &readfds);
    select(nfds + 1, &readfds, NULL, NULL, NULL);

    int fd[2] = {client_fd, server_fd};
    char message[8192] = {0};
    for (int i = 0; i < 2; i++) {
      if (FD_ISSET(fd[i], &readfds)) {
        recv(fd[i], message, sizeof(message), MSG_WAITALL);
        send(fd[1 - i], message, sizeof(message), 0);
      }
    }
  }
}
