#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
using namespace std;

#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include "Logger.h"

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcTraderApi* pUserApi;

// ���ò���
extern char FRONT_ADDR[];		// ǰ�õ�ַ
extern char BROKER_ID[];		// ���͹�˾����
extern char INVESTOR_ID[];		// Ͷ���ߴ���
extern char PASSWORD[];			// �û�����
//extern char INSTRUMENT_ID[];	// ��Լ����
//extern TThostFtdcPriceType	LIMIT_PRICE;	// �۸�
//extern TThostFtdcDirectionType	DIRECTION;	// ��������

// ������
extern int iRequestID;
extern char* ppInstrumentID[];
extern int iInstrumentID;
extern Logger *my_logger;
extern char* ppStrikeDay[];
int now_instrumentID = 0;

// �Ự����
TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
TThostFtdcOrderRefType	ORDER_REF;	//��������

// �����ж�
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	 my_logger -> Logger_Info("OnFrontConnected");
	///�û���¼����
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	if (iResult == 0)
	{
		my_logger -> Logger_Info("�����û���¼����:�ɹ�");
	}
	else
	{
		my_logger -> Logger_Fatal("�����û���¼����:ʧ��");
	}
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///Ͷ���߽�����ȷ��
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	if (iResult == 0)
	{
				char table_m[19];
		my_logger -> Logger_Info("Ͷ���߽�����ȷ��:�ɹ�");
	}
	else
	{
		my_logger -> Logger_Fatal("Ͷ���߽�����ȷ��:ʧ��");
	}
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Info("OnRspSettlementInfoConfirm");
	if (bIsLast && !IsErrorRspInfo(pRspInfo)&&now_instrumentID < iInstrumentID)
	{
		///�����ѯ��Լ
		cout<<iInstrumentID<<endl;
		ReqQryInstrument(ppInstrumentID[now_instrumentID++]);
	}
}

void CTraderSpi::ReqQryInstrument(char *Instrument)
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, Instrument);
	cout << Instrument <<endl;
	while (true)
	{
		int iResult = pUserApi->ReqQryInstrument(&req, iRequestID);
		if (!IsFlowControl(iResult))
		{
			break;
		}
		else
		{
			sleep(1);
		}
	}
} // while

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Info("OnRspQryInstrument");
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		if (pInstrument != NULL)
		{	
			MYSQL mydata;
			//��ʼ�����ݽṹ
			if (NULL != mysql_init(&mydata)) 
			{
				my_logger -> Logger_Info("mysql_init() succeed");
			} 
			else 
			{
				my_logger -> Logger_Error("mysql_init() failed");
				return;
			}
			if (0 == mysql_library_init(0, NULL, NULL)) 
			{
				my_logger -> Logger_Info("mysql_library_init() succeed");
			}
			else 
			{
				char str[] = "mysql_library_init() failed";
				my_logger -> Logger_Error(str);
				return ;
			}
			if (NULL!= mysql_real_connect(&mydata, "192.168.22.59", "root", "abc0cba", "FutureMarket",
			        3306, NULL, 0))
            //����ĵ�ַ���û��������룬���ݿ⣬�˿ڿ��Ը����Լ����ص��������
			{
			
				my_logger -> Logger_Info("Trader mysql_real_connect() succeed");
				char mysql_quer[1000];
		
				sprintf(mysql_quer, "REPLACE INTO\
`FutureMarket_Info`(`InstrumentID`,`StrikeDay`) \
VALUES (\"%s\",\"%s\")",
					pInstrument->InstrumentID,
					pInstrument->EndDelivDate
					);
				if(mysql_query(&mydata, mysql_quer) != 0)
				{  
	
					char str[1000];
					sprintf(str,"Failed in futuremarket insert:%s",mysql_quer);
					my_logger -> Logger_Warning(str);
				}
			} 
			else 
			{
				char str[] = "mysql_real_connect() failed";
				my_logger -> Logger_Error(str);
				return;
			 }
			mysql_close(&mydata);
		}
		else
			cerr <<  "error:" << nRequestID <<endl;
	}
	sleep(1);
	if (now_instrumentID < iInstrumentID)
	{
		ReqQryInstrument(ppInstrumentID[now_instrumentID++]);
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			break;
		}
		else
		{
			cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ", �ܵ�����" << endl;
			sleep(1);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///�����ѯͶ���ֲ߳�
		ReqQryInstrument(ppInstrumentID[iInstrumentID]);
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, "cu1507");
	while (true)
	{
		int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cerr << "--->>> �����ѯͶ���ֲ߳�: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			break;
		}
		else
		{
			cerr << "--->>> �����ѯͶ���ֲ߳�: "  << iResult << ", �ܵ�����" << endl;
			sleep(1);
		}
	} // while
}



void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}


void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}


///�ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cerr << "--->>> " << "OnRtnTrade"  << endl;
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}
