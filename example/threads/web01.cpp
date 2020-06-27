#include "../../Socket.h"
#include "../../chaptr11lib.h"
#include <iostream>

using namespace std;
#define MAXFILES 20
#define MAXLINE 100
#define SERV "8080"

struct file{
    char * f_name;
    char * f_host;
    int f_fd;
    int f_flag;
    pthread_t f_tid;
}file[MAXFILES];
#define F_CONNECTING 1
#define F_READING 2
#define F_DONE 4

#define F_JOINED 8

int ndone;
pthread_mutex_t ndone_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ndone_cond=PTHREAD_COND_INITIALIZER;


#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn,nfiles,nlefttoconn,nlefttoread;

void * do_get_read(void *);
void home_page(const char *, const char *);
void write_get_cmd(struct file *);

int main(int argc, char * * argv)
{
    int i,n,maxnconn;
    pthread_t tid;
    struct file *fptr;

    if(argc<5)
    {
        cout<<"errno: argc<5"<<endl;
        return 0;
    }
    maxnconn=atoi(argv[1]);

    nfiles=min(argc-4,MAXFILES);
    for(i=0;i<nfiles;i++)
    {
        file[i].f_name=argv[i+4];
        file[i].f_host=argv[2];
        file[i].f_flag=0;
    }
    printf("nfiles=%d\n",nfiles);
    home_page(argv[2],argv[3]);//首先获取主页

    nlefttoread=nlefttoconn=nfiles;
    nconn=0;

    while(nlefttoread>0)
    {
        while(nconn<maxnconn&&nlefttoconn>0)
        {
            for(i=0;i<nfiles;i++)
                if(file[i].f_flag==0)
                    break;
            if(i==nfiles)
            {
                cout<<"nlefttoconn="<<nlefttoconn<<"but nothing found"<<endl;
                return 0;
            }

            file[i].f_flag=F_CONNECTING;
            pthread_create(&tid,NULL,&do_get_read,&file[i]);
            file[i].f_tid=tid;
            nconn++;
            nlefttoconn--;
        }
        
        pthread_mutex_lock(&ndone_mutex);
        while(ndone==0)
            pthread_cond_wait(&ndone_cond,&ndone_mutex);

        for(i=0;i<nfiles;i++)
        {
            if(file[i].f_flag&F_DONE)
            {
                pthread_join(file[i].f_tid,(void * *)&fptr);
            }

            if(&file[i]!=fptr)
            {
                cout<<"file[i]!=fptr"<<endl;
                return 0;
            }
            fptr->f_flag=F_JOINED;
            ndone--;
            nconn--;
            nlefttoread--;
            printf("thread %d for %s done\n",fptr->f_tid,fptr->f_name);
        }
        pthread_mutex_unlock(&ndone_mutex);
    }
    exit(0);
}

void * do_get_read(void * vptr)
{
    int fd,n;
    char line[MAXLINE];
    struct file *fptr;
    helpNet hp;

    fptr=(struct file *)vptr;
    fd=hp.Tcp_connect(fptr->f_host,SERV);
    if(fd<0)
    {
        cout<<"Tcp_connect failed"<<endl;
        return NULL;
    }
    fptr->f_fd=fd;
    printf("do_get_read for %s, fd %d, thread %d\n",fptr->f_name,fd,fptr->f_tid);
    write_get_cmd(fptr);//向对应的超链接发送get请求

    for(;;)
    {
        if((n=read(fd,line,MAXLINE))==0)
            break;
        printf("read %d bytes from %s\n",n,fptr->f_name);
    }
    printf("end-of-file on %s\n",fptr->f_name);
    close(fd);

    pthread_mutex_lock(&ndone_mutex);
    fptr->f_flag=F_DONE;
    ndone++;
    pthread_cond_signal(&ndone_cond);//唤醒home_page中处理F_DONE的代码部分，因为pthread_join必须要指定等待那个具体的线程结束.
    pthread_mutex_unlock(&ndone_mutex);
    return fptr;
}

void home_page(const char * host, const char * fname)
{
    int fd, n;
    char line[MAXLINE];
    helpNet hp;
    fd = hp.Tcp_connect(host, "8080");
    if(fd<0)
    {
        cout<<"Tcp_connect failed"<<endl;
        return ;
    }
    n = snprintf(line, sizeof(line), GET_CMD, fname);
    cout<<"line: "<<line<<endl;
    write(fd, line, n);
    for(;;)
    {
        if((n=read(fd, line, MAXLINE)) == 0)
            break;
        cout<<"read "<<n<<" bytes of home page"<<endl;
    }
    cout<<"end-of-file on home page"<<endl;
    close(fd);
}

void write_get_cmd(struct file * fptr)
{
    int n;
    char line[MAXLINE];

    n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
    write(fptr->f_fd, line, n);
    printf("wrote %d bytes for %s\n", n, fptr->f_name);

    fptr->f_flag = F_READING;

    /*
    FD_SET(fptr->f_fd, &rset);
    if(fptr->f_fd>maxfd)
        maxfd = fptr->f_fd;
    */
}