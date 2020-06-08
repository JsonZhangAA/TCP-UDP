#include "../Socket.h"
#include <iostream>

using namespace std;
#define MAX_USER 10
#define MAX_LEN 100

void runServer()
{
    Socket * ssocket=new Socket(true,1234);
    ssocket->Bind();
    ssocket->Listen();

    fd_set rset,sset;
    int clients[MAX_USER],maxI=0;
    char buf[MAX_LEN];

    FD_ZERO(&rset);
    FD_ZERO(&sset);
    for(int i=0;i<MAX_USER;i++)
    {
        clients[i]=-1;
    }

    int fd_nums=ssocket->get_socket_fd();
    FD_SET(ssocket->get_socket_fd(),&rset);

    while(1)
    {
        sset=rset;
        int read_num=select(fd_nums+1,&sset,NULL,NULL,NULL);
        cout<<"read_num: "<<read_num<<endl;
        if(FD_ISSET(ssocket->get_socket_fd(),&sset))//socket_fd ready
        {
            int ret=ssocket->Accept();
            if(ret<0)
                continue;
            FD_SET(ssocket->get_return_fd(),&rset);
            if(ssocket->get_return_fd()>fd_nums)
            {
                fd_nums=ssocket->get_return_fd();
            }  
            for(int i=0;i<MAX_USER;i++)
            {
                if(clients[i]<0)
                {
                    clients[i]=ssocket->get_return_fd();
                    if(maxI<i+1)
                    {
                        maxI=i+1;
                    }
                    if(i==MAX_USER)
                    {
                        cout<<"error: big user no: "<<ssocket->get_return_fd()<<endl;
                        continue;
                    }
                    cout<<"client: "<<ssocket->get_return_fd()<<" coming in"<<endl;
                    break;
                }
            }
        }
        for(int i=0;i<maxI;i++)
        {
            if(FD_ISSET(clients[i],&sset))
            {
                int nw=ssocket->Read(clients[i],buf,MAX_LEN);
                if(nw==0)
                {
                    shutdown(clients[i],SHUT_WR);
                    close(clients[i]);
                    clients[i]=-1;
                    FD_CLR(clients[i],&rset);
                }
                else if(nw==-1)
                {
                    cout<<"read failed"<<endl;
                    close(clients[i]);
                    clients[i]=-1;
                    FD_CLR(clients[i],&rset);
                }
                else
                {
                    ssocket->Write(clients[i],buf,MAX_LEN);
                }
                
            }
        }

    }
}

void runCustomer()
{
    Socket * csocket= new Socket(1,1234);
    csocket->Connect();
    char buf[100]="hello";
    while(1)
    {
        cout<<"customer: ";
        cin>>buf;
        if(strcmp(buf,"end")==0)
        {
            break;
        }
        csocket->Write(csocket->get_socket_fd(),buf,MAX_LEN);
        cout<<"write fin"<<endl;
        csocket->Read(csocket->get_socket_fd(),buf,MAX_LEN);
        cout<<"read fin"<<endl;
        cout<<"server: "<<buf<<endl;
        
    }
}

int main(int argc, char * * argv)
{
    if(argc<2)
    {
        cout<<"error: argc<2"<<endl;
        return 0;
    }
    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-c")==0)
        {
            runCustomer();
        }
        if(strcmp(argv[i],"-s")==0)
        {
            runServer();
        }
    }
    return 0;
}