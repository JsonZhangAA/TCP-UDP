//#include "../../Socket.h"
//#include "./lock_pthread.h"
#include <pthread.h>


extern pthread_mutex_t *mptr;
void my_lock_init(char *pathname);
void my_lock_wait();
void my_lock_release();

#include "child02.h"

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

void child_main(int i,int listenfd,int addrlen)
{
    int connfd;
    //void web_child(int);
    socklen_t clilen;
    struct sockaddr *cliaddr;

    cliaddr=(struct sockaddr *)malloc(addrlen);
    printf("child %ld starting\n",(long)getpid());
    for(;;)//死循环，子进程循环使用
    {
        clilen=addrlen;
        my_lock_wait();
        connfd=accept(listenfd,cliaddr,&clilen);//防止多个子进程都被唤醒，防止进群效应。
        my_lock_release();

        web_child(connfd);//执行对应的操作。
        close(connfd);
    }
}

pid_t child_make(int i,int listenfd,int addrlen)//负责生成子进程。
{
    pid_t pid;
    void child_main(int,int,int);

    if((pid=fork())>0)
        return pid;
    child_main(i,listenfd,addrlen);
}