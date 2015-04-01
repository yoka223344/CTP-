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

// USER_API参数
extern CThostFtdcTraderApi* pUserApi;

// 配置参数
extern char FRONT_ADDR[];		// 前置地址
extern char BROKER_ID[];		// 经纪公司代码
extern char INVESTOR_ID[];		// 投资者代码
extern char PASSWORD[];			// 用户密码
//extern char INSTRUMENT_ID[];	// 合约代码
//extern TThostFtdcPriceType	LIMIT_PRICE;	// 价格
//extern TThostFtdcDirectionType	DIRECTION;	// 买卖方向

// 请求编号
extern int iRequestID;
extern char* ppInstrumentID[];
extern int iInstrumentID;
extern Logger *my_logger;
extern char* ppStrikeDay[];
int now_instrumentID = 0;

// 会话参数
TThostFtdcFrontIDType	FRONT_ID;	//前置编号
TThostFtdcSessionIDType	SESSION_ID;	//会话编号
TThostFtdcOrderRefType	ORDER_REF;	//报单引用

// 流控判断
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	 my_logger -> Logger_Info("OnFrontConnected");
	///用户登录请求
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
		my_logger -> Logger_Info("发送用户登录请求:成功");
	}
	else
	{
		my_logger -> Logger_Fatal("发送用户登录请求:失败");
	}
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///投资者结算结果确认
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
		my_logger -> Logger_Info("投资者结算结果确认:成功");
	}
	else
	{
		my_logger -> Logger_Fatal("投资者结算结果确认:失败");
	}
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Info("OnRspSettlementInfoConfirm");
	if (bIsLast && !IsErrorRspInfo(pRspInfo)&&now_instrumentID < iInstrumentID)
	{
		///请求查询合约
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
			//初始化数据结构
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
            //这里的地址，用户名，密码，数据库，端口可以根据自己本地的情况更改
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
			cerr << "--->>> 请求查询资金账户: "  << iResult << ((iResult == 0) ? ", 成功" : ", 失败") << endl;
			break;
		}
		else
		{
			cerr << "--->>> 请求查询资金账户: "  << iResult << ", 受到流控" << endl;
			sleep(1);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询投资者持仓
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
			cerr << "--->>> 请求查询投资者持仓: "  << iResult << ((iResult == 0) ? ", 成功" : ", 失败") << endl;
			break;
		}
		else
		{
			cerr << "--->>> 请求查询投资者持仓: "  << iResult << ", 受到流控" << endl;
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


///成交通知
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
	// 如果ErrorID != 0, 说明收到了错误的响应
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
