#include "proxy.h"

#include "function.h"

void proxy::run() {
  int temp_fd = build_server(this->port_num);
  int client_fd = server_accept(temp_fd);
  char req_msg[8192] = {0};
  // while(1){
  recv(client_fd, req_msg, sizeof(req_msg), 0);
  std::cout << "The receiver message is: " << req_msg << std::endl;
  int server_fd  = build_client("www.google.com","443");
  if(server_fd==-1){
    std::cout<<"Server Error"<<std::endl;
  }
  send(server_fd,req_msg,sizeof(req_msg),0);
  char response_msg[8192]={0};
  int status;
  //while((status=recv(server_fd,response_msg,sizeof(response_msg),0))>0){
    send(client_fd,"HTTP/1.1 200 OK\r\n\r\n",19,0);
    //}
    //std::cout << "The response message is: " << response_msg << std::endl;
  // }
}
