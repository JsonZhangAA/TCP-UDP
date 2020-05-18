#include <iostream>
#include "../Socket.h"
#include <thread>
#include <time.h>

using namespace std;

int createSocket(int port)
{
    Socket * tsocket=new Socket(true,port);
    
    int yes = 1;
    //打开了地址复用功能
    if (setsockopt(tsocket->get_socket_fd(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
    {
        perror("setsockopt");
        exit(1);
    }
    
    tsocket->Bind();

    tsocket->Listen();
    tsocket->Accept();
    close(tsocket->get_socket_fd());
    return tsocket->get_return_fd();
}

void customer(int port,int len)
{
    Socket * csocket=new Socket(true,port);
    csocket->Connect();
    printf("connect socketFd: %d\n",csocket->get_socket_fd());

    for(int i=0;i<len;i++)
    {
        //客户端向服务端写入数据
        //write(socketFd,"hello world",100);
        csocket->Write("hello world",100);
    }

    //close(socketFd);
}

void server_thread(int socketFd)
{
    printf("server: socketFd: %d\n",socketFd);
    char buff[100];
    int len;
    clock_t begin=clock();
    for(int i=0;i<5;i++)
    {
        len=read(socketFd,buff,100);
        //printf("buff: %s\n",buff);
    }
    close(socketFd);
    clock_t end=clock();
    float sizes=(1.0*len*65535)/1024/1024;
    printf("read data: %0.3f\n",sizes);
    cout<<"time: "<<(end-begin)<<" ms"<<endl;
    printf("read data: %0.3f Mib/s\n",sizes/((end-begin)/1000));
}

void server(int port)
{
    //server_thread(port);
    while (1)
    {
        int socketFd=createSocket(port);
        if(socketFd==-1)
            return;
        thread thread_obj(server_thread,socketFd);
        thread_obj.detach();
    } 
}

int main(int argc, char * * argv)
{
    int port=5001,len=5;
    bool flag=true;
    for(int i=1;i<argc;i++)
    {
        if(!strcmp(argv[i],"-p"))
        {
            port=atoi(argv[++i]);
            continue;
        }
        if(!strcmp(argv[i],"-s"))
        {
            flag=true;
            continue;
        }
        if(!strcmp(argv[i],"-c"))
        {
            flag=false;
            continue;
        }
        if(!strcmp(argv[i],"-l"))
        {
            len=atoi(argv[++i]);
            continue;
        }
    }
    if(flag)
    {
        server(port);
    }
    else
    {
        customer(port,len);
    }
    return 0;
}