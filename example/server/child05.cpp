#include "./child02.h"

//extern ssize_t Read_fd(int fd, void *ptr, size_t nbytes, int *recvfd)

void web_child(int sockfd)
{
    char buf[MAXLINE],result[MAXN]="testetstetet";
    cout<<"ehllo"<<endl;
    for(;;)
    {
        int nw=read(sockfd,buf,MAXLINE);
        if(nw<=0)
        {
            return;
        }
        printf("child %ld working\n",(long)getpid());
        write(sockfd,result,nw);
    }
}


pid_t child_make(int i,int listenfd,int addrlen)
{
    int sockfd[2];
    pid_t pid;
    void child_main(int,int,int);

    socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd);//创建全双工的无名管道
    if((pid=fork())>0)
    {
        close(sockfd[1]);//关闭管道的一段
        cptr[i].child_pid=pid;
        cptr[i].child_pipefd=sockfd[0];
        cptr[i].child_status=0;
        return pid;
    }

    dup2(sockfd[1],STDERR_FILENO);//重定向
    close(sockfd[0]);
    close(sockfd[1]);
    close(listenfd);
    child_main(i,listenfd,addrlen);
}

void child_main(int i,int listenfd,int addrlen)
{
    char c;
    int connfd;
    ssize_t n;
    void web_child(int);

    printf("child %ld starting\n",(long)getpid());
    for(;;)
    {
        if((n=read(STDERR_FILENO,&c,1))==0)//阻塞在read上，由父进程来处理，避免为每个子进程加锁带来的开销。
        {
            cout<<"error: read failed"<<endl;
            return;
        }
        if(connfd<0)
        {
            cout<<"error:connfd<0"<<endl;
            return;
        }
        web_child(connfd);
        close(connfd);
        write(STDERR_FILENO,"",1);//告诉主线程，本线程已经空闲了。
    }
}