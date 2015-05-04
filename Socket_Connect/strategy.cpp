#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "strategy.h"
using namespace std;


Strategy::Strategy(char *Trading_IP,
				char *Risking_IP,
				int Trading_Port,
				int Risking_Port)
{
	Socket_to_Trading = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	Socket_to_Risk = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket_to_Trading == -1 || Socket_to_Trading == -1)
	{
		cerr << "Socket Create Failed" <<endl;
		exit(1);
	}
	memset(&Trading_Sockaddr, 0, sizeof(Trading_Sockaddr));
	memset(&Risking_Sockaddr, 0, sizeof(Risking_Sockaddr));
	if (SetIpPort(&Trading_Sockaddr, Trading_IP, Trading_Port) <= 0)
	{
		close(Socket_to_Trading);
		cout << "default IP and Port Error" <<endl;
		exit(1);
	}
	if (SetIpPort(&Risking_Sockaddr, Risking_IP, Risking_Port) <= 0)
	{
		close(Socket_to_Risk);
		cout << "default IP and Port Error" <<endl;
		exit(1);
	}

}

int Strategy::SetIpPort(struct sockaddr_in *sockad, char *IP, int Port)
{
	int RES;
	sockad->sin_family = AF_INET;	
	sockad->sin_port =htons(Port);
	RES = inet_pton(AF_INET, IP, &sockad->sin_addr);
	return RES;
}

int Strategy::Connect(int type)
{
	int res;
	int sockFD;
	struct sockaddr_in stSockAddr;
	if (type == 0)
	{
		sockFD = Socket_to_Trading;
		stSockAddr = Trading_Sockaddr;
	}
	else
	{
		sockFD = Socket_to_Risk;
		stSockAddr = Risking_Sockaddr; 
	}
	res = connect(sockFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
	if (res == -1)
	{
		cerr << type <<" : Connect Error" <<endl;
		close(sockFD);
	}
	else
	{
		cerr << "connect success" << stSockAddr.sin_port <<endl;
	}
}

int Strategy::send_data(void *buf, int type)
{
	int sockFD;
	if (type == 0)
	{
		sockFD = Socket_to_Trading;
	}
	else
	{
		sockFD = Socket_to_Risk;
	}
	write(sockFD, buf, sizeof(&buf));
	int res;
	while(read(sockFD, &res, sizeof(res))<=0)
	{
		sleep(1);
	}
	return res;
}

void Strategy::Close_Connect(int type)
{
	if (type == 0)
		close(Socket_to_Trading);
	else
		close(Socket_to_Risk);
}
Strategy::~Strategy()
{
	close(Socket_to_Trading);
	close(Socket_to_Risk);
}







