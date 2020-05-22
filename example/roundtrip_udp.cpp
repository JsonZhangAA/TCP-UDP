#include "../Socket.h"
#include <thread>
#include <time.h>
#include <fcntl.h>
#include <iostream>

struct message
{
    int request;
    int response;
};

void server(int port)
{
    Socket * ssocket=new Socket(false,port);
    ssocket->Setsockopt();
    ssocket->Bind();
    /*
    int flags = fcntl(ssocket->get_socket_fd(), F_GETFL, 0);            //获取文件的flags值。
    fcntl(ssocket->get_socket_fd(), F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；
    */
    while(true)
    {
        struct message msg;
        //::usleep(2000*1000);
        //int nw=ssocket->Read(&msg,sizeof(msg));
        struct sockaddr_in  addr;
        int nw=ssocket->Recvfrom(&msg,sizeof(msg),addr);
        if(nw!=-1)
            printf("recv sockfd: %d, nw:%d done\n",ssocket->get_socket_fd(),nw);
        msg.response=clock();
        //nw=ssocket->Write(&msg,sizeof(msg));
        nw=ssocket->Sendto(&msg,sizeof(msg),addr);
        if(nw!=-1)
            printf("send sockfd: %d,nw:%d,msg.request:%jd, msg.response:%jd done\n",ssocket->get_socket_fd(),nw,msg.request,msg.response);
        std::cout<<"addr.sin_addr.s_addr: "<<addr.sin_addr.s_addr<<" port: "<<addr.sin_port<<std::endl;
    }
}

void customer_thread(Socket * csocket)
{
    while (true)
    {
        struct message msg={0,0};
        msg.request = clock();
        //csocket->Write(&msg,sizeof(msg));
        struct sockaddr_in  addr=csocket->get_addr();
        csocket->Sendto(&msg,sizeof(msg),addr);
        printf("write sockfd:%d msg.request: %jd done\n",csocket->get_socket_fd(),msg.request);
        //close(csocket->get_socket_fd());
        ::usleep(10000*1000);
    }
}
void customer_thread1(Socket * csocket)
{
    while(1)
    {
        struct message rmsg={0,0};
        //printf("before read\n");
        //::usleep(10000*1000);
        int nw=csocket->Read(&rmsg,sizeof(rmsg));
        /*
        struct sockaddr_in  addr=csocket->get_addr();
        int nw=csocket->Recvfrom(&rmsg,sizeof(rmsg),addr);
        */
        if(nw!=-1)
        {
            printf("after read nw: %d msg.request: %jd, msg.response: %jd\n",nw,rmsg.request,rmsg.response);
            int64_t back = clock();
            int64_t mine = (back + rmsg.request) / 2;
            printf("now %jd round trip %jd clock error %jd\n",
                    back, back - rmsg.request, rmsg.response - mine);
        }
    }
}

void customer(int port)
{
    Socket * csocket = new Socket(false,port);
    csocket->Connect();
    /*
    int flags = fcntl(csocket->get_socket_fd(), F_GETFL, 0);            //获取文件的flags值。
    fcntl(csocket->get_socket_fd(), F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；
    */

    printf("connect success\n");
    std::thread thread_obj(customer_thread,csocket);
    //std::thread thread_obj1(customer_thread1,csocket);
    thread_obj.detach();
    //thread_obj1.detach();
    while(1)
    {
        struct message rmsg={0,0};
        //printf("before read\n");
        //::usleep(10000*1000);
        int nw=csocket->Read(&rmsg,sizeof(rmsg));
        /*
        struct sockaddr_in  addr=csocket->get_addr();
        int nw=csocket->Recvfrom(&rmsg,sizeof(rmsg),addr);
        */
        if(nw!=-1)
        {
            printf("after read nw: %d msg.request: %jd, msg.response: %jd\n",nw,rmsg.request,rmsg.response);
            int64_t back = clock();
            int64_t mine = (back + rmsg.request) / 2;
            printf("now %jd round trip %jd clock error %jd\n",
                    back, back - rmsg.request, rmsg.response - mine);
        }
    }
}

int main(int argc,char * * argv)
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
        customer(port);
    }
    return 0;
}