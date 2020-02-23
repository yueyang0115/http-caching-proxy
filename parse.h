#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
class Parse {
 public:
  std::string host;
  std::string port;
  std::string method;
  std::string log_info;
  std::string send;
  std::string input;

 public:
  Parse(std::string request) : input(request) {
    //    std::cout << "request origin: " << request << std::endl;
    //std::cout<<"request in parse function is"<<input<<std::endl;
    ParseInput();
    ParseMethod();
  }

  void ParseInput();
  void ParseMethod();
};
