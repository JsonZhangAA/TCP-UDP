#include "../../Socket.h"
#include <pthread.h>
#include "child02.h"

typedef struct 
{
    pthread_t thread_tid;
    long thread_count;
}Thread;

Thread *tptr;

int listenfd,nthreads;
socklen_t addrlen;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;

void thread_make(int i)
{
    void * thread_main(void *);
    pthread_create(&tptr[i].thread_tid,NULL,&thread_main,(void *)i);
    return;
}

void * thread_main(void * arg)
{
    int connfd;
    void web_child(int);
    socklen_t clilen;
    struct  sockaddr * cliaddr;

    cliaddr=(struct  sockaddr *)malloc(addrlen);

    printf("1\n");
    printf("thread %d starting\n",(int)arg);
    printf("2\n");
    for(;;)//死循环，循环利用子线程
    {
        clilen=addrlen;
        pthread_mutex_lock(&mlock);
        connfd=accept(listenfd,cliaddr,&clilen);//为accept加锁，防止惊群效应
        pthread_mutex_unlock(&mlock);
        tptr[(int)arg].thread_count++;

        web_child(connfd);
        close(connfd);
    }
    
}
