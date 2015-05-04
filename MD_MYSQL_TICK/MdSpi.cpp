#include "MdSpi.h"
#include <iostream>
#include <mysql/mysql.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Logger.h"
#include "StatNode.h"
#include "TimeConvert.h"
using namespace std;
#pragma warning(disable : 4996)
//#pragma comment(lib,"Ws2_32.lib") 
// USER_API参数
extern CThostFtdcMdApi* pMdApi;
extern Logger *my_logger;
// 配置参数
extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];	
extern int iInstrumentID;
extern StatNode Stat_queue[500];

// 请求编号
extern int iRequestID;
void GetTableName(char *str, char *table) 
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
}

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

    //初始化数据库
    if (0 == mysql_library_init(0, NULL, NULL)) 
	{
		
		my_logger -> Logger_Info("mysql_library_init() succeed");
		char str[] = "Update time:";
		my_logger -> Logger_Info(str);
		my_logger -> Logger_Info(pDepthMarketData->UpdateTime);
    }
	else 
	{
		char str[] = "mysql_library_init() failed";
		my_logger -> Logger_Error(str);
        return ;
    }
	
    //连接数据库
    if (NULL!= mysql_real_connect(&mydata, "localhost", "root", "abc0cba", "FutureMarket",
                    3306, NULL, 0))
            //这里的地址，用户名，密码，数据库，端口可以根据自己本地的情况更改
	{
		my_logger -> Logger_Info("FutureMarket connect succeed");
		char mysql_quer[10000];
		double ClosePrice = 0;
		double OpenPrice = 0;
		if (pDepthMarketData->ClosePrice <= 1000000 && pDepthMarketData->ClosePrice >= 0)
			ClosePrice = pDepthMarketData->ClosePrice;
		if (pDepthMarketData->OpenPrice <= 100000 && pDepthMarketData->OpenPrice >= 0)
			OpenPrice = pDepthMarketData->OpenPrice;
		char Instrument_kind[3];
		char table[15];
		if ((int)(pDepthMarketData->InstrumentID[1] - 'A') >= 0 )
		{
			char lin[3] = {pDepthMarketData->InstrumentID[0], pDepthMarketData->InstrumentID[1],'\0'};
			strcpy(Instrument_kind, lin);
		}
		else
		{
			char lin[3] = {pDepthMarketData->InstrumentID[0], '\0', '\0'};
			strcpy(Instrument_kind, lin);
		}
		GetTableName(Instrument_kind, table);
		time_t timestamp;
		timestamp = TimeStamp_Convert(pDepthMarketData->ActionDay, pDepthMarketData->UpdateTime);
		sprintf(mysql_quer,"INSERT INTO `%s`(`TradingDay`, `InstrumentID`, `ExchangeID`, \
`ExchangeInstID`, `LastPrice`,`HighestPrice`, `LowestPrice`, `Volume`, `Turnover`, `OpenInterest`, \
`SettlementPrice`, `UpperLimitPrice`, `LowerLimitPrice`, `CurrDelta`, `UpdateTime`, `UpdateMillisec`, \
`BidPrice1`, `BidVolume1`, `AskPrice1`, `AskVolume1`, `BidPrice2`, `BidVolume2`, `AskPrice2`, `AskVolume2`, \
`BidPrice3`, `BidVolume3`, `AskPrice3`, `AskVolume3`, `BidPrice4`, `BidVolume4`, `AskPrice4`, `AskVolume4`, \
`BidPrice5`, `BidVolume5`, `AskPrice5`, `AskVolume5`, `AveragePrice`, `ActionDay`, `TimeStamp`) \
VALUES (\"%s\",\"%s\",\"%s\",\"%s\",%10.2lf,%10.2lf,%10.2lf,%ld,\
%10.2lf,%10.2lf,%10.2lf,%10.2lf,%10.2lf,%10.2lf,\"%s\",%ld,%10.2lf,%ld,%10.2lf,%ld,%10.2lf,%ld,\
%10.2lf,%ld,%10.2lf,%ld,%10.2lf,%ld,%10.2lf,%ld,%10.2lf,%ld,%10.2lf,%ld,%10.2lf,%ld,%10.2lf,\"%s\", %ld)",
							table,
							pDepthMarketData->TradingDay,
							pDepthMarketData->InstrumentID,
							pDepthMarketData->ExchangeID,
							pDepthMarketData->ExchangeInstID,
							pDepthMarketData->LastPrice,
							pDepthMarketData->HighestPrice,
							pDepthMarketData->LowestPrice,
							pDepthMarketData->Volume,
							pDepthMarketData->Turnover,
							pDepthMarketData->OpenInterest,
							pDepthMarketData->SettlementPrice,
							pDepthMarketData->UpperLimitPrice,
							pDepthMarketData->LowerLimitPrice,
							pDepthMarketData->CurrDelta,
							pDepthMarketData->UpdateTime,
							pDepthMarketData->UpdateMillisec,
							pDepthMarketData->BidPrice1,
							pDepthMarketData->BidVolume1,
							pDepthMarketData->AskPrice1,
							pDepthMarketData->AskVolume1,
							//pDepthMarketData->BidPrice2,
							0,
							pDepthMarketData->BidVolume2,
							//pDepthMarketData->AskPrice2,
							0,
							pDepthMarketData->AskVolume2,
							//pDepthMarketData->BidPrice3,
							0,
							pDepthMarketData->BidVolume3,
							//pDepthMarketData->AskPrice3,
							0,
							pDepthMarketData->AskVolume3,
							//pDepthMarketData->BidPrice4,
							0,
							pDepthMarketData->BidVolume4,
							//pDepthMarketData->AskPrice4,
							0,
							pDepthMarketData->AskVolume4,
							//pDepthMarketData->BidPrice5,
							0,
							pDepthMarketData->BidVolume5,
							//pDepthMarketData->AskPrice5,
							0,
							pDepthMarketData->AskVolume5,
							pDepthMarketData->AveragePrice,
							pDepthMarketData->ActionDay,
							timestamp
							);
		//cerr << mysql_quer << endl;
		int tmp_k = 0;
		while(strcmp(ppInstrumentID[tmp_k], pDepthMarketData->InstrumentID)!=0)
			tmp_k++;
		Stat_queue[tmp_k].daily_OpenClose(OpenPrice,ClosePrice);
		char openclose_sql[10000];
		char table_day[19];
		strcpy(table_day, table);
		strcat(table_day,"_DAY");
		sprintf(openclose_sql, "REPLACE INTO `%s`(`TradingDay`, `InstrumentID`, \
`PreSettlementPrice`, `PreClosePrice`, `PreOpenInterest`, `PreDelta`, `OpenPrice`, `ClosePrice`, `HighestPrice`, `LowestPrice`, `SettlementPrice`, `Volume`, `OpenInterset`)\
VALUES (\"%s\", \"%s\", %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %ld, %10.2lf)",
							table_day,
							pDepthMarketData->TradingDay,
							pDepthMarketData->InstrumentID,
							pDepthMarketData->PreSettlementPrice,
							pDepthMarketData->PreClosePrice,
							pDepthMarketData->PreOpenInterest,
							pDepthMarketData->PreDelta,
							Stat_queue[tmp_k].OpenPrice,
							Stat_queue[tmp_k].ClosePrice,
							pDepthMarketData->HighestPrice,
							pDepthMarketData->LowestPrice,
							pDepthMarketData->SettlementPrice,
							pDepthMarketData->Volume,
							pDepthMarketData->OpenInterest
							);
		int flag = Stat_queue[tmp_k].Updatecomp(pDepthMarketData->UpdateTime);
		if (flag == 0 || flag == 1)
		{
			if(flag == 1)
			{
				Stat_queue[tmp_k].get_Volume();
				char min_sql[1000];
				char table_min[19];
				strcpy(table_min, table);
				strcat(table_min, "_MIN");
				time_t timestamp;
				timestamp = TimeStamp_Convert(Stat_queue[tmp_k].TradingDay, Stat_queue[tmp_k].Updatetime);
				sprintf(min_sql,"INSERT INTO `%s`\
(`TradingDay`, `InstrumentID`, `High`, `Low`, `Open`, `Close`,`Volume`, `AgrVolume`, `OpenInterest`,`Updatetime`, `TimeStamp`) VALUES (\"%s\",\"%s\",%10.2lf,%10.2lf,%10.2lf,%10.2lf,%ld,%ld,%10.2lf,\"%s\", %ld)",
					table_min,
					Stat_queue[tmp_k].TradingDay,
					Stat_queue[tmp_k].InstrumentID,
					Stat_queue[tmp_k].High,
					Stat_queue[tmp_k].Low,
					Stat_queue[tmp_k].Open,
					Stat_queue[tmp_k].Close,
					Stat_queue[tmp_k].Volume,
					Stat_queue[tmp_k].agrVolume,
					Stat_queue[tmp_k].OpenInterset,
					Stat_queue[tmp_k].Updatetime,
					timestamp);
				if (mysql_query(&mydata, min_sql) != 0)
				{
	
					char str[1000];
					sprintf(str,"Failed in Future_Min insert:%s",mysql_quer);
					my_logger -> Logger_Warning(str);

			
				}
				Stat_queue[tmp_k].reset();
			}
			strcpy(Stat_queue[tmp_k].TradingDay, pDepthMarketData->TradingDay);
			Stat_queue[tmp_k].Open = pDepthMarketData->LastPrice;
			Stat_queue[tmp_k].set_price(pDepthMarketData->LastPrice);
			Stat_queue[tmp_k].agrVolume = pDepthMarketData->Volume;
			Stat_queue[tmp_k].OpenInterset = pDepthMarketData->OpenInterest;
		}
		else if (flag == 2)
		{
			Stat_queue[tmp_k].set_price(pDepthMarketData->LastPrice);
			
		}
		if(mysql_query(&mydata, mysql_quer) != 0)
		{  
			char str[10000];
			sprintf(str,"Failed in futuremarket insert:%s",mysql_quer);
			my_logger -> Logger_Warning(str);
			
		}
		if(mysql_query(&mydata, openclose_sql) != 0)
		{  
			char str[10000]; 
			sprintf(str, "Failed in FutureMarketOpenClose insert:%s"
					, openclose_sql);
			my_logger -> Logger_Warning(str);
		}
	} 
	else 
	{
		char str[] = "FutureMarket connect failed";
		my_logger -> Logger_Error(str);
        return;
    }
	mysql_close(&mydata);
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
