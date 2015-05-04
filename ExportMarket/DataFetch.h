#include <mysql/mysql.h>
#include <string.h>
#include <vector>
#include "ClientBase.h"
using namespace std;
class Datafetch
{
private:
	MYSQL _mydata;         ///< 数据库连接句柄
    MYSQL_RES* _result;    ///< query结果的存储
	MYSQL_FIELD *_fd;
public:
	int get_sql_result(char *sql);
	void close_connect();
	bool connect(char *host = "localhost",
		char *user = "root",
		char *passwd = "abc0cba",
		char *database = "FutureMarket",
		int port = 3306);
    void set_node_data(char *InstrumentID, ClientBase *myClient);
	// begintime and endtime format is %Y%m%d%H:%M:%S ps:2015041511:13:00
	//Tick level and Min level closeprice;type 0 : Tick; 1 : Min
	vector<double> get_ClosePrice(char *Instrument, char *begintime, char *endtime, int type);
	//Tick level and Min level Volume;type 0 : Tick; 1 : Min
	vector<int> get_Volume(char *Instrument, char *begintime, char *endtime, int type);
	//Tick level and Min level OpenInterest;type 0 : Tick; 1 : Min
	vector<int> get_OpenInterest(char *Instrument, char *begintime, char *endtime, int type);
	//TICK level;num : the num-th Price num =1~5
	vector<double> get_BidPrice(char *Instrument, char *begintime, char *endtime, int num);
	vector<double> get_AskPrice(char *Instrument, char *begintime, char *endtime,int num);
	// get Min and day level highestPrice; type = 0 is min ,type = 1 is day
	vector<double> get_HighestPrice(char *Instrument, char *begintime, char *endtime, int type);
	// get Min and day level lowestPrice; type = 0 is min ,type = 1 is day
	vector<double> get_LowestPrice(char *Instrument, char *begintime, char *endtime, int type);
	// get Min level OpenPrice
	vector<double> get_OpenPrice(char *Instrument, char *begintime, char *endtime);
	// get Day level ClosPirce the tradingday fromat is %Y%m%d ps:20150415
	vector<double> get_ClosePrice(char *Instrument, char *tradingday_start, char *tradingday_end);
};
