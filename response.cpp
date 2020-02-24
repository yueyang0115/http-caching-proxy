#include "response.h"

void Response::AppendResponse(char * new_part, int len) {
  std::string new_part_str(new_part, len);
  response += new_part_str;
}
const char * Response::getResponse() {
  return response.c_str();
}
int Response::getSize() {
  return response.length();
}
