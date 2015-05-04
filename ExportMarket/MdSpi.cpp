#include "Client.h"
#include "MdSpi.h"
#include <iostream>
#include <mysql/mysql.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Logger.h"
#include "TimeConvert.h"
using namespace std;
#pragma warning(disable : 4996)
//#pragma comment(lib,"Ws2_32.lib")
// USER_API参数
extern Client *myClient; 
extern CThostFtdcMdApi* pMdApi;
extern Logger *my_logger;
// 配置参数
extern char FRONT_ADDR[];
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];
extern int iInstrumentID;

// 请求编号
extern int iRequestID;
/*void GetTableName(char *str, char *table)
{
	//memset(table, 0, sizeof(table));
	if(!strcmp(str, "cu"))
		strcpy(table, "Cu_FutureMarket");
	else if (!strcmp(str, "al"))
		strcpy(table, "Al_FutureMarket");
	else if (!strcmp(str, "zn"))
		strcpy(table, "Zn_FutureMarket");
	else if (!strcmp(str, "pb"))
		strcpy(table, "Pb_FutureMarket");
	else if (!strcmp(str, "au"))
		strcpy(table, "Au_FutureMarket");
	else if	(!strcmp(str, "ag"))
		strcpy(table, "Ag_FutureMarket");
	else if (!strcmp(str, "rb"))
		strcpy(table, "Rb_FutureMarket");
	else if (!strcmp(str, "wr"))
		strcpy(table, "Wr_FutureMarket");
	else if (!strcmp(str, "hc"))
		strcpy(table, "Hc_FutureMarket");
	else if (!strcmp(str, "fu"))
		strcpy(table, "Fu_FutureMarket");
	else if (!strcmp(str, "bu"))
		strcpy(table, "Bu_FutureMarket");
	else if (!strcmp(str, "ru"))
		strcpy(table, "Ru_FutureMarket");
	else if (!strcmp(str, "a"))
		strcpy(table, "A_FutureMarket");
	else if (!strcmp(str, "b"))
		strcpy(table, "B_FutureMarket");
	else if (!strcmp(str, "c"))
		strcpy(table, "C_FutureMarket");
	else if (!strcmp(str, "cs"))
		strcpy(table, "Cs_FutureMarket");
	else if (!strcmp(str, "m"))
		strcpy(table, "M_FutureMarket");
	else if (!strcmp(str, "y"))
		strcpy(table, "Y_FutureMarket");
	else if	(!strcmp(str, "p"))
		strcpy(table, "P_FutureMarket");
	else if (!strcmp(str, "jd"))
		strcpy(table, "JD_FutureMarket");
	else if (!strcmp(str, "fb"))
		strcpy(table, "FB_FutureMarket");
	else if (!strcmp(str, "bb"))
		strcpy(table, "BB_FutureMarket");
	else if (!strcmp(str, "l"))
		strcpy(table, "L_FutureMarket");
	else if (!strcmp(str, "v"))
		strcpy(table, "V_FutureMarket");
	else if (!strcmp(str, "j"))
		strcpy(table, "J_FutureMarket");
	else if (!strcmp(str, "jm"))
		strcpy(table, "JM_FutureMarket");
	else if (!strcmp(str, "i"))
		strcpy(table, "I_FutureMarket");
	else if (!strcmp(str, "pp"))
		strcpy(table, "PP_FutureMarket");
	else if (!strcmp(str, "CF"))
		strcpy(table, "CF_FutureMarket");
	else if (!strcmp(str, "FG"))
		strcpy(table, "FG_FutureMarket");
	else if (!strcmp(str,"JR"))
			strcpy(table, "JR_FutureMarket");
	else if (!strcmp(str,"LR"))
		strcpy(table, "LR_FutureMarket");
	else if (!strcmp(str, "MA"))
		strcpy(table, "MA_FutureMarket");
	else if (!strcmp(str, "ME"))
			strcpy(table, "ME_FutureMarket");
	else if (!strcmp(str, "OI"))
			strcpy(table, "OI_FutureMarket");
	else if (!strcmp(str, "PM"))
			strcpy(table, "PM_FutureMarket");
	else if (!strcmp(str, "RI"))
			strcpy(table, "RI_FutureMarket");
	else if (!strcmp(str, "RM"))
			strcpy(table, "PM_FutureMarket");
	else if (!strcmp(str, "RS"))
			strcpy(table, "RS_FutureMarket");
	else if (!strcmp(str, "SF"))
			strcpy(table, "SF_FutureMarket");
	else if (!strcmp(str, "SM"))
			strcpy(table, "SM_FutureMarket");
	else if (!strcmp(str, "SR"))
			strcpy(table, "SR_FutureMarket");
	else if (!strcmp(str, "TA"))
			strcpy(table, "TA_FutureMarket");
	else if (!strcmp(str, "TC"))
			strcpy(table, "TC_FutureMarket");
	else if (!strcmp(str, "WH"))
			strcpy(table, "WH_FutureMarket");
	else if (!strcmp(str, "sn"))
			strcpy(table, "Sn_FutureMarket");
	else if (!strcmp(str, "ni"))
			strcpy(table, "Ni_FutureMarket");
}*/

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Fatal(__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	char str[100];
	sprintf(str, "%s : reason is %d", __FUNCTION__, nReason);
	my_logger -> Logger_Info(str);
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	char str[100];
	sprintf(str, "%s : nTimerLapse is %d", __FUNCTION__, nTimeLapse);
	my_logger -> Logger_Info(str);
}

void CMdSpi::OnFrontConnected()
{
	my_logger -> Logger_Info(__FUNCTION__);
	///用户登录请求
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pMdApi->ReqUserLogin(&req, ++iRequestID);
	if (iResult == 0)
	{
		char str[] = "送用户登录请求:成功";
		my_logger -> Logger_Info(str);
	}
	else
	{
		char str[] = "送用户登录请求:失败";
		my_logger -> Logger_Info(str);
	}
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Info(__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		SubscribeMarketData();
	}
}
void CMdSpi::SubscribeMarketData()
{
	int iResult = pMdApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	if (iResult == 0)
	{
		char str[] = "送行情订阅请求:成功";
		my_logger -> Logger_Info(str);
	}
	else
	{
		char str[] = "送行情订阅请求:失败";
		my_logger -> Logger_Info(str);
	}
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Info(__FUNCTION__);
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	my_logger -> Logger_Info(__FUNCTION__);
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    Node pNode;
	if(myClient -> find(pDepthMarketData -> InstrumentID))
	{
		long timestamp = TimeStamp_Convert(pDepthMarketData->ActionDay, pDepthMarketData->UpdateTime);
		pNode.ArgVolume = pDepthMarketData->Volume;
		pNode.Last = pDepthMarketData->LastPrice;
		strcpy(pNode.Day, pDepthMarketData->ActionDay);
		strcpy(pNode.Time, pDepthMarketData->UpdateTime);
		pNode.OpenInterest = pDepthMarketData->OpenInterest;
		pNode.TimeStamp = timestamp;
		myClient -> UpdateData(pDepthMarketData->InstrumentID, pNode);
	}
	else
		return;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		char str[1000];
		sprintf(str,"RspInfo Erro\
r : ErrorID= %d, ErrorMsg= %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		my_logger -> Logger_Warning(str);
	}
	return bResult;
}
