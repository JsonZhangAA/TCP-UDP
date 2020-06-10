#include "../Socket.h"
#include <iostream>

using namespace std;
#define MAX_LEN 100

void runServer()
{
    Socket * ssocket_u = new Socket(0,1234);
    ssocket_u->Bind();

    Socket * ssocket_t = new Socket(1,1234);
    ssocket_t->Setsockopt();
    ssocket_t->Bind();
    ssocket_t->Listen();

    fd_set rset, sset;
    FD_ZERO(&rset);
    FD_ZERO(&sset);

    FD_SET(ssocket_t->get_socket_fd(),&rset);
    FD_SET(ssocket_u->get_socket_fd(),&rset);

    int maxfd=ssocket_u->get_socket_fd()>ssocket_t->get_socket_fd()?ssocket_u->get_socket_fd():ssocket_t->get_socket_fd();

    while(1)
    {
        sset=rset;
        int nw = select(maxfd+1,&sset,NULL,NULL,NULL);

        //tcp fd ready
        if(FD_ISSET(ssocket_t->get_socket_fd(),&sset))
        {
            ssocket_t->Accept();
            char buf[MAX_LEN];
            int nw = ssocket_t->Read(ssocket_t->get_return_fd(),buf,MAX_LEN);
            if(nw<0)
            {
                cout<<"error: read failed"<<endl;
                break;
            }
            cout<<"tcp customer: "<<buf<<endl;

            nw = ssocket_t->Write(ssocket_t->get_return_fd(),buf,nw);
            if(nw<0)
            {
                cout<<"error: write failed"<<endl;
                break;
            }
        }

        if(FD_ISSET(ssocket_u->get_socket_fd(),&sset))
        {
            char buf[MAX_LEN];
            struct sockaddr_in ssocket_u_addr;
            int nw = ssocket_u->Recvfrom(buf,MAX_LEN,ssocket_u_addr);
            if(nw<0)
            {
                cout<<"error: recvfrom failed"<<endl;
                break;
            }
            cout<<"udp customer: "<<buf<<endl;

            nw = ssocket_u->Sendto(buf,MAX_LEN,ssocket_u_addr);
            if(nw<0)
            {
                cout<<"error: sendto failed"<<endl;
                break;
            }
        }
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
        if(strcmp(argv[i],"-s")==0)
        {
            runServer();
        }
    }
    return 0;
}