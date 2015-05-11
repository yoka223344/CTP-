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
		myClient -> UpdateData(pDepthMarketData->InstrumentID, &pNode, timestamp);
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
