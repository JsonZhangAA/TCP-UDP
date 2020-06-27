#include "str.h"

int main(int argc, char * * argv)
{
    Socket * ssocket=new Socket(true,1234);
    ssocket->Connect();
    FILE * fp=fopen("./input.txt","r");
    str_cli(fp,ssocket->get_socket_fd());
    return 0;
}