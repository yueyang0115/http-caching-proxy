#include "proxy.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "client_info.h"
#include "function.h"
#include "response.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void proxy::run() {
  Client_Info * client_info = new Client_Info();
  int temp_fd = build_server(this->port_num);
  if (temp_fd == -1) {
    return;
  }
  int client_fd;
  while (1) {
    client_fd = server_accept(temp_fd);
    if (client_fd == -1) {
      std::cout << "connect client error" << std::endl;
      return;
    }
    pthread_t thread;
    client_info->setFd(client_fd);
    pthread_create(&thread, NULL, handle, client_info);
  }
}

void * proxy::handle(void * info) {
  Client_Info * client_info = (Client_Info *)info;
  int client_fd = client_info->getFd();

  char req_msg[65536] = {0};
  int len = recv(client_fd, req_msg, sizeof(req_msg), 0);
  if (len <= 0) {
    return NULL;
  }
  std::cout << "received client request is:" << req_msg << "end" << std ::endl;
  std::cout << "received client request's length:" << len << "end" << std ::endl;

  std::string input = std::string(req_msg, 65536);
  if (input == "") {
    return NULL;
  }
  Parse * parser = new Parse(input);
  const char * method = parser->method.c_str();
  std::cout << "method is " << method << "end\n";

  const char * host = parser->host.c_str();
  const char * port = parser->port.c_str();
  std::cout << "host is " << host << "end\n";
  std::cout << "port is " << port << "end\n";

  int server_fd = build_client(host, port);
  if (server_fd == -1) {
    std::cout << "Error in build client!\n";
    return NULL;
  }

  if (parser->method == "CONNECT") {
    handleConnect(client_fd, server_fd);
  }
  else if (parser->method == "GET") {
    send(server_fd, req_msg, sizeof(req_msg), 0);
    handleGet(client_fd, server_fd);
  }
  return NULL;
}

void proxy::handleGet(int client_fd, int server_fd) {
  char server_msg[28000] = {0};
  int mes_len = recv(server_fd, server_msg, sizeof(server_msg), 0);
  int content_len = getLength(server_msg, mes_len);
  //send(client_fd, server_msg, mes_len, 0);
  if (content_len != -1) {
    int len = 0;
    Response response;
    int total_len = 0;
    std::string msg(server_msg, mes_len);
    while (total_len < content_len) {
      len = recv(server_fd, server_msg, sizeof(server_msg), 0);
      std::string temp(server_msg, len);
      msg += temp;
      total_len += len;
    }
    char send_response[msg.length()];
    strcpy(send_response, msg.c_str());
    std::cout << "Send client response is: " << send_response << std::endl;
    send(client_fd, send_response, sizeof(send_response), MSG_NOSIGNAL);
  }
}

int proxy::getLength(char * server_msg, int mes_len) {
  std::string msg(server_msg, mes_len);
  size_t pos;
  if ((pos = msg.find("Content-Length: ")) != std::string::npos) {
    size_t end = msg.find("\r\n", pos);
    std::string content_len = msg.substr(pos + 16, end - pos - 17);
    int num = 0;
    for (size_t i = 0; i < content_len.length(); i++) {
      num = num * 10 + (content_len[i] - '0');
    }
    return num;
  }
  return -1;
}

void proxy::handleConnect(int client_fd, int server_fd) {
  send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);

  fd_set readfds;
  int nfds = server_fd > client_fd ? server_fd + 1 : client_fd + 1;
  while (1) {
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    FD_SET(client_fd, &readfds);
    select(nfds, &readfds, NULL, NULL, NULL);
    int fd[2] = {server_fd, client_fd};
    int len;
    char message[65536] = {0};
    for (int i = 0; i < 2; i++) {
      if (FD_ISSET(fd[i], &readfds)) {
        len = recv(fd[i], message, sizeof(message), 0);
        if (len == 0) {
          //          std::cout << "Error: Receive length 0\n";
          return;
        }
        else {
          send(fd[1 - i], message, len, 0);
          // std::cout << "Error in Sending!\n" << fd[i] << std::endl;
        }
      }
    }
  }
}
