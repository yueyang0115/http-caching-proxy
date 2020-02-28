#include <pthread.h>

#include <fstream>

#include "function.h"
#include "proxy.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
std::ofstream logFile("/var/log/erss/proxy.log");
