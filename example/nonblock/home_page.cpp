#include "./web.h"
#include <iostream>

using namespace std;

extern struct file file[MAXFILES];

extern int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
extern fd_set rset, wset;

void home_page(const char * host, const char * fname)
{
    int fd, n;
    char line[MAXLINE];
    helpNet hp;
    fd = hp.Tcp_connect(host, "8080");//不仅支持主机和端口号进行连接，也支持主机和服务名来进行连接。
    n = snprintf(line, sizeof(line), GET_CMD, fname);//拼接请求。
    write(fd, line, n);
    for(;;)
    {
        if((n=read(fd, line, MAXLINE)) == 0)
            break;
        cout<<"read "<<n<<" bytes of home page"<<endl;
    }
    cout<<"end-of-file on home page"<<endl;
    close(fd);
}