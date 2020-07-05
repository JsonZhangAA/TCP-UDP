/*
Socket类封装如下方法：
bind、listen和connect
read、write
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
//#include <sys/types.h> 
#include <signal.h> //for 信号相关的函数
#include <sys/wait.h>//for watipid
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/un.h>//for struct sockaddr_un
#include <fcntl.h>//for fcntl
#include <errno.h>//for errno

typedef void Sigfunc(int);

class Socket
{
    public:
        //Socket(bool flag,int port);//flag为true表示使用TCP，否则使用UDP，sin_addr.s_addr初始化为INADDR_ANY
        //Socket(bool flag,int port, struct in_addr * ptr);//指定sin_addr.s_addr初始化为ptr
        Socket(bool flag,int port, struct in_addr * ptr=NULL);
        Socket(bool flag,char * sun_path);//负责为Unix域进行初始化,flag为true表示使用字节流，为false使用数据报
        ~Socket()
        {
            close(socket_fd);
            close(return_fd);   
        }
        void Bind();
        void BindUnix();
        void readBind();
        void Listen();
        int Connect();
        int ConnectUnix();
        int realConnect();
        int Read(int fd,void * buf,int len);
        //ssize_t nr = ::recvfrom(sock.fd(), &message, sizeof message, 0, &peerAddr, &addrLen);
        int Recvfrom(void * buf,int len,struct sockaddr_in & peerAddr);
        int Recvfrom(void * buf,int len,struct sockaddr_un & peerAddr);
        int realRecvfrom(void * buf, int len,struct sockaddr * peerAddr);
        //sendto(sock.fd(), &message, sizeof message, 0, &peerAddr, addrLen);
        int Sendto(const void * buf, int len,struct sockaddr_in & addr);
        int Sendto(const void * buf, int len,struct sockaddr_un & addr);
        int realSendto(const void * buf,int len,struct sockaddr * addr);
        int Write(int fd,const void * buf,int len);
        int Accept();
        int AcceptUnix();
        int realAccept();
        void Setsockopt();
        int get_return_fd()
        {
            return return_fd;
        }
        int get_socket_fd()
        {
            return socket_fd;
        }
        struct sockaddr_in get_addr()
        {
            return addr;
        }
        struct sockaddr_un get_addr1()
        {
            return addr1;
        }
    private:
        int socket_fd,return_fd;
        struct sockaddr_in addr;//用于不同主机之间的socket编程
        struct sockaddr_un addr1;//用于同一主机内的socket编程
        bool inOrUn;//true表示sockaddr_in，false表示使用sockaddr_un。
};