#include "../../Socket.h"
#include "../../chaptr11lib.h"
#include <iostream>

using namespace std;

#define MAXN 16384
#define MAXLINE 4096

int main(int argc, char * * argv)
{
    int i,j,fd,nchildren,nloops,nbytes;
    pid_t pid;
    ssize_t n;
    char request[MAXLINE],reply[MAXN];

    if(argc<6)
    {
        cout<<"error: argc<6"<<endl;
        return 0;
    }

    nchildren=atoi(argv[3]);//表示有多少个子进程
    nloops=atoi(argv[4]);//每个子进程循环的次数
    nbytes=atoi(argv[5]);//这个参数没用到
    snprintf(request,sizeof(request),"%d\n",nbytes);

    for(i=0;i<nchildren;i++)
    {
        if((pid=fork())==0)
        {
            for(j=0;j<nloops;j++)
            {
                helpNet hp;
                fd=hp.Tcp_connect(argv[1],argv[2]);//根据主机名和端口号或服务名来建立TCP连接
                cout<<"fd: "<<fd<<endl;
                write(fd,request,strlen(request));
                /*
                if((n=read(fd,reply,nbytes))!=nbytes)
                {
                    cout<<"server returned "<<n<<" bytes, reply:"<<reply<<endl;
                    return 0;
                }
                */
                close(fd);
            }
            printf("child %d done\n",i);
            exit(0);
        }
    }
    while(wait(NULL)>0);//父进程等待子进程结束，并将参数设置为NULL，表示不保存死掉子进程的任何状态。

    if(errno!=ECHILD)//ECHILD表示未找到要等待的子进程，这个错误应该忽略。
    {
        cout<<"wait error"<<endl;
    }
    exit(0);
}