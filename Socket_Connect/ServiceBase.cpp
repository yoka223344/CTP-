#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "ServiceBase.h"
using namespace std;

ServiceBase::ServiceBase(int Port, int max_threadnum, int max_Jobnum)
{
	threadpool =ThreadPool(max_threadnum, max_Jobnum);
	SockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == SockFD)
    {
		cerr << "can not create socket" << endl;
		exit(1);
	}
	memset(&service_addr, 0, sizeof(service_addr));
	service_addr.sin_family = AF_INET;
	service_addr.sin_port = htons(9000);
	service_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(-1 == bind(SockFD,(struct sockaddr *)&service_addr, sizeof(service_addr)))
	{
		cerr << "error bind failed" << endl;
		close(SockFD);
		exit(1);
	}
}
void ServiceBase::start_listen(int listen_num, void *(*function)(void *arg))
{
	if (-1 == listen(SockFD, listen_num))
	{
		cerr << "error listen failed" << endl;
		close(SockFD);
		exit(1);
	}
	while(1)
	{
		int ConnectFD = accept(SockFD, NULL, NULL);
		int *fdptr = (int *)malloc(sizeof(int));
		*fdptr = ConnectFD;
		if(0 > ConnectFD)
		{
			cerr << "error accept failed" << endl;
			close(SockFD);
			exit(1);
		}
		Job job;
		job.doJobFun = function;
		job.arg = fdptr;
		threadpool.addJob(job);
	}
}
ServiceBase::~ServiceBase()
{
	threadpool.~ThreadPool();
}

