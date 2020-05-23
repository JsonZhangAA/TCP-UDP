#include "../Socket.h"
#include <time.h>
#include <iostream>
//端口默认为1234

void runServer()
{
    Socket * ssocket=new Socket(true,1234);
    ssocket->Setsockopt();
    ssocket->Bind();
    ssocket->Listen();
    while(1)
    {
        ssocket->Accept();
        int server_time=clock();
        std::cout<<"server_time: "<<server_time<<std::endl;
        int nw=ssocket->Write(&server_time,sizeof(server_time));
        if(nw==-1)
        {
            printf("write failed\n");
        }
    }
}

void runCustomer()
{
    Socket * csocket=new Socket(true,1234);
    csocket->Connect();
    int server_time=0;
    int nw=csocket->Read(&server_time,sizeof(server_time));
    if(nw==-1)
    {
        printf("read failed\n");
    }
    std::cout<<"server_time: "<<server_time<<std::endl;
}

int main(int argc,char * * argv)
{
    int flag=0;//0运行服务端部分，1表示客户端部分
    for(int i=0;i<argc;i++)
    {
        if(strcmp(argv[i],"-s"))
        {
            flag=0;
        }
        if(strcmp(argv[i],"-c"))
        {
            flag=1;
        }
    }
    if(flag)
    {
        runServer();
    }
    else
    {
        runCustomer();
    }
    
    return 0;
}