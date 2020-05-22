#include "./Socket.h"

Socket::Socket(bool flag,int port)
{
    if(flag)
    {
        socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    }
    else
    {
        socket_fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
    printf("socket_fd: %d\n",socket_fd);
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

int Socket::Read(void * buf, int len)
{
    //printf("waiting read sockfd: %d\n",socket_fd);
    return read(socket_fd,buf,len);
}
//ssize_t nr = ::recvfrom(sock.fd(), &message, sizeof message, 0, &peerAddr, &addrLen);
int Socket::Recvfrom(void * buf, int len,struct sockaddr_in & peerAddr)
{
    //struct sockaddr peerAddr;
    bzero(&peerAddr, sizeof peerAddr);
    socklen_t addrLen = sizeof(peerAddr);
    return recvfrom(socket_fd,buf,len,0,(struct sockaddr *)&peerAddr,&addrLen);
}

int Socket::Write(const void * buf,int len)
{
    //printf("writing sockfd: %d\n",socket_fd);
    return write(socket_fd,buf,len);
}
//sendto(sock.fd(), &message, sizeof message, 0, &peerAddr, addrLen);
int Socket::Sendto(const void * buf,int len,struct sockaddr_in & addr)
{
    socklen_t addrLen = sizeof(addr);
    return sendto(socket_fd, buf, len,0,(struct sockaddr *)&addr,addrLen);
}

void Socket::Setsockopt()
{
    int yes=1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
    {
        perror("setsockopt");
        return;
    }
}