#include "str.h"
#include "../../chaptr11lib.h"

static void * doit(void *);

int main(int argc, char **argv)
{
    int listenfd, * iptr;
    pthread_t tid;
    socklen_t addrlen, len;
    struct sockaddr * cliaddr;
    Socket * ssocket=new Socket(true,1234);
    ssocket->Bind();
    ssocket->Listen();
    while(1)
    {
        ssocket->Accept();
        iptr=(int *)malloc(sizeof(int));
        *iptr=ssocket->get_return_fd();
        pthread_create(&tid,NULL,doit,(void *)iptr);
    }
}

void str_echo(int connfd)
{
    char buf[MAXLINE];
    int nw=read(connfd,buf,MAXLINE);
    cout<<"client: "<<buf<<", nw:"<<nw<<endl;
    write(connfd,buf,MAXLINE);
}

static void * doit(void * arg)
{
    int connfd;

    connfd=*((int *)arg);
    free(arg);

    pthread_detach(pthread_self());//分离线程，这样线程运行结束后会自动释放资源。
    str_echo(connfd);
    close(connfd);
    return NULL;
}