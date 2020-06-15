#include "../../Socket.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char * * argv)
{
    struct hostent * hptr;
    struct servent * sptr;
    struct in_addr * * pptr;
    if(argc<3)
    {
        cout<<"error: argc<3"<<endl;
        return 0;
    }

    if((hptr=gethostbyname(argv[1]))==NULL)
    {
        cout<<"error: gethostbyname h_errno: "<<h_errno<<endl;
        return 0;
    }
    else
    {
        pptr=(struct in_addr **)hptr->h_addr_list;
    }

    if((sptr=getservbyname(argv[2],"tcp"))==NULL)
    {
        cout<<"error: h_errno: "<<h_errno<<endl;
        return 0;
    }

    cout<<"argv[2]: "<<argv[2]<<" sptr->s_port: "<<htons(sptr->s_port)<<" sptr->s_name: "<<sptr->s_name<<endl;
    
    while(*pptr!=NULL)
    {
        Socket * csocket = new Socket(true, htons(sptr->s_port), *pptr);
        //Socket * csocket = new Socket(true, 1234, *pptr);
        if(csocket->Connect()==0)
        {
            int server_time=0;
            int nw=csocket->Read(csocket->get_socket_fd(),&server_time,sizeof(server_time));
            if(nw==-1)
            {
                printf("read failed\n");
            }
            std::cout<<"server_time: "<<server_time<<std::endl;
            break;
        }
        pptr++;
    }
    return 0;
}