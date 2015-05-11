#include "DataFetch.h"
#include <string.h>
#include <vector>
#include "Logger.h"
#include "mysql/mysql.h"
#include <stdlib.h>
#include <ctime>
#include <cfloat>

using namespace std;



extern Logger *my_logger;

// get the table name
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

// get table_MIN
void Get_table_min(char *str, char *table_min)
{
	char table[15];
	GetTableName(str, table);
	strcpy(table_min, table);
	strcat(table_min, "_MIN");
}

// parser Instrument to get the kind of contract
void Get_table_kind(char *table_kind, char *Instrument)
{
	if((int)(Instrument[1] - 'A') >= 0 )
	{
		char lin[3] = {Instrument[0], Instrument[1], '\0'};
		strcpy(table_kind, lin);
	}
	else
	{
		char lin[3] = {Instrument[0], '\0', '\0'};
		strcpy(table_kind, lin);
	}
}

// get the table_DAY
void Get_table_day(char *str, char *table_day)
{
	char table[15];
	GetTableName(str, table);
	strcpy(table_day, table);
	strcat(table_day, "_DAY");
}

//Convert the datetime to TimeStamp
time_t TimeStamp_Convert(char *datetime)
{
	tm ptr;
	time_t timestamp;
	strptime(datetime, "%Y%m%d%H:%M:%S", &ptr);
	timestamp = mktime(&ptr);
	return timestamp;
}


bool Datafetch::connect(char *host,
	char *user,
	char *passwd,
	char *database,
	int port)
{
	if (NULL == mysql_init(&_mydata))
	{
		my_logger -> Logger_Info("init failed");
		return false;
	}
	if (0 != mysql_library_init(0, NULL, NULL))
	{
		my_logger -> Logger_Info("database init failed");
		return false;
	}
	if (NULL != mysql_real_connect(&_mydata, host, user, passwd, database, port, NULL, 0))
	{
		my_logger -> Logger_Info("FutureMarket connect complete");
		return true;
	}
	else
	{
		my_logger -> Logger_Info("connect failed");
		return false;
	}
}
void Datafetch::close_connect()
{
	mysql_free_result(_result);
	mysql_close(&_mydata);
	my_logger -> Logger_Info("FutureMarket Connect Closed");
}

int Datafetch::get_sql_result(char *sql)
{
	int res;
	res = mysql_query(&_mydata, sql);
	if (!res)
	{
		_result = mysql_store_result(&_mydata);
	}
	else
	{
		my_logger -> Logger_Error(sql);
	}
	return res;
}

long Datafetch::set_node_data(char *InstrumentID, ClientBase *myClient, int span_sec)
{
	int max_size = myClient -> node_number * span_sec *4 ; 
    MYSQL_ROW m_row;
    char sql[1000];
    char table[20];
    char Instrument_kind[3];
    Get_table_kind(Instrument_kind, InstrumentID);
    GetTableName(Instrument_kind, table);
    sprintf(sql, "SELECT `LastPrice`, `OpenInterest`, `Volume`, `ActionDay`, `UpdateTime` \
, `TimeStamp` FROM %s WHERE `InstrumentID`= \"%s\" and `TradingDay` IS NOT NULL ORDER BY `TimeStamp` DESC limit %d", table, InstrumentID, max_size);
	int result_num =  get_sql_result(sql);
    Node *pNode = myClient -> NodeList;
	long timestamp_end = -1;
	long last_timestamp = -1;
	double High = 0;
	double Low = DBL_MAX;
    while((m_row = mysql_fetch_row(_result)))
    {
		double lastprice = atof(m_row[0]);
		if (timestamp_end == -1)
		{
			last_timestamp = atol(m_row[5]);
			timestamp_end = atol(m_row[5]) - span_sec;
		}
		if (atol(m_row[5]) < timestamp_end)
		{
			timestamp_end =atol(m_row[5]) -  span_sec;
			pNode -> Last = lastprice;
			pNode -> OpenInterest = atoi(m_row[1]);
			pNode -> ArgVolume = atoi(m_row[2]);
			pNode -> High = High;
			pNode -> Low = Low;
			strcpy(pNode ->Day, m_row[3]);
			strcpy(pNode ->Time, m_row[4]);
			pNode = pNode -> Next;
			if (pNode == myClient -> NodeList)
				break;
			else
			{
				pNode -> Open = lastprice;
				High = lastprice;
				Low = lastprice;
				pNode -> Close = lastprice;
			}
			if (pNode -> Front -> ArgVolume - pNode -> ArgVolume > 0)
				pNode -> Volume = pNode -> Front -> ArgVolume - pNode -> ArgVolume;
		}
		else
		{
			if (High < lastprice)
			{
				High = lastprice;
			}
			if (Low > lastprice)
			{
				Low = lastprice;
			}
			pNode -> Open = lastprice; 
		}
	}
	return (timestamp_end + span_sec);
}

