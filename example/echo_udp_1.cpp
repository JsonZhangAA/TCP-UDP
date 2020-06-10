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
    char buf[MAX_LEN];
    while(1)
    {
        cin>>buf;
        struct sockaddr_in ssocketaddr=csocket->get_addr();
        int nw = csocket->Sendto(buf,MAX_LEN,ssocketaddr);
        if(nw==-1)
        {
            cout<<"error: sendto failed"<<endl;
            break;
        }
        nw = csocket->Recvfrom(buf,nw,ssocketaddr);
        if(nw==-1)
        {
            cout<<"error: recvfrom failed"<<endl;
            break;
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