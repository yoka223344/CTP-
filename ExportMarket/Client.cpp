#include<iostream>
#include "Client.h"
#include <cstring>
using namespace std;

InstrumentNode::InstrumentNode(char *pInstrumentID, int ptype)
{
	strcpy(InstrumentID, pInstrumentID);
	type = ptype;
}
void Client::AddInstrument(char *InstrumentID, int type, int Max_Size)
{
	InstrumentNode pNode(InstrumentID, type);
	Instrument.push_back(pNode);
	ClientBase *clientNode = new ClientBase(Max_Size);
	if(mydatafetch -> connect())
    {
        mydatafetch -> set_node_data(InstrumentID, clientNode);
        mydatafetch -> close_connect();
    }
    else
    {
        cerr << __FUNCTION__ << "DataBase DisConnect" << endl;
    }
    myClientBase.push_back(clientNode);
	cout << __FUNCTION__ << "FINISH" <<endl;
}
void Client::UpdateData(char *InstrumentID, Node node)
{
	for(int i = 0; i < Instrument.size(); i++)
	{
		if (strcmp(Instrument[i].InstrumentID, InstrumentID) == 0)
		{
			myClientBase[i] -> UpdateData(node);
		}
	}
}

void Client::AddInstrument(const char *InstrumentID, int type, int Max_Size)
{
	char pInstrumentID[11];
	strcpy(pInstrumentID, InstrumentID);
	AddInstrument(pInstrumentID, type, Max_Size);
}

bool Client::find(char *InstrumentID)
{
	char pInstrument[11];
	strcpy(pInstrument, InstrumentID);
	for (int i =0; i < Instrument.size(); i++)
	{
		if(!strcmp(Instrument[i].InstrumentID, pInstrument))
		{
			return true;
		}
	}
	return false;
}

int Client::getSpan(int type, int span)
{
	if(type == 1)
		return span*60;
	if(type == 2)
		return span*60*60*24;
	return span;
}
int Client::find(const char *InstrumentID, int type)
{
	int count = -1;
	char pInstrument[11];
	strcpy(pInstrument, InstrumentID);
	for (int i = 0; i < Instrument.size(); i++)
	{
		if (strcmp(Instrument[i].InstrumentID, pInstrument) == 0\
				&& type == Instrument[i].type)
		{
			count = i;
			break;
		}
	}
	return count;
}

int Client::getOpen(double *Open, char *InstrumentID, int type, int span, int open_size)
{
	int Index = find(InstrumentID, type);
	int span_of_type = getSpan(type, span);
	return myClientBase[Index] -> getOpen(Open, span_of_type, open_size); 
}

int Client::getClose(double *Close, char *InstrumentID, int type, int span, int close_size)
{
	int Index = find(InstrumentID, type);
	int span_of_type = getSpan(type, span);
	return myClientBase[Index] -> getClose(Close, span_of_type, close_size); 
}
int Client::getHigh(double *High, char *InstrumentID, int type, int span, int High_size)
{
	int Index = find(InstrumentID, type);
	int span_of_type = getSpan(type, span);
	return myClientBase[Index] -> getHigh(High, span_of_type, High_size);
}
int Client::getLow(double *Low, char *InstrumentID, int type, int span, int Low_size)
{
	int Index = find(InstrumentID, type);
	int span_of_type = getSpan(type, span);
	return myClientBase[Index] -> getLow(Low, span_of_type, Low_size);
}
int Client::getVolume(int *Volume, char *InstrumentID, int type, int span, int Volume_size)
{
	int Index = find(InstrumentID, type);
	int span_of_type = getSpan(type, span);
	return myClientBase[Index] -> getVolume(Volume, span_of_type, Volume_size);
}
int Client::getargVolume(int *argVolume, char *InstrumentID, int type, int span, int argVolume_size)
{
	int Index = find(InstrumentID, type);
	int span_of_type = getSpan(type, span);
	return myClientBase[Index] -> getargvolume(argVolume, span_of_type, argVolume_size);
}
void Client::DeleteInstrument(int InstrumentNodeNumber)
{
	vector<InstrumentNode>::iterator itr = Instrument.begin();
	vector<ClientBase*>::iterator ctr = myClientBase.begin();
	Instrument.erase(itr + InstrumentNodeNumber);
	delete myClientBase[InstrumentNodeNumber - 1];
	myClientBase.erase(ctr + InstrumentNodeNumber);
}
