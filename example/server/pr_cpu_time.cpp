#include "../../Socket.h"
#include <iostream>
#include <sys/resource.h>

using namespace std;

#ifndef HAVE_GETRUSAGE_PROTO
int getusage(int,struct rusage *);
#endif

void pr_cpu_time(void)
{
    double user,sys;
    struct rusage myusage,childusage;

    if(getrusage(RUSAGE_SELF,&myusage)<0)
    {
        cout<<"getrusage error"<<endl;
        return;
    }
    if(getrusage(RUSAGE_SELF,&childusage)<0)
    {
        cout<<"getrusage error"<<endl;
        return;
    }
    user = (double) myusage.ru_utime.tv_sec +
					myusage.ru_utime.tv_usec/1000000.0;
	user += (double) childusage.ru_utime.tv_sec +
					 childusage.ru_utime.tv_usec/1000000.0;
	sys = (double) myusage.ru_stime.tv_sec +
				   myusage.ru_stime.tv_usec/1000000.0;
	sys += (double) childusage.ru_stime.tv_sec +
					childusage.ru_stime.tv_usec/1000000.0;

	printf("\nuser time = %g, sys time = %g\n", user, sys);
}

