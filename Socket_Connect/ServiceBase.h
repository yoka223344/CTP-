#include<iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "threadpool.h"
using namespace std;

class ServiceBase
{
	private:
		int SockFD;
		int ContractFD;
		struct sockaddr_in service_addr;
		ThreadPool threadpool;
	public:
		ServiceBase(int Port, int max_threadnum, int max_Jobnum);
		void start_listen(int listen_num, void *(*function)(void *arg));
		~ServiceBase();
};
