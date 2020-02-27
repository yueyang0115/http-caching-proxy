#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
class timemap {
 public:
  std::map<std::string, int> time_map;

 public:
  timemap() {
    time_map.insert(std::pair<std::string, int>("Jan", 1));
    time_map.insert(std::pair<std::string, int>("Feb", 2));
    time_map.insert(std::pair<std::string, int>("Mar", 3));
    time_map.insert(std::pair<std::string, int>("Apr", 4));
    time_map.insert(std::pair<std::string, int>("May", 5));
    time_map.insert(std::pair<std::string, int>("Jun", 6));
    time_map.insert(std::pair<std::string, int>("Jul", 7));
    time_map.insert(std::pair<std::string, int>("Aug", 8));
    time_map.insert(std::pair<std::string, int>("Sep", 9));
    time_map.insert(std::pair<std::string, int>("Oct", 10));
    time_map.insert(std::pair<std::string, int>("Nov", 11));
    time_map.insert(std::pair<std::string, int>("Dec", 12));
    time_map.insert(std::pair<std::string, int>("Sun", 0));
    time_map.insert(std::pair<std::string, int>("Mon", 1));
    time_map.insert(std::pair<std::string, int>("Tue", 2));
    time_map.insert(std::pair<std::string, int>("Wed", 3));
    time_map.insert(std::pair<std::string, int>("Thu", 4));
    time_map.insert(std::pair<std::string, int>("Fri", 5));
    time_map.insert(std::pair<std::string, int>("Sat", 6));
  }
  int getMap(std::string str) { return time_map.find(str)->second; }
};

class parsetime {
 private:
  struct tm time_struct;

 public:
  parsetime() {}
  void init(std::string exp) {
    timemap mymap;
    time_struct.tm_mday = atoi(exp.substr(5).c_str());
    time_struct.tm_mon = mymap.getMap(exp.substr(8, 3).c_str()) - 1;
    time_struct.tm_year = atoi(exp.substr(12).c_str()) - 1900;
    time_struct.tm_hour = atoi(exp.substr(17).c_str());
    time_struct.tm_min = atoi(exp.substr(20).c_str());
    time_struct.tm_sec = atoi(exp.substr(23).c_str());
    time_struct.tm_wday = mymap.getMap(exp.substr(0, 3).c_str());
    time_struct.tm_isdst = 0;
  }
  struct tm * getTimeStruct() {
    return &time_struct;
  }
};
