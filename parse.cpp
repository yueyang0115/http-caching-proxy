#include "parse.h"

#include <string>
void Parse::ParseInput() {
  size_t pos = input.find("Host: ");
  std::string after_host = input.substr(pos + 6);
  size_t host_line_end = after_host.find_first_of("\r\n");
  std::string hostline = after_host.substr(0, host_line_end - pos - 7);
  size_t host_end;
  if ((host_end = hostline.find(":")) != std::string::npos) {
    host = hostline.substr(0, host_end - 1);
    port = hostline.substr(host_end + 1);
  }
  else {
    host = hostline;
    port = "80";
  }
}
