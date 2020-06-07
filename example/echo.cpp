#include "../Socket.h"
#include <iostream>
#include <thread>

using namespace std;

#define MAXSIZE 100

void server_thread(Socket * ssocket,int fd)
{
    while(1)
    {
        char buf[100]="error";
        int nw=ssocket->Read(fd,buf,MAXSIZE);
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
        
        ssocket->Write(fd,buf,nw);
    }
}

void runServer()
{
    Socket * ssocket =new Socket(1,1234);
    ssocket->Setsockopt();
    ssocket->Bind();
    ssocket->Listen();
    while(1)
    {
        ssocket->Accept();
        thread thread_obj(server_thread,ssocket,ssocket->get_return_fd());
        thread_obj.detach();
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
        csocket->Write(csocket->get_socket_fd(),buf,MAXSIZE);
        cout<<"write fin"<<endl;
        csocket->Read(csocket->get_socket_fd(),buf,MAXSIZE);
        cout<<"read fin"<<endl;
        cout<<"server: "<<buf<<endl;
        
    }
}

int main(int argc,char * * argv)
{
    if(argc<2)
    {
        cout<<"error: argc <2"<<endl;
    }
    for(int i=0;i<argc;i++)
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