#include "./pthread08.h"

static int nthreads;

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
    int i,listenfd,connfd;
    socklen_t addrlen,clilen;
    struct sockaddr * cliaddr;

    helpNet hp;
    listenfd=hp.Tcp_listen(argv[1],argv[2]);
    cliaddr=(struct sockaddr *)malloc(sizeof(struct sockaddr));
    iget=iput=0;
    nthreads=atoi(argv[argc-1]);
    tptr=(Thread *)calloc(nthreads,sizeof(Thread));
    cout<<"make "<<nthreads<<" threads"<<endl;
    for(i=0;i<nthreads;i++)
        thread_make(i);
    cout<<"thread make end"<<endl;
    signal(SIGINT,sig_int);
    for(;;)
    {
        clilen=addrlen;
        connfd=accept(listenfd,cliaddr,&clilen);
        if(connfd<0)
            continue;

        pthread_mutex_lock(&clifd_mutex);
        clifd[iput]=connfd;
        if(++iput==MAXNCLI)
            iput=0;
        cout<<"iget: "<<iget<<" iput: "<<iput<<" connfd: "<<connfd<<endl;
        if(iput==iget)
        {
            cout<<"error: iput=iget="<<iput<<endl;
            return 0;
        }
        pthread_cond_signal(&clifd_cond);//唤醒对应的子线程
        pthread_mutex_unlock(&clifd_mutex);
    }
}