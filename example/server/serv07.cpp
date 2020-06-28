#include "./pthread07.h"
#include <iostream>

using namespace std;

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
    int i;
    helpNet hp;
    listenfd=hp.Tcp_listen(argv[1],argv[2]);

    nthreads=atoi(argv[argc-1]);
    tptr=(Thread *)calloc(nthreads,sizeof(Thread));
    cout<<"make "<<nthreads<<" threads"<<endl;

    for(i=0;i<nthreads;i++)
        thread_make(i);
    
    cout<<"thread make end"<<endl;
    
    signal(SIGINT,sig_int);
    for(;;)
        pause();
}