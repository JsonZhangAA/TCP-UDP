#include "../../chaptr11lib.h"
#include <iostream>

using namespace std;

#define MAX_LEN 100

int main(int argc, char * * argv)
{
    if(argc<3)
    {
        cout<<"error: argc<3"<<endl;
        return 0;
    }

    helpNet * hp;
    int fd=hp->Tcp_connect(argv[1],argv[2]);
    if(fd==-1)
    {
        cout<<"error: host or serv is error"<<endl;
        return 0;
    }

    int buf;
    int nw=read(fd,&buf,MAX_LEN);
    if(nw<0)
    {
        cout<<"error: read failed"<<endl;
        return 0;
    }

    cout<<"server time: "<<buf<<endl;

    return 0;
}