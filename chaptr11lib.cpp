#include "./chaptr11lib.h"

int helpNet::Tcp_connect(const char * host, const char * serv)
{
    int fd;//存放返回值fd
    struct addrinfo hint, * res, * res1;
    bzero(&hint,sizeof(struct addrinfo));
    hint.ai_family=AF_UNSPEC;
    hint.ai_socktype=SOCK_STREAM;

    if(getaddrinfo(host,serv,&hint,&res)!=0)//通过主机名hots和服务名serv来查找对应的服务器信息，存放到res中。
    {
        printf("error: getaddrinfo failed\n");
        return -1;
    }

    res1=res;//res后是一张链表，保存链头位置，方便释放整个链表

    while(res!=NULL)//一次查询链表上的每个元素，只要有元素connect成功，那么就break出循环。
    {
        fd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(fd<0)
            continue;
        
        if(connect(fd, res->ai_addr,res->ai_addrlen)==0)
            break;
        close(fd);

        res=res->ai_next;
    }

    freeaddrinfo(res1);
    return fd;
}

int helpNet::Tcp_listen(const char * host, const char * serv)
{
    int fd,yes=1;
    struct addrinfo hint, * res, * res1;
     bzero(&hint,sizeof(struct addrinfo));
    hint.ai_family=AF_UNSPEC;
    hint.ai_socktype=SOCK_STREAM;

    if(getaddrinfo(host,serv,&hint,&res)!=0)
    {
        printf("error: getaddrinfo failed\n");
        return -1;
    }

    res1=res;

    while(res!=NULL)
    {
        fd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(fd<0)
        {
            continue;
        }

        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))!=0)//设置地址复用功能
            continue;
        if(bind(fd, res->ai_addr, res->ai_addrlen)==0)
            break;
        close(fd);
        res=res->ai_next;
    }

    listen(fd,5);
    freeaddrinfo(res1);
    return fd;
}

int helpNet::Udp_connect(const char * host, const char * serv,struct sockaddr * * resa, socklen_t * len)
{
    int fd;
    struct addrinfo hint, * res, * res1;
    bzero(&hint,sizeof(struct addrinfo));
    hint.ai_family=AF_UNSPEC;
    hint.ai_socktype=SOCK_DGRAM;

    if(getaddrinfo(host,serv,&hint,&res)!=0)
    {
        printf("error: getaddrinfo failed\n");
        return -1;
    }

    res1=res;

    while(res!=NULL)
    {
        fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(fd<0)
            continue;
        if(connect(fd,res->ai_addr,res->ai_addrlen)==0)//这里的udp使用了connect，所以后续可以使用read和write
            break;
        close(fd);
        res=res->ai_next;
    }
    /*
    resa=malloc(res->ai_addrlen);
    memcpy(resa,res->ai_addr,res->ai_addrlen);
    */
   freeaddrinfo(res1);
   return fd;
}

int helpNet::Udp_client(const char * host, const char * serv,struct sockaddr * * resa, socklen_t * len)
{
    int fd;
    struct addrinfo hint, * res, * res1;
    bzero(&hint,sizeof(struct addrinfo));
    hint.ai_family=AF_UNSPEC;
    hint.ai_socktype=SOCK_DGRAM;

    if(getaddrinfo(host,serv,&hint,&res)!=0)
    {
        printf("error: getaddrinfo failed\n");
        return -1;
    }

    res1=res;

    while(res!=NULL)
    {
        fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(fd<0)
            continue;
        /*
        if(connect(fd,res->ai_addr,res->ai_addrlen)==0)
            break;
        close(fd);
        */
       break;
        res=res->ai_next;
    }
    
    *resa=(struct sockaddr *)malloc(res->ai_addrlen);
    memcpy(*resa,res->ai_addr,res->ai_addrlen);
    *len=res->ai_addrlen;
    
   freeaddrinfo(res1);
   return fd;
}

int helpNet::Udp_server(const char * host, const char * serv, socklen_t * len)
{
    int fd;
    struct addrinfo hint, * res, * res1;
    bzero(&hint,sizeof(struct addrinfo));
    hint.ai_family=AF_UNSPEC;
    hint.ai_socktype=SOCK_DGRAM;

    if(getaddrinfo(host,serv,&hint,&res)!=0)
    {
        printf("error: getaddrinfo failed\n");
        return -1;
    }

    res1=res;

    while(res!=NULL)
    {
        fd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(fd<0)
            continue;
        if(bind(fd,res->ai_addr, res->ai_addrlen)==0)
        {
            break;
        }
        close(fd);
        res=res->ai_next;
    }

    *len=res->ai_addrlen;

    freeaddrinfo(res1);
    return fd;
}