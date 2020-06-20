#include "web.h"

extern struct file file[MAXFILES];
extern int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
extern fd_set rset, wset;

struct addrinfo * Host_serv(const char * host, const char * serv, int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = family;
    hints.ai_socktype = socktype;

    if((n = getaddrinfo(host, serv, &hints, &res))!=0)
        return(NULL);
    
    return res;
}

void start_connect(struct file * fptr)//为每个file客户端启动一个连接
{
    int fd, flags, n;
    struct addrinfo * ai;

    ai = Host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
    if(ai==NULL)
    {
        cout<<"Host_serv failed!!!"<<endl;
    }
    /*
    Socket * ssocket = new Socket(true,"1234",((struct sockaddr_in)(ai->ai_addr)).sin_addr);
    fd=ssocket->get_socket_fd();
    */
    fd=socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    fptr->f_fd = fd;
    printf("start_connect for %s, fd %d\n", fptr->f_name, fd);

    flags = fcntl(fd, F_SETFL, flags|O_NONBLOCK);

    if((n = connect(fd, ai->ai_addr, ai->ai_addrlen))<0)//connect成功，则返回0；失败则返回-1.
    {
        if(errno != EINPROGRESS)//EINPROGRESS表示连接还在进行，非阻塞IO特有。
        {
            cout<<"nonblocking connect error"<<endl;
            return;
        }
        fptr->f_flags = F_CONNECTING;
        FD_SET(fd, &rset);
        FD_SET(fd, &wset);
        if(fd > maxfd)
            maxfd = fd;
    } else if(n>=0)
        write_get_cmd(fptr);//连接成功，发送GET请求。
}