#include <ctime>
class StatNode
{
	int preVolume;
	struct tm begin_time;
public:
	char TradingDay[9];
	char InstrumentID[31];
	double High;
	double Low;
	double Open;
	double Close;
	double OpenPrice;
	double ClosePrice;
	int Volume;
	int agrVolume;
	double OpenInterset;
	char Updatetime[9];
	void init(char *Instr);
	void set_price(double num);
	void reset();
	void get_Volume();
	int Updatecomp(char *str);
	void daily_OpenClose(double open, double close);
};
