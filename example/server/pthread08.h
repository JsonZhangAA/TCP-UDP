#include "../../Socket.h"
#include <iostream>
#include <pthread.h>
#include "child02.h"

using namespace std;

typedef struct 
{
    pthread_t thread_tid;
    long thread_count;
}Thread;

Thread * tptr;

#define MAXNCLI 32
int clifd[MAXNCLI],iget,iput;
pthread_mutex_t clifd_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond=PTHREAD_COND_INITIALIZER;
//pthread_cond_t ndone_cond=PTHREAD_COND_INITIALIZER;
//pthread_cond_t clifd_cond=PTHREAD_COND_INITIALIZE;

void * thread_main1(void * arg)
{
    int connfd;
    printf("thread %d starting\n",(int)arg);
    for(;;)
    {
        pthread_mutex_lock(&clifd_mutex);
        while(iget==iput)
            pthread_cond_wait(&clifd_cond,&clifd_mutex);//使用了信号量，同样也是有主线程负责唤醒那些子线程。这种效率是否会更高了，我觉得见仁见智吧。
        connfd=clifd[iget];
        if(++iget==MAXNCLI)
            iget=0;
        pthread_mutex_unlock(&clifd_mutex);
        tptr[(int)arg].thread_count++;
        web_child(connfd);
        close(connfd);
    }
}

void thread_make(int i)
{
    //void * thread_main(void *);
    cout<<"i: "<<i<<endl;
    pthread_create(&tptr[i].thread_tid,NULL,&thread_main1,(void *)i);
  //pthread_create(&tptr[i].thread_tid,NULL,&thread_main,(void *)i);
    return;
}


