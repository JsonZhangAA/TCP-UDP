#include "child02.h"
#include "../../Socket.h"

static int nchildren;

//extern ssize_t Write_fd(int fd, void *ptr, size_t nbytes, int sendfd);
extern void pr_cpu_time();

void sig_int(int signo)
{
    int i;
    if(errno!=ECHILD)
    {
        cout<<"wait error"<<endl;
        return;
    }

    pr_cpu_time();
    exit(0);
}

int main(int argc,char * * argv)
{
    int listenfd,i,navail,maxfd,nsel,connfd,rc;
    ssize_t n;
    fd_set rset,masterset;
    socklen_t addrlen,clilen;
    struct sockaddr *cliaddr;

    helpNet hp;
    listenfd=hp.Tcp_listen(argv[1],argv[2]);

    FD_ZERO(&masterset);
    FD_SET(listenfd,&masterset);
    maxfd=listenfd;
    cliaddr=(struct sockaddr *)malloc(sizeof(struct sockaddr));

    nchildren=atoi(argv[argc-1]);
    navail=nchildren;
    cptr=(Child *)calloc(nchildren,sizeof(Child));

    for(i=0;i<nchildren;i++)
    {
        child_make(i,listenfd,addrlen);
        FD_SET(cptr[i].child_pipefd,&masterset);
        maxfd=max(maxfd,cptr[i].child_pipefd);
    }

    signal(SIGINT,sig_int);
    for(;;)
    {
        rset=masterset;
        if(navail<=0)
            FD_CLR(listenfd,&rset);
        nsel=select(maxfd+1,&rset,NULL,NULL,NULL);//select子进程对应的管道和服务器的监听字节符
        if(FD_ISSET(listenfd,&rset))
        {
            clilen=addrlen;
            connfd=accept(listenfd,cliaddr,&clilen);

            for(i=0;i<nchildren;i++)
            {
                if(cptr[i].child_status==0)
                    break;
            }

            if(i==nchildren)
            {
                cout<<"error: no available children"<<endl;
                return 0;
            }
            cptr[i].child_status=1;
            cptr[i].child_count++;
            navail--;

            n=write(cptr[i].child_pipefd,"",1);//唤醒对应的子进程
            close(connfd);
            if(--nsel==0)
                continue;
        }
        for(i=0;i<nchildren;i++)//处理已经处于空闲的子线程
        {
            if(FD_ISSET(cptr[i].child_pipefd,&rset))
            {
                if((n==read(cptr[i].child_pipefd,&rc,1))==0)
                {
                    cout<<"error: child "<<i<<" terminated unexpectedly"<<endl;
                    return 0;
                }
                cptr[i].child_status=0;//表示当前的子线程空闲了。
                navail++;
                if(--nsel==0)
                    break;
            }
        }
    }
}