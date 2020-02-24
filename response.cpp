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
void Response::ParseLine(char *first_part,int len){
  std::string first_part_str(first_part,len);
  size_t pos = first_part_str.find_first_of("\r\n");
  line = first_part_str.substr(0, pos);
}
