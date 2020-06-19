#include "../Socket.h"
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
    int sockfd;
    socklen_t len;
    struct sockaddr_un addr2;
    if(argc!=2)
    {
        cout<<"error: argc!=2"<<endl;
        return 0;
    }

    Socket * ssocket=new Socket(true,argv[1]);//指定unix路径
    sockfd=ssocket->get_socket_fd();
    unlink(argv[1]);//如果该路径被使用了，就将其释放掉

    ssocket->BindUnix();//将套接字和路径进行绑定

    len=sizeof(addr2);
    getsockname(sockfd,(struct sockaddr *)&addr2,&len);
    printf("bound name=%s, returned len=%d\n",addr2.sun_path,len);
    return 0;
}