#include "parse.h"
#include <iostream>
#include <cstring>

void Parse::ParseInput() {
  //  std::cout<<"input is"<<input<<std::endl;
  size_t pos = input.find("Host: ");
  std::string after_host = input.substr(pos + 6);
  //  std::cout<<"after_host is"<<after_host<<std::endl;
  size_t host_line_end;
  //std::cout << "After Host charcter is: " << after_host[pos + 6] << std:: endl;
  if((host_line_end = after_host.find_first_of(":\r")) != std::string::npos){
    //std::cout << ": before port number is: " << host_line_end << std::endl;
    host = after_host.substr(0, host_line_end);
    //std::cout<<"host ="<<host<<std::endl;  
    size_t port_end = after_host.find_first_of("\r\n");
    port = after_host.substr(host_line_end + 1, port_end - host_line_end - 1 );
  }
    else {
      size_t port_end = after_host.find_first_of("\r\n");
      host = after_host.substr(0, port_end);
      std::cout<<"host = "<<host<<std::endl;
      port = "80";
  }
}
