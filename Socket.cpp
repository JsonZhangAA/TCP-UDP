#include "./Socket.h"

Socket::Socket(bool flag,int port)
{
    if(flag)
    {
        socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    }
    else
    {
        socket_fd=socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    }
    assert(socket_fd>=0);
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=INADDR_ANY;
}

void Socket::Bind()
{
    printf("socket_fd: %d\n",socket_fd);
    if(bind(socket_fd,(struct sockaddr *)(&addr),sizeof(addr)))
    {
        printf("bind failed");
        return;
    }
}

void Socket::Listen()
{
    if(listen(socket_fd,5))
    {
        printf("listen failed\n");
        return;
    }
}

void Socket::Connect()
{
    if(connect(socket_fd,(struct sockaddr *)(&addr),sizeof(addr)))
    {
        printf("connect failed\n");
        return;
    }
}

void Socket::Accept()
{
    struct sockaddr_in temp_addr;
    memset(&temp_addr,0,sizeof(temp_addr));
    socklen_t len;
    return_fd=accept(socket_fd,(struct sockaddr *)(&temp_addr),&len);
    if(return_fd<0)
    {
        printf("accept failed\n");
        return;
    }
}

void Socket::Read(char * buf, int len)
{
    if(read(socket_fd,buf,len))
    {

    }
}

void Socket::Write(char * buf,int len)
{
    if(write(socket_fd,buf,len))
    {

    }
}