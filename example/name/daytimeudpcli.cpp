#include "../../chaptr11lib.h"
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char * * argv)
{
    if(argc<3)
    {
        cout<<"error: argc<3"<<endl;
        return 0;
    }

    helpNet hp;
    socklen_t len;
    struct  sockaddr * cliaddr;
    int fd=hp.Udp_client(argv[1],argv[2],&cliaddr,&len);
    if(fd<0)
    {
        cout<<"error: host or server is error"<<endl;
        return 0;
    }

    int server_time=clock();
    int nw=sendto(fd,&server_time,sizeof(server_time),0,cliaddr,len);

    cout<<"sendto "<<nw<<endl;

    recvfrom(fd,&server_time,sizeof(server_time),0,cliaddr,&len);
    cout<<"server_time: "<<server_time<<endl;
    return 0;
}