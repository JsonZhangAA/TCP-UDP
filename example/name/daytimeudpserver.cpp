#include "../../chaptr11lib.h"
#include <iostream>
#include <time.h>

using namespace std;
#define MAX_LEN 100

int main(int argc, char * * argv)
{
    if(argc<3)
    {
        cout<<"error: argc<3"<<endl;
        return 0;
    }

    helpNet hp;
    socklen_t len;
    struct  sockaddr_storage cliaddr;
    int fd=hp.Udp_server(argv[1],argv[2],&len);
    if(fd<0)
    {
        cout<<"error: host or server is error"<<endl;
        return 0;
    }
    while(1)
    {
        int server_time;
        int nw=recvfrom(fd,&server_time,sizeof(server_time),0,(struct sockaddr *)&cliaddr,&len);
        cout<<"client time: "<<server_time<<endl;
        
        server_time=clock();
        sendto(fd,&server_time,sizeof(server_time),0,(struct sockaddr *)&cliaddr,len);
    }
    return 0;
}