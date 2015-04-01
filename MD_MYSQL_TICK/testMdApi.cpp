// testTraderApi.cpp : ????????台应?贸????????诘恪?
//

#include "MdSpi.h"
#include "TraderSpi.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#define MaxInstrumentNumber 1000
using namespace std;

#include "./ThostTraderApi/ThostFtdcMdApi.h"
#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "StatNode.h"
#include "log4cpp/Priority.hh"
// UserApi????
CThostFtdcMdApi* pMdApi;

CThostFtdcTraderApi *pUserApi;

Logger *my_logger;

// ???貌???
char  FRONT_ADDR_TRADER[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";   //????前?玫?址
char FRONT_ADDR_MD[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ????前?玫?址
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ???凸?司????
TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// 投???叽???
TThostFtdcPasswordType  PASSWORD = "888888";			// ?没?????
char *ppInstrumentID[MaxInstrumentNumber] = {};			// ???槎???斜?
int iInstrumentID = 0;									// ???槎????量
StatNode Stat_queue[500];
// ????????
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
			Stat_queue[iInstrumentID].init(ppInstrumentID[iInstrumentID]);
//			cout<<ppInstrumentID[iInstrumentID]<<iInstrumentID<<endl;
			iInstrumentID++;
		}
	}
	fd.close();
}

int main()
{
	char * log_file = "./log/test.log";
	my_logger = new Logger(log_file, log4cpp::Priority::DEBUG);
	char *filepathtest = "/home/lizx/crawlled_page/test.txt";
	GetInstrumentID(filepathtest);
	cout<< "Instrument load success:"<<iInstrumentID<<endl;
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);	
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);		
	pUserApi->RegisterFront(FRONT_ADDR_TRADER); // connect
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi();			//UserApi
	CThostFtdcMdSpi* pMdSpi = new CMdSpi();
	pMdApi->RegisterSpi(pMdSpi);						
	pMdApi->RegisterFront(FRONT_ADDR_MD);					// connect
	pMdApi->Init();
	pUserApi->Init();
	pUserApi->Join();
	pMdApi->Join();
	return 0;
}
