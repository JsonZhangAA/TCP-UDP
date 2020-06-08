#include "../Socket.h"
#include <iostream>
#include <poll.h>

using namespace std;

#define MAX_USER 10
#define MAX_LEN 100
#define INFTIM -1

void runServer()
{
    char buf[MAX_LEN];
    Socket * ssocket = new Socket(1,1234);
    ssocket->Bind();
    ssocket->Listen();

    struct pollfd clients[MAX_USER];
    clients[0].fd=ssocket->get_socket_fd();
    clients[0].events=POLLRDNORM;
    for(int i=1;i<MAX_USER;i++)
    {
        clients[i].fd=-1;
    }
    int maxi=0;
    while(1)
    {
        int read_num=poll(clients,maxi+1,INFTIM);
        if(clients[0].revents==POLLRDNORM)
        {
            ssocket->Accept();
            for(int i=1;i<MAX_USER;i++)
            {
                if(clients[i].fd==-1)
                {
                    clients[i].fd=ssocket->get_return_fd();
                    clients[i].events=POLLRDNORM;
                    if(maxi<i)
                    {
                        maxi=i;
                    }
                    break;
                }
            }
        }

        for(int i=1;i<=maxi;i++)
        {
            if(clients[i].fd==-1)
                continue;
            if(clients[i].revents&POLLRDNORM)
            {
                int nw=ssocket->Read(clients[i].fd,buf,MAX_LEN);
                if(nw==0)
                {
                    shutdown(clients[i].fd,SHUT_WR);
                    close(clients[i].fd);
                    clients[i].fd=-1;
                }
                else if(nw==-1)
                {
                    cout<<"read failed, no: "<<clients[i].fd<<endl;
                    close(clients[i].fd);
                    clients[i].fd=-1;
                }
                else
                {
                    ssocket->Write(clients[i].fd,buf,MAX_LEN);
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