#include "./Socket.h"

Socket::Socket(bool flag,int port, struct in_addr * ptr=NULL)
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
    if(ptr!=NULL)
    {
        memcpy(&addr.sin_addr, ptr, sizeof(struct in_addr));
    }
    else
    {
        addr.sin_addr.s_addr=INADDR_ANY;
    }
    inOrUn=true;
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
    inOrUn=false;
}

void Socket::Bind()
{
    realBind();
}

void Socket::BindUnix()
{
    realBind();
}

void Socket::realBind()
{   
    printf("socket_fd: %d\n",socket_fd);
    if(inOrUn)
    {
        if(bind(socket_fd,(struct sockaddr *)(&addr),sizeof(addr)))
        {
            printf("bind failed");
            return;
        }
    }
    else
    {
        if(bind(socket_fd,(struct sockaddr *)(&addr1),sizeof(addr1)))
        {
            printf("bind failed");
            return;
        }
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
    return realConnect();
}

int Socket::ConnectUnix()
{
    return realConnect();
}

int Socket::realConnect()
{
    printf("socket_fd: %d\n",socket_fd);
    if(inOrUn)
    {
        if(connect(socket_fd,(struct sockaddr *)(&addr),sizeof(addr)))
        {
            printf("connect failed\n");
            return -1;
        }
        return 0;
    }
    else
    {
        if(connect(socket_fd,(struct sockaddr *)(&addr1),sizeof(addr1)))
        {
            printf("connect failed\n");
            return -1;
        }
        return 0;
    }
    
}

int Socket::Accept()
{
    return realAccept();
}

int Socket::AcceptUnix()
{
    return realAccept();
}

int Socket::realAccept()
{
    socklen_t len;
    if(inOrUn)
    {
        struct sockaddr_in temp_addr;
        memset(&temp_addr,0,sizeof(temp_addr));
        return_fd=accept(socket_fd,(struct sockaddr *)(&temp_addr),&len);
        return return_fd;
    }
    else
    {
        struct sockaddr_un temp_addr;
        memset(&temp_addr,0,sizeof(temp_addr));
        return_fd=accept(socket_fd,(struct sockaddr *)(&temp_addr),&len);
        return return_fd;
    }
    
}

int Socket::Read(int fd,void * buf, int len)
{
    printf("waiting read sockfd: %d\n",fd);
    return read(fd,buf,len);
}

int Socket::Write(int fd,const void * buf,int len)
{
    printf("writing sockfd: %d\n",fd);
    return write(fd,buf,len);
}

//ssize_t nr = ::recvfrom(sock.fd(), &message, sizeof message, 0, &peerAddr, &addrLen);
int Socket::Recvfrom(void * buf, int len,struct sockaddr_in & peerAddr)
{
    //struct sockaddr peerAddr;
    bzero(&peerAddr, sizeof peerAddr);
    //socklen_t addrLen = sizeof(peerAddr);
    return realRecvfrom(buf,len,(struct sockaddr *)&peerAddr);
}

int Socket::Recvfrom(void * buf, int len,struct sockaddr_un & peerAddr)
{
    //struct sockaddr peerAddr;
    bzero(&peerAddr, sizeof peerAddr);
    //socklen_t addrLen = sizeof(peerAddr);
    //return recvfrom(socket_fd,buf,len,0,(struct sockaddr *)&peerAddr,&addrLen);
    return realRecvfrom(buf,len,(struct sockaddr *)&peerAddr);
}
int Socket::realRecvfrom(void * buf, int len,struct sockaddr * peerAddr)
{
    socklen_t addrLen = sizeof(peerAddr);
    recvfrom(socket_fd,buf,len,0,peerAddr,&addrLen);
}

//sendto(sock.fd(), &message, sizeof message, 0, &peerAddr, addrLen);
int Socket::Sendto(const void * buf,int len,struct sockaddr_in & addr)
{
    return realSendto(buf,len,(struct sockaddr *)&addr);
}

int Socket::Sendto(const void * buf,int len,struct sockaddr_un & addr)
{
    return realSendto(buf,len,(struct sockaddr *)&addr);
}

int Socket::realSendto(const void * buf,int len,struct sockaddr * addr)
{
    socklen_t addrLen = sizeof(addr);
    return sendto(socket_fd, buf, len,0,addr,addrLen);
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