#include "response.h"

//#include "expiretime.h"
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
void Response::ParseLine(char * first_part, int len) {
  std::string first_part_str(first_part, len);
  size_t pos = first_part_str.find_first_of("\r\n");
  line = first_part_str.substr(0, pos);
}

void Response::ParseField(char * first_msg, int len) {
  std::string msg(first_msg, len);
  size_t date_pos;
  if ((date_pos = msg.find("Date: ")) != std::string::npos) {
    size_t GMT_pos = msg.find(" GMT");
    std::string date_str = msg.substr(date_pos + 6, GMT_pos - date_pos - 6);
    std::cout << "Respnse date  = " << date_str << std::endl;
    response_time.init(date_str);
  }
  size_t max_age_pos;
  if ((max_age_pos = msg.find("max-age=")) != std::string::npos) {
    std::string max_age_str = msg.substr(max_age_pos + 8);
    //std::cout << "max_age after = is:" << max_age_str << std::endl;
    //max_age = toInteger(max_age_str);
    max_age = atoi(max_age_str.c_str());
    //std::cout << "first_msg=" << first_msg << std::endl;
    //std::cout << "(DEBUG!!!!!!!!)" << std::endl;
    //std::cout << "max_age=" << max_age << std::endl;
    return;
  }
  size_t expire_pos;
  if ((expire_pos = msg.find("Expires: ")) != std::string::npos) {
    size_t GMT_pos = msg.find(" GMT");
    exp_str = msg.substr(expire_pos + 9, GMT_pos - expire_pos - 9);
    //std::cout << "Expires time = " << exp_str << std::endl;
    expire_time.init(exp_str);
  }
}
