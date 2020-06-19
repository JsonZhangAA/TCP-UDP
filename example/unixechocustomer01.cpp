#include "../Socket.h"
#include <iostream>

using namespace std;
#define MAXSIZE 100

int main(int argc,char * * argv)
{
    Socket * csocket=new Socket(false,tmpnam(NULL));
    //csocket->ConnectUnix();
    csocket->BindUnix();//显示的调用bind，并且要指定与之绑定的路径
    char buf[MAXSIZE]="hello";
    while(1)
    {
        cin>>buf;
        struct sockaddr_un ssocketaddr;
        bzero(&ssocketaddr,sizeof(ssocketaddr));
        ssocketaddr.sun_family=AF_LOCAL;
        strcpy(ssocketaddr.sun_path,argv[1]);
        int nw = csocket->Sendto(buf,MAXSIZE,ssocketaddr);//UDP对应的方法
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
    return 0;
}