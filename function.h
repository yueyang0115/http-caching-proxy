#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
using namespace std;

int build_server(const char * port);
int build_client(const char * hostname, const char * port);
int server_accept(int socket_fd);
int get_port_num(int socket_fd);
