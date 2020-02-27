//#include <fcntl.h>

#include <time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>

#include "expiretime.h"
class Response {
 public:
  std::string response;
  std::string line;
  std::string Etag;
  std::string Last_modified;
  int max_age;
  parsetime expire_time;
  std::string exp_str;
  parsetime response_time;
  bool nocache_flag;
  std::string ETag;
  std::string LastModified;

 public:
  // Response(std::string msg) : response(msg) {}
  Response() :
      Etag(""),
      Last_modified(""),
      max_age(-1),
      exp_str(""),
      nocache_flag(false),
      ETag(""),
      LastModified("") {}
  std::string getLine() { return line; }
  void ParseLine(char * first_part, int len);
  void AppendResponse(char *, int len);
  int getSize();
  const char * getResponse();
  void ParseField(char * first_msg, int len);
  void setEntireRes(std::string msg) { response = msg; }
};
