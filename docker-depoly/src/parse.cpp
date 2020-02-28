#include "parse.h"

#include <cstring>
#include <exception>
#include <iostream>
void Request::ParseLine() {
  size_t pos = input.find_first_of("\r\n");
  line = input.substr(0, pos);
}
void Request::ParseMethod() {
  size_t method_end = input.find_first_of(" ");
  method = input.substr(0, method_end);
}

void Request::ParseInput() {
  try {
    size_t pos = input.find("Host: ");
    std::string after_host = input.substr(pos + 6);
    size_t host_line_end;
    host_line_end = after_host.find_first_of("\r\n");
    std::string host_line = after_host.substr(0, host_line_end);
    size_t port_begin;
    if ((port_begin = host_line.find_first_of(":\r")) != std::string::npos) {
      host = after_host.substr(0, port_begin);
      port = host_line.substr(port_begin + 1);
    }
    else {
      host = host_line;
      port = "80";
    }
  }
  catch (std::exception & e) {
    std::cout << "Host is Empty!" << std::endl;
    host = "";
    port = "";
    return;
  }
}
