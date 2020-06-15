//#include "../Socket.h"
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define MAX_LEN 100

int main(int argc, char * * argv)
{
    char * ptr, * * pptr;
    char str[MAX_LEN];
    struct  hostent * hptr;

    while ((--argc>0))
    {
        ptr=*++argv;
        if((hptr=gethostbyname(ptr))==NULL)
        {
            cout<<"error: hostname: "<<ptr<<" not right"<<endl;
            continue;
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
            pptr=hptr->h_addr_list;
            while(*pptr!=NULL)
            {
                cout<<"addr: "<<inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str))<<endl;
                pptr++;
            }
            break;
        default:
            break;
        }
    }
    return 0;
}