#include "Client.h"
#include "MdSpi.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define MaxInstrumentNumber 1000
using namespace std;
#include "./ThostTraderApi/ThostFtdcMdApi.h"
#include "log4cpp/Priority.hh"

CThostFtdcMdApi *pMdApi;
Client *myClient;
Logger *my_logger;
char  FRONT_ADDR_TRADER[] = "tcp://180.168.146.181:10000";
char FRONT_ADDR_MD[] = "tcp://180.168.146.181:10100";
TThostFtdcBrokerIDType	BROKER_ID = "2030";
TThostFtdcInvestorIDType INVESTOR_ID = "00092";
TThostFtdcPasswordType  PASSWORD = "888888";
char *ppInstrumentID[MaxInstrumentNumber] = {};
int iInstrumentID = 0;

int iRequestID = 0;

void GetInstrumentID(char *filepath)
{
	fstream fd;
	fd.open(filepath, ios::in);
	if (fd.fail())
	{	
		cout << "open failed" <<endl;
	}
	char tempInstrumentID[10];
	while (!fd.eof())
	{
		fd.getline(tempInstrumentID,10);
		if(tempInstrumentID != NULL)
		{
//			cout<<tempInstrumentID<<endl;
			ppInstrumentID[iInstrumentID] = (char *) malloc(10*sizeof(char));
			strcpy(ppInstrumentID[iInstrumentID], tempInstrumentID);
//			cout<<ppInstrumentID[iInstrumentID]<<iInstrumentID<<endl;
			iInstrumentID++;
		}
	}
	fd.close();
}
//int Node::count = 0;
int main()
{
    myClient = new Client;
	char *log_file = "./log/test.log";
	my_logger = new Logger(log_file, log4cpp::Priority::DEBUG);
	char *filepathtest = "/home/lizx/crawlled_page/test.txt";
	GetInstrumentID(filepathtest);
    myClient -> AddInstrument("cu1509", 0, 50);
    myClient -> AddInstrument("cu1512", 0, 50);
	double Open[100] = {0};
	cout << myClient -> getOpen(Open, "cu1509", 0, 0, 100) << endl;
	for (int i = 0 ;i < 51; i++)
	{
		cout << Open[i] <<endl;
	}
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcMdSpi* pMdSpi = new CMdSpi();
	pMdApi->RegisterSpi(pMdSpi);
	pMdApi->RegisterFront(FRONT_ADDR_MD);
	pMdApi->Init();
	pMdApi->Join();
	return 0;
}
