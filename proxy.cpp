#include "proxy.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <ctime>
#include <fstream>
#include <map>

#include "client_info.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
std::ofstream logFile("proxy.log");

std::map<std::string, Response> Cache;
void proxy::run() {
  Client_Info * client_info = new Client_Info();
  int temp_fd = build_server(this->port_num);
  if (temp_fd == -1) {
    return;
  }
  int client_fd;
  int id = 0;
  while (1) {
    std::string ip;
    client_fd = server_accept(temp_fd, &ip);
    if (client_fd == -1) {
      std::cout << "connect client error" << std::endl;
      continue;
    }
    pthread_t thread;
    client_info->setFd(client_fd);
    client_info->setIP(ip);
    client_info->setID(id);
    id++;
    pthread_create(&thread, NULL, handle, client_info);
  }
}

void * proxy::handle(void * info) {
  Client_Info * client_info = (Client_Info *)info;
  int client_fd = client_info->getFd();

  char req_msg[65536] = {0};
  int len = recv(client_fd, req_msg, sizeof(req_msg), 0);  // fisrt request from client
  if (len <= 0) {
    return NULL;
  }
  std::string input = std::string(req_msg, 65536);
  if (input == "") {
    return NULL;
  }
  Request * parser = new Request(input);
  pthread_mutex_lock(&mutex);
  logFile << client_info->getID() << ": \"" << parser->line << "\" from "
          << client_info->getIP() << " @ " << getTime() << std::endl;
  pthread_mutex_unlock(&mutex);

  std::cout << "received client request is:" << req_msg << "end" << std ::endl;
  std::cout << "received client request's length:" << len << "end" << std ::endl;

  const char * method = parser->method.c_str();
  std::cout << "method is " << method << "end\n";
  const char * host = parser->host.c_str();
  const char * port = parser->port.c_str();
  std::cout << "host is " << host << "end\n";
  std::cout << "port is " << port << "end\n";

  int server_fd = build_client(host, port);  //connect to server
  if (server_fd == -1) {
    std::cout << "Error in build client!\n";
    return NULL;
  }
  pthread_mutex_lock(&mutex);
  logFile << client_info->getID() << ": "
          << "Requesting \"" << parser->line << "\" from " << host << std::endl;
  pthread_mutex_unlock(&mutex);

  if (parser->method == "CONNECT") {
    handleConnect(client_fd, server_fd, client_info->getID());
  }
  else if (parser->method == "GET") {
    bool valid = false;
    std::map<std::string, Response>::iterator it;
    if ((it = Cache.find(parser->line)) != Cache.end()) {
      valid = CheckTime(it->second);  //
    }
    if (!valid) {
      send(server_fd, req_msg, len, 0);
      handleGet(client_fd, server_fd, client_info->getID(), host);
    }
  }
  else if (parser->method == "POST") {
    handlePOST(client_fd, server_fd, req_msg, len, client_info->getID(), host);
  }
  close(server_fd);
  close(client_fd);
  return NULL;
}
bool proxy::CheckTime(Response & rep) {
  if (rep.max_age != -1) {
    time_t curr_time = time(0);
    time_t rep_time = mktime(rep.response_time.getTimeStruct());
    int max_age = rep.max_age;
    if (rep_time + max_age <= curr_time) {
      return false;
    }
  }
  if (rep.exp_str != "") {
    time_t curr_time = time(0);
    time_t expire_time = mktime(rep.expire_time.getTimeStruct());
    if (curr_time > expire_time) {
      return false;
    }
  }
  return true;
  //Wed Feb 26 04:09:10 2020
}

