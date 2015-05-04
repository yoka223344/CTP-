/*************************************************************************
    > File Name: service.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 20 Apr 2015 04:18:58 PM UTC
 ************************************************************************/

#include<iostream>
#include "ServiceBase.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;
void *make_action(void *contract)
{
	int *contractFD = (int *) contract;
	char Instrument[7];
	read(*contractFD, Instrument, 7);
	cout << Instrument <<endl;
	sleep(10);
	int res = 12345;
	write(*contractFD, &res, sizeof(res));
	close(*contractFD);
}

int main()
{
	ServiceBase service(9000, 10, 20);
	service.start_listen(10, make_action);
	
}
