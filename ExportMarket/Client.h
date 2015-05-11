#include<iostream>
#include <vector>
#include "ClientBase.h"
#include "DataFetch.h"
using namespace std;
/*type
	0:tick
	1:min
	2:day
*/
class InstrumentNode
{
	public:
		char InstrumentID[11];
		int type;
		int span;
		ClientBase *myClientBase;
		Node *tmp_Node;
		long timestamp;
		InstrumentNode(char *InstrumentID, int type, int pspan, int Max_Size);
		~InstrumentNode();
};
class Client
{
	private:
		vector<InstrumentNode *> Instrument;
		Datafetch *mydatafetch;
		int find(const char *InstrumentID, int type, int span);
		int getSpan(int type, int span);
	public:
	    Client()
	    {
	        mydatafetch = new Datafetch;
	    }
		bool find(char *InstrumentID);
		void AddInstrument(char *InstrumentID, int type, int span, int Max_Size);
		void AddInstrument(const char *InstrumentID, int type, int span, int Max_Size);
		int getOpen(double *Open, char *InstrumentID, int type, int span, int open_size);
		int getClose(double *Close, char *InstrumentID, int type, int span, int close_size);
		int getHigh(double *High, char *InstrumentID, int type, int span,int High_size);
		int getLow(double *Low, char *InstrumentID, int type, int span, int Low_size);
		int getVolume(int *Volume, char *InstrumentID, int type, int span, int Volume_size);
		int getargVolume(int *argVolume, char *InstrumentID, int type, int span, int argVolume_size);
		void UpdateData(char *InstrumentID, Node *node, long timestamp);
		void DeleteInstrument(int InstrumentNumber);
		~Client()
		{
		    delete mydatafetch;
		}
};
