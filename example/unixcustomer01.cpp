#include "../Socket.h"
#include <iostream>

using namespace std;
#define MAXSIZE 100

int main(int argc,char * * argv)
{
    Socket * csocket=new Socket(true,argv[1]);
    csocket->ConnectUnix();//注意，这里使用的字节流域，所以不用像数据报域一样，显示的调用bind
    char buf[MAXSIZE]="hello";
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
    return 0;
}