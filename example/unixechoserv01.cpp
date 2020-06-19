#include "../Socket.h"
#include <iostream>

using namespace std;
#define MAXSIZE 100

Sigfunc * Signal(int signo, Sigfunc * func)
{
    struct sigaction act, oact;
    act.sa_handler=func;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    if(sigaction(signo, &act, &oact)<0)
        return (SIG_ERR);
    return (oact.sa_handler);
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while((pid=waitpid(-1,&stat,WNOHANG))>0)
        cout<<"child "<<pid<<" terminated"<<endl;
}

int main(int argc,char * * argv)
{
    pid_t childpid;
    void sig_chld(int);

    Socket * ssocket=new Socket(false,argv[1]);
    unlink(argv[1]);
    ssocket->BindUnix();
    Signal(SIGCHLD,sig_chld);
    char buf[100];
    
    while(1)
    {
        struct sockaddr_un  csockaddr;
        int nw = ssocket->Recvfrom(buf,MAXSIZE,csockaddr);
        if(nw==-1)
        {
            cout<<"error: recvfrom failed"<<endl;
            break;
        }
        cout<<"customer: "<<buf<<endl;

        struct sockaddr_un addr2;
        socklen_t len;
        getsockname(ssocket->get_socket_fd(),(struct sockaddr *)&addr2,&len);
        printf("bound name=%s, returned len=%d\n",addr2.sun_path,len);

        nw = ssocket->Sendto(buf,nw,csockaddr);
        if(nw==-1)
        {
            cout<<"error: sendto failed"<<endl;
            break;
        }
    }
    return 0;
}