void proxy::handlePOST(int client_fd,
                       int server_fd,
                       char * req_msg,
                       int len,
                       int id,
                       const char * host) {
  int post_len = getLength(req_msg, len);  //get length of client request
  if (post_len != -1) {
    std::string request = sendContentLen(client_fd, req_msg, len, post_len);
    char send_request[request.length() + 1];
    strcpy(send_request, request.c_str());
    std::cout << "begin sending to server" << std::endl;
    send(server_fd,
         send_request,
         sizeof(send_request),
         MSG_NOSIGNAL);  // send all the request info from client to server
    char response[65536] = {0};
    int response_len = recv(server_fd,
                            response,
                            sizeof(response),
                            MSG_WAITALL);  //first time received response from server
    if (response_len != 0) {
      Response res;
      res.ParseLine(req_msg, len);
      pthread_mutex_lock(&mutex);
      logFile << id << ": Received \"" << res.getLine() << "\" from " << host
              << std::endl;
      pthread_mutex_unlock(&mutex);

      std::cout << "receive response from server which is:" << response << std::endl;
      std::cout << "begin sending to client" << std::endl;
      send(client_fd, response, response_len, MSG_NOSIGNAL);
      std::cout << "finish sending to clinet" << std::endl;
    }
    else {
      std::cout << "server socket closed!\n";
    }
  }
}
void proxy::handleGet(int client_fd, int server_fd, int id, const char * host) {
  char server_msg[65536] = {0};
  int mes_len = recv(server_fd,
                     server_msg,
                     sizeof(server_msg),
                     0);  //received first response from server(all header, part body)
  std::cout << "Receive server response is: " << server_msg << std::endl;
  if (mes_len == 0) {
    return;
  }
  Response response;
  response.ParseLine(server_msg, mes_len);
  pthread_mutex_lock(&mutex);
  logFile << id << ": Received \"" << response.getLine() << "\" from " << host
          << std::endl;
  pthread_mutex_unlock(&mutex);

  bool is_chunk = findChunk(server_msg, mes_len);
  if (is_chunk) {
    send(client_fd, server_msg, mes_len, 0);  //send first response to server
    char chunked_msg[28000] = {0};
    while (1) {  //receive and send remaining message
      int len = recv(server_fd, chunked_msg, sizeof(chunked_msg), 0);
      if (len <= 0) {
        return;
      }
      send(client_fd, chunked_msg, len, 0);
    }
  }
  else {
    int content_len = getLength(server_msg, mes_len);  //get content length
    //no-store--store in cache
    Response parse_res;
    parse_res.ParseField(server_msg, mes_len);
    if (content_len != -1) {
      std::cout << "\n content_len = " << content_len << std::endl;
      std::string msg = sendContentLen(
          server_fd, server_msg, mes_len, content_len);  //get the entire message
      char send_response[msg.length() + 1];
      strcpy(send_response, msg.c_str());
      parse_res.setEntireRes(msg);
      //std::cout << "Send client response is: " << send_response << std::endl;
      send(client_fd, send_response, sizeof(send_response), MSG_NOSIGNAL);
    }
    else {
      std::cout << "no content-length "
                   "field-----------------Return to client at once"
                << std::endl;
      std::string server_msg_str(server_msg, mes_len);
      parse_res.setEntireRes(server_msg_str);
      send(client_fd, server_msg, mes_len, 0);
    }
  }
}

std::string proxy::sendContentLen(int send_fd,
                                  char * server_msg,
                                  int mes_len,
                                  int content_len) {
  int total_len = 0;
  int len = 0;
  std::string msg(server_msg, mes_len);
  while (total_len < content_len) {
    len = recv(send_fd, server_msg, sizeof(server_msg), 0);
    std::cout << "\n in while loop, received length = " << len << std::endl;
    std::string temp(server_msg, len);
    msg += temp;
    total_len += len;
  }
  std::cout << "\n after while loop, total length = " << total_len << std::endl;
  return msg;
}

bool proxy::findChunk(char * server_msg, int mes_len) {
  std::string msg(server_msg, mes_len);
  size_t pos;
  if ((pos = msg.find("chunked")) != std::string::npos) {
    return true;
  }
  return false;
}

int proxy::getLength(char * server_msg, int mes_len) {
  std::cout << "mes_len = " << mes_len << std::endl;
  std::string msg(server_msg, mes_len);
  size_t pos;
  if ((pos = msg.find("Content-Length: ")) != std::string::npos) {
    size_t head_end = msg.find("\r\n\r\n");
    std::cout << "head_end=" << head_end << std::endl;
    int part_body_len = mes_len - static_cast<int>(head_end) - 8;
    std::cout << "cast<head_end>=" << static_cast<int>(head_end) << std::endl;
    std::cout << "part_body_len = " << part_body_len << std::endl;
    size_t end = msg.find("\r\n", pos);
    std::string content_len = msg.substr(pos + 16, end - pos - 16);
    std::cout << "conten_length string is: " << content_len << std::endl;
    std::cout << "string length = " << content_len.length() << std::endl;
    int num = 0;
    for (size_t i = 0; i < content_len.length(); i++) {
      num = num * 10 + (content_len[i] - '0');
    }
    std::cout << "calculate num =" << num << std::endl;
    return num - part_body_len - 4;
  }
  return -1;
}

void proxy::handleConnect(int client_fd, int server_fd, int id) {
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
    //char message[65536] = {0};
    for (int i = 0; i < 2; i++) {
      char message[65536] = {0};
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
  pthread_mutex_lock(&mutex);
  logFile << id << "Tunnel closed" << std::endl;
  pthread_mutex_unlock(&mutex);
}

std::string proxy::getTime() {
  time_t currTime = time(0);
  struct tm * nowTime = gmtime(&currTime);
  const char * t = asctime(nowTime);
  std::cout << "The current time UTC/GMT is: " << std::string(t) << std::endl;
  return std::string(t);
}
