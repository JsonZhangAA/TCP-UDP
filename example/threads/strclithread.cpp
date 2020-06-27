#include "./str.h"

void *copyto(void *);

static int sockfd;
static FILE *fp;

void str_cli(FILE *fp_arg, int sockfd_arg)
{
    char recvline[MAXLINE];
    pthread_t tid;

    sockfd=sockfd_arg;
    fp=fp_arg;
    pthread_create(&tid,NULL,copyto,NULL);
    while(read(sockfd,recvline,MAXLINE)>0)
        fputs(recvline,stdout);
}

void * copyto(void * arg)
{
    char sendline[MAXLINE];
    while(fgets(sendline,MAXLINE,fp)!=NULL)
    {
        cout<<"sendline: "<<sendline<<endl;
        write(sockfd,sendline,strlen(sendline));
    }
    shutdown(sockfd,SHUT_WR);
    return(NULL);
}