vector<double> Datafetch::get_ClosePrice(char *Instrument,
		char *begintime, char *endtime, int type)
{
	vector<double> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	char listname[10];
	time_t begin_stamp;
	time_t end_stamp;
	begin_stamp = TimeStamp_Convert(begintime);
	end_stamp = TimeStamp_Convert(endtime);
	Get_table_kind(Instrument_kind, Instrument);
	//chose the column and table with different level
	if( type == 0)
	{
		strcpy(listname, "LastPrice");
		GetTableName(Instrument_kind, table);
	}
	else
	{
		strcpy(listname, "Close");
		Get_table_min(Instrument_kind, table);
	}
	sprintf(sql, "SELECT `%s` FROM `%s` \
WHERE `InstrumentID`=\"%s\"  and `TimeStamp` >=%ld and `TimeStamp`<=%ld ORDER BY `TimeStamp` ASC",
			listname,
			table,
			Instrument,
			begin_stamp,
			end_stamp,
			begintime,
			endtime
					);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double closeprice = atof(m_row[0]);
			result.push_back(closeprice);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<int> Datafetch::get_Volume(char *Instrument,
		char *begintime, char *endtime, int type)
{
	vector<int> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	char column_name[10];
	time_t begin_stamp;
	time_t end_stamp;
	begin_stamp = TimeStamp_Convert(begintime);
	end_stamp = TimeStamp_Convert(endtime);
	//chose the column and table with different level
	if( type == 0)
	{
		GetTableName(Instrument_kind, table);
		strcpy(column_name, "Volume");
	}
	else
	{
		Get_table_min(Instrument_kind, table);
		strcpy(column_name, "AgrVolume");
	}
	sprintf(sql, "SELECT `%s` FROM `%s` \
WHERE `InstrumentID`=\"%s\"  and `TimeStamp` >=%ld and `TimeStamp` <=%ld ORDER BY `TimeStamp` ASC",
			column_name,
			table,
			Instrument,
			begin_stamp,
			end_stamp
					);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			int volume = atoi(m_row[0]);
			result.push_back(volume);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<int> Datafetch::get_OpenInterest(char *Instrument,
		char *begintime, char *endtime, int type)
{
	vector<int> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	time_t begin_stamp;
	time_t end_stamp;
	begin_stamp = TimeStamp_Convert(begintime);
	end_stamp = TimeStamp_Convert(endtime);
	//chose the table with different level
	if( type == 0)
	{
		GetTableName(Instrument_kind, table);
	}
	else
	{
		Get_table_min(Instrument_kind, table);
	}
	sprintf(sql, "SELECT `OpenInterest` FROM `%s` WHERE `InstrumentID`=\"%s\" \
and `TimeStamp` >=\"%ld\" and `TimeStamp`<=\"%ld\" ORDER BY `TimeStamp` ASC",
			table,
			Instrument,
			begin_stamp,
			end_stamp
					);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			int OpenInterset = atoi(m_row[0]);
			result.push_back(OpenInterset);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<double> Datafetch::get_BidPrice(char *Instrument,
		char *begintime, char *endtime, int num)
{
	vector<double> result;
	MYSQL_ROW m_row;
	if (num <= 0 || num > 5)
	{
		my_logger -> Logger_Warning("Price Number Must Be 1-5 ");
		return result;
	}
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	GetTableName(Instrument_kind, table);
	time_t begin_stamp;
	time_t end_stamp;
	begin_stamp = TimeStamp_Convert(begintime);
	end_stamp = TimeStamp_Convert(endtime);
	sprintf(sql, "SELECT `BidPrice%d` FROM `%s` WHERE `InstrumentID`=\"%s\" \
and `TimeStamp` >=%ld and `TimeStamp` <=%ld ORDER BY `TimeStamp` ASC",
		num,
		table,
		Instrument,
		begin_stamp,
		end_stamp
		);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double BidPrice = atof(m_row[0]);
			result.push_back(BidPrice);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<double> Datafetch::get_AskPrice(char *Instrument,
		char *begintime, char *endtime, int num)
{
	vector<double> result;
	MYSQL_ROW m_row;
	if (num <= 0 || num > 5)
	{
		my_logger -> Logger_Warning("Price Number Must Be 1-5 ");
		return result;
	}
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	GetTableName(Instrument_kind, table);
	time_t begin_stamp;
	time_t end_stamp;
	begin_stamp = TimeStamp_Convert(begintime);
	end_stamp = TimeStamp_Convert(endtime);
	sprintf(sql, "SELECT `AskPrice%d` FROM `%s` WHERE `InstrumentID`=\"%s\" \
and `TimeStamp` =%ld and `TimeStamp` =%ld ORDER BY `TimeStamp` ASC",
		num,
		table,
		Instrument,
		begin_stamp,
		end_stamp
		);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double OpenInterset = atof(m_row[0]);
			result.push_back(OpenInterset);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}


vector<double> Datafetch::get_HighestPrice(char *Instrument,
		char *begintime, char *endtime, int type)
{
	vector<double> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	if(type == 0)
	{
		Get_table_min(Instrument_kind, table);
		time_t begin_stamp;
		time_t end_stamp;
		begin_stamp = TimeStamp_Convert(begintime);
		end_stamp = TimeStamp_Convert(endtime);
		sprintf(sql, "SELECT `High` FROM `%s` WHERE `InstrumentID`=\"%s\" \
and `TimeStamp` >=%ld and `TimeStamp` >=%ld ORDER BY `TimeStamp` ASC",
			table,
			Instrument,
			begin_stamp,
			end_stamp
			);
	}
	else if (type == 1)
	{
		Get_table_day(Instrument_kind, table);
		sprintf(sql, "SELECT `HighestPrice` FROM `%s` WHERE `InstrumentID=\"%s\" \
and `TradingDay`>=\"%s\" and `TradingDay`<=\"%s\" ORDER BY `TradingDay` ASC",
			table,
			Instrument,
			begintime,
			endtime
			);
	}
	else
	{
		my_logger -> Logger_Error("HighestPrice: type must be 0,1");
	}
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double LowestPrice = atof(m_row[0]);
			result.push_back(LowestPrice);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<double> Datafetch::get_LowestPrice(char *Instrument,
		char *begintime, char *endtime, int type)
{
	vector<double> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	if (type == 0)
	{
		Get_table_min(Instrument_kind, table);
		time_t begin_stamp;
		time_t end_stamp;
		begin_stamp = TimeStamp_Convert(begintime);
		end_stamp = TimeStamp_Convert(endtime);
		sprintf(sql, "SELECT `Low` FROM `%s` WHERE `InstrumentID`=\"%s\" \
and `TimeStamp` >=%ld and `TimeStamp` >=%ld ORDER BY `TimeStamp` ASC",
			table,
			Instrument,
			begin_stamp,
			end_stamp
			);
	}
	else if(type == 1)
	{
		Get_table_day(Instrument_kind, table);
		sprintf(sql, "SELECT `LowestPrice` FROM `%s` WHERE `InstrumentID=\"%s\" \
and `TradingDay`>=\"%s\" and `TradingDay`<=\"%s\" ORDER BY `TradingDay` ASC",
			table,
			Instrument,
			begintime,
			endtime
			);
	}
	else
	{
		my_logger -> Logger_Error("LowestPrice: type must be 0,1");
	}
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double LowestPrice = atof(m_row[0]);
			result.push_back(LowestPrice);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<double> Datafetch::get_OpenPrice(char *Instrument,
		char *begintime, char *endtime)
{
	vector<double> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	Get_table_min(Instrument_kind, table);
	time_t begin_stamp;
	time_t end_stamp;
	begin_stamp = TimeStamp_Convert(begintime);
	end_stamp = TimeStamp_Convert(endtime);
	sprintf(sql, "SELECT `Open` FROM `%s` WHERE `InstrumentID`=\"%s\" \
and `TimeStamp` >=\"%ld\" and `TimeStamp` >=\"%ld\" ORDER BY `TimeStamp` ASC",
		table,
		Instrument,
		begin_stamp,
		end_stamp
		);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double LowestPrice = atof(m_row[0]);
			result.push_back(LowestPrice);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}

vector<double>  Datafetch::get_ClosePrice(char *Instrument, char *tradingday_start,
		char *tradingday_end)
{
	vector<double> result;
	MYSQL_ROW m_row;
	char sql[1000];
	char table[20];
	char Instrument_kind[3];
	Get_table_kind(Instrument_kind, Instrument);
	Get_table_day(Instrument_kind, table);
	sprintf(sql, "SELECT `ClosePrice` FROM `%s` \
WHERE `InstrumentID`=\"%s\"  and `TradingDay` >=\"%s\" \
and `TradingDay` <=\"%s\" ORDER BY `Tradingday` ASC",
		table,
		Instrument,
		tradingday_start,
		tradingday_end
		);
	int result_num =  get_sql_result(sql);
	if (result_num == 0)
	{
		while(m_row = mysql_fetch_row(_result))
		{
			double LowestPrice = atof(m_row[0]);
			result.push_back(LowestPrice);
		}
		mysql_free_result(_result);
	}
	else
	{
		my_logger -> Logger_Warning("Fetch Data Eorror");
	}
	return result;
}
