#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
//#include <sys/types.h> 
#include <signal.h> //for 信号相关的函数
#include <sys/wait.h>//for watipid
#include <netdb.h>

class helpNet
{
    public:
        int Tcp_connect(const char * host, const char * serv);
        int Tcp_listen(const char * host, const char * serv);
        int Udp_connect(const char * host, const char * serv,struct sockaddr * * resa, socklen_t * len);
        int Udp_client(const char * host, const char * serv,struct sockaddr * * resa, socklen_t * len);
        int Udp_server(const char * host, const char * serv, socklen_t * len);
};