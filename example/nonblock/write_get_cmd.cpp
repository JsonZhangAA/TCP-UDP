#include "web.h"

extern struct file file[MAXFILES];
extern int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
extern fd_set rset, wset;

void write_get_cmd(struct file * fptr)
{
    int n;
    char line[MAXLINE];

    n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
    write(fptr->f_fd, line, n);
    printf("wrote %d bytes for %s\n", n, fptr->f_name);

    fptr->f_flags = F_READING;//当前请求以完成

    FD_SET(fptr->f_fd, &rset);
    if(fptr->f_fd>maxfd)
        maxfd = fptr->f_fd;
}