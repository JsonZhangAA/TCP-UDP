#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char * * argv)
{
    struct in_addr ptr;
    struct  hostent * hptr;
    int i=1;
    while((--argc>0))
    {
        inet_pton(AF_INET, argv[i++], &ptr);
        if((hptr=gethostbyaddr(&ptr,sizeof(ptr),AF_INET))==NULL)
        {
            cout<<"error:  h_errno: "<<h_errno<<endl;
        }

        cout<<"h_name: "<<hptr->h_name<<endl;

        char * * aliases=hptr->h_aliases;
        cout<<"h_aliases: ";
        while(*aliases!=NULL)
        {
            cout<<*aliases<<" ";
            aliases++;
        }
        cout<<endl;

        switch (hptr->h_addrtype)
        {
        case AF_INET:
            cout<<"h_addrtype: "<<hptr->h_addrtype<<endl;
            break;
        default:
            break;
        }
    }
    return 0;
}