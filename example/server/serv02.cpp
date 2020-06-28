//#include "../../Socket.h"
#include "child02.h"
#include "lock_pthread.h"

using namespace std;

//static pthread_mutex_t *mptr;
static int nchildren;
static pid_t *pids;

int main(int argc, char * * argv)
{
    int listenfd,i;
    socklen_t addrlen;
    void sig_int(int);
    pid_t child_make(int,int,int);

    helpNet hp;
    listenfd=hp.Tcp_listen(argv[1],argv[2]);
    cout<<"listenfd: "<<listenfd<<endl;
    nchildren=atoi(argv[argc-1]);
    pids=(pid_t *)calloc(nchildren,sizeof(pid_t));

    my_lock_init("/tmp/lock.XXXXXX"); /* one lock file for all children */
    for(i=0;i<nchildren;i++)
    {
        pids[i]=child_make(i,listenfd,addrlen);
    }

    signal(SIGINT,sig_int);//ctrl+c时，输出相应的系统信息。
    for(;;)
        pause();
}

void sig_int(int signo)
{
    int i;
    void pr_cpu_time(void);

    for(i=0;i<nchildren;i++)
        kill(pids[i],SIGTERM);
    while(wait(NULL)>0);
    if(errno!=ECHILD)
    {
        cout<<"wait error"<<endl;
        return;
    }

    pr_cpu_time();
    exit(0);
}