#include "StatNode.h"
#include <string.h>
#include <ctime>
#include <iostream>
using namespace std;
/*StatNode::StatNode(char *Instr)
{
	strcpy(InstrumentID, Instr);
	High = 0;
	Low = 100000;
	Open = 0;
	Close = 0;
	Volume = 0;
	agrVolume = 0;
	preVolume = 0;
	OpenInterset = 0;
	strcpy(Updatetime, "00000000"); 
	strptime(Updatetime, "%H:%M:%S", &begin_time);
}*/
void StatNode::init(char * Instr)
{
	preVolume = 0;
	strcpy(InstrumentID, Instr);
	High = 0;
	Low = 100000;
	Open = 0;
	Close = 0;
	Volume = 0;
	agrVolume = 0;
	preVolume = 0;
	OpenInterset = 0;
	strcpy(Updatetime, "00000000");
	//cerr<<"init"<<endl;
	strptime(Updatetime, "%H:%M:%S", &begin_time);
	OpenPrice = 0;
	ClosePrice = 0;
	//cerr<<"success"<<endl;
}
void StatNode::reset()
{
	High = 0;
	Low = 100000;
	Open = 0;
	Close = 0;
	Volume = 0;
	preVolume = agrVolume;
	agrVolume = 0;
	OpenInterset = 0;

}
void StatNode::set_price(double num)
{
	if (High < num)
		High = num;
	if (Low > num)
		Low = num;
	Close = num;	
}
void StatNode::daily_OpenClose(double open, double close)
{
	if (open >= 0)
		OpenPrice = open;
	if (close >= 0)
		ClosePrice =close;	
}
void StatNode::get_Volume()
{
	Volume = agrVolume - preVolume;
}
int differ(struct tm time1, struct tm time2)
{
	int seconds = 0;
	long ctime1 = time1.tm_hour * 3600 + time1.tm_min * 60 + time1.tm_sec;
	long ctime2 = time2.tm_hour * 3600 + time2.tm_min * 60 + time2.tm_sec;
	if (time1.tm_hour - time2.tm_hour > 20)
		ctime2 += 24 * 3600;
	seconds = ctime2 - ctime1;
	return seconds;
}
int StatNode::Updatecomp(char *str)
{
	if (strcmp(Updatetime, "00000000") == 0)
	{	
		str[6] = '0';
		str[7] = '0';
		strcpy(Updatetime, str);
		strptime(Updatetime, "%H:%M:%S", &begin_time);
		return 0;
	}
	else if (strcmp(Updatetime, str) > 0)
		return -1;
	else
	{
		struct tm tmp_time;
		strptime( str, "%H:%M:%S", &tmp_time);
		if (differ(begin_time, tmp_time) >= 60)
		{
			str[6] = '0';
			str[7] = '0';
			strcpy(Updatetime, str);
			strptime(Updatetime, "%H:%M:%S", &begin_time);
			return 1;
		}
		else
		{
			return 2;
		}
		strcpy(Updatetime, str);
	}
}
