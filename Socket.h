/*
Socket类封装如下方法：
bind、listen和connect
read、write
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>

class Socket
{
    public:
        Socket(bool flag,int port);//flag为true表示使用TCP，否则使用UDP；
        ~Socket()
        {
            close(socket_fd);
            close(return_fd);   
        }
        void Bind();
        void Listen();
        void Connect();
        void Read(char * buf,int len);
        void Write(char * buf,int len);
        void Accept();
        int get_return_fd()
        {
            return return_fd;
        }
        int get_socket_fd()
        {
            return socket_fd;
        }
    private:
        int socket_fd,return_fd;
        struct sockaddr_in addr;
};