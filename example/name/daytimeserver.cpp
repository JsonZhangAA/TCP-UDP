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

    struct sockaddr_storage cliaddr;
    helpNet hp;
    int fd= hp.Tcp_listen(argv[1],argv[2]);
    if(fd==-1)
    {
        cout<<"error: host or serv is error"<<endl;
        return 0;
    }
    while(1)
    {
        socklen_t len;
        int return_fd=accept(fd,(struct sockaddr *)&cliaddr,&len);
        int buf=clock();
        int nw=write(return_fd,&buf,sizeof(buf));
        if(nw<0)
        {
            cout<<"write failed\n"<<endl;
            return 0;
        }
        cout<<"server time: "<<buf<<endl;
    }
    return 0;
}