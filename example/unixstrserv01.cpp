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

    Socket * ssocket=new Socket(true,argv[1]);
    unlink(argv[1]);
    ssocket->BindUnix();
    ssocket->Listen();
    Signal(SIGCHLD,sig_chld);
    
    while(1)
    {
        if(ssocket->AcceptUnix()<0)
        {
            continue;
        }

        if((childpid=fork())==0)//为每个客户端分配一个进程进行读写。
        {
            //close(ssocket->get_socket_fd());
            while(1)
            {
                char buf[MAXSIZE]="error";
                int nw=ssocket->Read(ssocket->get_return_fd(),buf,MAXSIZE);
                cout<<"nw: "<<nw<<endl;
                if(nw==-1)
                {
                    cout<<"read failed,nw: "<<nw<<endl;
                    break;
                }  
                else if(nw==0)
                {
                    cout<<"read finished"<<endl;
                    break;
                }
                cout<<"customer: "<<buf<<endl;
                
                ssocket->Write(ssocket->get_return_fd(),buf,nw);
            }
            exit(0);
        }
    }
    return 0;
}