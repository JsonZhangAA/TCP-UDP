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
    int f_flags;
};

#define F_CONNECTING 1
#define F_READING 2
#define F_DONE 4

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"



void home_page(const char *, const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);