#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
class Response {
 private:
  std::string response;
  std::string line;
  
 public:
  Response() : response("") {}
  std::string getLine(){return line;}
  void ParseLine(char *first_part,int len);
  void AppendResponse(char *,int len);
  int getSize();
  const char * getResponse();
};
