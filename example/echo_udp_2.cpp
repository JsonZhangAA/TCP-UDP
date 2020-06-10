#include "../Socket.h"
#include <iostream>

using namespace std;

#define MAX_LEN 100

void runServer()
{
    Socket * ssocket = new Socket(0,1234);
    ssocket->Bind();
    char buf[100];
    
    while(1)
    {
        struct sockaddr_in  csockaddr;
        int nw = ssocket->Recvfrom(buf,MAX_LEN,csockaddr);
        if(nw==-1)
        {
            cout<<"error: recvfrom failed"<<endl;
            break;
        }
        cout<<"customer: "<<buf<<endl;
        nw = ssocket->Sendto(buf,nw,csockaddr);
        if(nw==-1)
        {
            cout<<"error: sendto failed"<<endl;
            break;
        }
    }
}

void runCustomer()
{
    Socket * csocket = new Socket(0,1234);
    csocket->Connect();
    char buf[MAX_LEN];

    while(1)
    {
        cin>>buf;
        int nw = csocket->Write(csocket->get_socket_fd(),buf,MAX_LEN);
        if(nw<0)
        {
            cout<<"error: write failed"<<endl;
        }
        nw=csocket->Read(csocket->get_socket_fd(),buf,MAX_LEN);
        if(nw<0)
        {
            cout<<"error: read failed\n"<<endl;
        }
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