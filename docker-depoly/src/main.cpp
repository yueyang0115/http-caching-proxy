#include <pthread.h>

#include "function.h"
#include "proxy.h"
int main() {
  const char * port = "12345";
  proxy * myproxy = new proxy(port);
  myproxy->run();
  return 1;
}
