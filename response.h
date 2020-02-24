#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
class Response {
 private:
  std::string response;

 public:
  Response() : response("") {}
  void AppendResponse(char * new_part, int len);
  int getSize();
  const char * getResponse();
};
