#include <pthread.h>
#include "proxy.h"

#include "function.h"
typedef struct _thr_arg {
  size_t id;
  struct sockaddr_storage client_addr;
  int client_fd;
  std::ofstream * log_stream_p;
} thr_arg;

void proxy::run() {
  thr_arg* arg = (thr_arg *)malloc(sizeof(*arg));
  
  int temp_fd = build_server(this->port_num);
  int client_fd;
    while(1){
    client_fd = server_accept(temp_fd);
    pthread_t thread;

    arg->client_fd = client_fd;
    
    pthread_create(&thread, NULL, handle, arg);
  }
}

void* proxy::handle(void * varg){
  thr_arg * arg = (thr_arg*)varg;
  char req_msg[8192] = {0};
  int client_fd = arg->client_fd;
  recv(client_fd, req_msg, sizeof(req_msg), 0);
  //  std::cout << "The receiver message is: " << req_msg << std::endl;

  /*std::string str = std::string(req_msg, sizeof(req_msg));

  // get req_line
  std::string req_line = str.substr(0, str.find("\r\n"));

  // get method
  std::string method = str.substr(0, str.find(" "));

  // get host
  size_t host_first = str.find("Host: ") + 6;
  size_t host_next = str.find_first_of(":\r", host_first);
  std::string host = str.substr(host_first, host_next - host_first);

  // get port
  std::string port;
  if (str[host_next] == ':') {
    size_t port_next = str.find("\r\n", host_next);
    port = str.substr(host_next + 1, port_next - host_next - 1);
  }
  
  int port_len = port.length();
  char port_send[port_len + 1];

  strcpy(port_send, port.c_str());
  
  int host_len = host.length();
  char host_send[host_len + 1];
  strcpy(host_send, host.c_str());
  */
  //int server_fd_init = build_client(host_send, port_send);
  int server_fd_init = build_client("google.com", "443");
  char mes_buf[8192] = {0};
  /*
  std::cout << "server_fd is: " << server_fd << std::endl;
    
  if (server_fd == -1) {
    std::cout << "Server Error" << std::endl;
  }
  */
  if(send(server_fd_init, req_msg, sizeof(req_msg), MSG_NOSIGNAL) == 0){
    std::cout << "Message send to server is 0\n";
  }
  if(recv(server_fd_init, mes_buf, sizeof(mes_buf), 0) == 0){
    std::cout << "before while loop closed\n";
  }
  send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);

  //int server_fd = build_client(host_send, port_send);
  int server_fd = build_client("google.com", "443");
  fd_set readfds;
  int nfds = server_fd > client_fd ? server_fd + 1 : client_fd + 1;
  
  
  FD_ZERO(&readfds);
    while (1) {
    FD_SET(server_fd, &readfds);
    FD_SET(client_fd, &readfds);
    select(nfds, &readfds, NULL, NULL, NULL);
    /*int ready_fd;
    if (FD_ISSET(client_fd, &readfds)) {
        ready_fd = client_fd;
      }
    else if (FD_ISSET(server_fd, &readfds)) {  // first priority (terminating signal)
        ready_fd = server_fd;
      }
     
      else {
        std::perror("Cannot get the ready fd");
        return NULL;
      }

      int other = (ready_fd == client_fd) ? server_fd : client_fd;

      // Receive data from one side
      int mes_len = 0;
      char mes_buf[8192] = {0};
      mes_len = recv(ready_fd, mes_buf, sizeof(mes_buf), 0);
      if (mes_len == -1) {
        std::perror("CONNECT: receive data from client or server");
        return NULL;
      }

      if (mes_len == 0) {  // server or client has closed its socket
	std::string who = ready_fd == server_fd? "server" : "client";  
	std::cout << "Closed Socket!\n";
	std::cout << "The closed socket is: " << who << ready_fd << std::endl;;
	//std::cout << who << std::endl;
        break;
      }

      // Send date to the other side
      if (send(other, mes_buf, mes_len, MSG_NOSIGNAL) == -1) {
        std::perror("CONNECT: sent data to client or server");
        return NULL;
      }*/
    
    int fd[2] = {server_fd, client_fd};
    for (int i = 0; i < 2; i++) {
      if (FD_ISSET(fd[i], &readfds)) {
        int len;
	char message[8192] = {0};
	len = recv(fd[i], message, sizeof(message), 0);
	if(len == 0){
	  // std::cout << "Error: Receive length 0\n";
	  break;
	}
	else{
	  send(fd[1 - i], message, len, 0);
	  // std::cout << "Error in Sending!\n" << fd[i] << std::endl; 
       	}
      }
  }
    }
  return NULL;
}
