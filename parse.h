#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

class Parse {
 public:
  std::string host;
  std::string port;
  std::string method;
  std::string log_info;
  std ::string send;
  std::string input;

 public:
  Parse(char * request) {
    std::stringstream ss;
    ss << request;
    ss >> input;
  }
  void ParseInput();
};
