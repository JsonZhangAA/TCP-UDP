#include "web.h"

int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
fd_set rset, wset;

struct file file[MAXFILES];

int main(int argc, char * * argv)
{
    FILE * fp=fopen("./result.txt","w+");
    int i, fd, n, maxnconn, flags, error;
    char buf[MAXLINE];
    fd_set rs, ws;
    if(argc<5)
    {
        cout<<"error: argc<5"<<endl;
        return 0;
    }
    maxnconn = atoi(argv[1]);

    nfiles=min(argc-4,MAXFILES);
    for(i=0;i<nfiles;i++)
    {
        file[i].f_name = argv[i+4];
        file[i].f_host = argv[2];
        file[i].f_flags = 0;
    }
    cout<<"nfiles = "<<nfiles<<endl;
    home_page(argv[2], argv[3]);
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    maxfd = 1;
    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;
    while (nlefttoread > 0) {
		while (nconn<maxnconn&&nlefttoconn>0) {//为各请求启动HTTP连接，注意事项非阻塞，故后续有的请求可能还处于进行连接的状态。
				/* 4find a file to read */
			for (i = 0 ; i < nfiles; i++)
				if (file[i].f_flags == 0)
					break;
			if (i == nfiles)
            {
				printf("nlefttoconn = %d but nothing found", nlefttoconn);
                return 0;
            }
			start_connect(&file[i]);
			nconn++;
			nlefttoconn--;
		}

		rs = rset;
		ws = wset;
		n = select(maxfd+1, &rs, &ws, NULL, NULL);

		for (i = 0; i < nfiles; i++) {
			flags = file[i].f_flags;
			if (flags == 0 || flags & F_DONE)//flag=0表示请求不存在，flags=F_DONE表示当前请求已经读写完毕了。
				continue;
			fd = file[i].f_fd;
			if (flags & F_CONNECTING &&
				(FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {//当前请求正在进行连接
				socklen_t nn = sizeof(error);
				if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error,&nn) < 0 ||
					error != 0) {
					printf("nonblocking connect failed for %s",
							file[i].f_name);
                    return 0;
				}
					/* 4connection established */
				printf("connection established for %s\n", file[i].f_name);
				FD_CLR(fd, &wset);		/* no more writeability test */
				write_get_cmd(&file[i]);/* write() the GET command */

			} else if (flags & F_READING && FD_ISSET(fd, &rs)) {//当前请求已经完成连接，对其进行读写操作。
				if ( (n = read(fd, buf, sizeof(buf))) == 0) {
					printf("end-of-file on %s\n", file[i].f_name);
					close(fd);
					file[i].f_flags = F_DONE;	/* clears F_READING */
					FD_CLR(fd, &rset);
					nconn--;
					nlefttoread--;
				} else {
					printf("read %d bytes from %s\n", n, file[i].f_name);
                    fputs(buf,fp);
                    //cout<<"buf: "<<buf<<endl;
				}
			}
		}
	}
	exit(0);
}