#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

class Strategy
{
	private:
		int Socket_to_Trading;
		int Socket_to_Risk;
		struct sockaddr_in Trading_Sockaddr;
		struct sockaddr_in Risking_Sockaddr;
		int SetIpPort(struct sockaddr_in *sockad, char *IP, int Port);
	public:
		Strategy(char *Trading_IP = "127.0.0.1",
				char *Risking_IP = "127.0.0.1",
				int Trading_Port = 9000,
				int Risking_Port = 9001);
		//type 为 0 表示连接的是 交易，type为1 表示连接的是风控,下同 
		int Connect(int type);
		int send_data(void *buf, int type);
		void Close_Connect(int type);
		~Strategy();
};
