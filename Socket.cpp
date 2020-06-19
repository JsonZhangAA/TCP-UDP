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

Socket::Socket(bool flag,char * sun_path)
{
    if(flag)
        socket_fd=socket(AF_LOCAL,SOCK_STREAM,0);
    else
        socket_fd=socket(AF_LOCAL,SOCK_DGRAM,0);
    bzero(&addr1,sizeof(addr1));
    addr1.sun_family=AF_LOCAL;
    strncpy(addr1.sun_path,sun_path,sizeof(addr1.sun_path)-1);
}

Socket::Socket(bool flag,int port, struct in_addr * ptr)
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
    memcpy(&addr.sin_addr, ptr, sizeof(struct in_addr));
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

void Socket::BindUnix()
{
    printf("socket_fd: %d\n",socket_fd);
    if(bind(socket_fd,(struct sockaddr *)(&addr1),sizeof(addr1)))
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

int Socket::Connect()
{
    printf("socket_fd: %d\n",socket_fd);
    if(connect(socket_fd,(struct sockaddr *)(&addr),sizeof(addr)))
    {
        printf("connect failed\n");
        return -1;
    }
    return 0;
}

int Socket::ConnectUnix()
{
    printf("socket_fd: %d\n",socket_fd);
    if(connect(socket_fd,(struct sockaddr *)(&addr1),sizeof(addr1)))
    {
        printf("connect failed\n");
        return -1;
    }
    return 0;
}

int Socket::Accept()
{
    struct sockaddr_in temp_addr;
    memset(&temp_addr,0,sizeof(temp_addr));
    socklen_t len;
    return_fd=accept(socket_fd,(struct sockaddr *)(&temp_addr),&len);
    return return_fd;
}

int Socket::AcceptUnix()
{
    struct sockaddr_un temp_addr;
    memset(&temp_addr,0,sizeof(temp_addr));
    socklen_t len;
    return_fd=accept(socket_fd,(struct sockaddr *)(&temp_addr),&len);
    return return_fd;
}

int Socket::Read(int fd,void * buf, int len)
{
    printf("waiting read sockfd: %d\n",fd);
    return read(fd,buf,len);
}
//ssize_t nr = ::recvfrom(sock.fd(), &message, sizeof message, 0, &peerAddr, &addrLen);
int Socket::Recvfrom(void * buf, int len,struct sockaddr_in & peerAddr)
{
    //struct sockaddr peerAddr;
    bzero(&peerAddr, sizeof peerAddr);
    socklen_t addrLen = sizeof(peerAddr);
    return recvfrom(socket_fd,buf,len,0,(struct sockaddr *)&peerAddr,&addrLen);
}

int Socket::Recvfrom(void * buf, int len,struct sockaddr_un & peerAddr)
{
    //struct sockaddr peerAddr;
    bzero(&peerAddr, sizeof peerAddr);
    socklen_t addrLen = sizeof(peerAddr);
    return recvfrom(socket_fd,buf,len,0,(struct sockaddr *)&peerAddr,&addrLen);
}

int Socket::Write(int fd,const void * buf,int len)
{
    printf("writing sockfd: %d\n",fd);
    return write(fd,buf,len);
}
//sendto(sock.fd(), &message, sizeof message, 0, &peerAddr, addrLen);
int Socket::Sendto(const void * buf,int len,struct sockaddr_in & addr)
{
    socklen_t addrLen = sizeof(addr);
    return sendto(socket_fd, buf, len,0,(struct sockaddr *)&addr,addrLen);
}

int Socket::Sendto(const void * buf,int len,struct sockaddr_un & addr)
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