#include<iostream>
#include "Client.h"
#include <cstring>
using namespace std;

InstrumentNode::InstrumentNode(char *pInstrumentID, int ptype, int pspan, int Max_Size)
{
	strcpy(InstrumentID, pInstrumentID);
	type = ptype;
	span = pspan;
	timestamp = 0;
	myClientBase = new ClientBase(Max_Size);
	tmp_Node = new Node;
}

InstrumentNode::~InstrumentNode()
{
	delete myClientBase;
	delete tmp_Node;
}

void Client::AddInstrument(char *InstrumentID, int type, int span, int Max_Size)
{
	int span_sec = getSpan(type, span);
	InstrumentNode *pNode = new InstrumentNode(InstrumentID, type, span, Max_Size);
	if(mydatafetch -> connect())
    {
		pNode -> timestamp = mydatafetch -> set_node_data(InstrumentID, pNode -> myClientBase, span_sec);
        mydatafetch -> close_connect();
    }
    else
    {
        cerr << __FUNCTION__ << "DataBase DisConnect" << endl;
    }
	Instrument.push_back(pNode);
	cout << __FUNCTION__ << "FINISH" <<endl;
}

void Client::UpdateData(char *InstrumentID, Node *node, long timestamp)
{
	for(int i = 0; i < Instrument.size(); i++)
	{
		if (strcmp(Instrument[i] -> InstrumentID, InstrumentID) == 0)
		{
			int span_sec = getSpan(Instrument[i] -> type, Instrument[i] -> span);
			if(timestamp > Instrument[i] -> timestamp + span_sec)
			{
				Instrument[i] -> myClientBase -> UpdateData(*Instrument[i] -> tmp_Node);
				Instrument[i] -> tmp_Node -> reset(node);
				Instrument[i] -> timestamp = timestamp + span_sec; 
			}
			else
			{
				Instrument[i] -> tmp_Node -> UpdateNode(node);
			}

		}
	}
}

void Client::AddInstrument(const char *InstrumentID, int type, int span, int Max_Size)
{
	char pInstrumentID[11];
	strcpy(pInstrumentID, InstrumentID);
	AddInstrument(pInstrumentID, type, span, Max_Size);
}

bool Client::find(char *InstrumentID)
{
	char pInstrument[11];
	strcpy(pInstrument, InstrumentID);
	for (int i =0; i < Instrument.size(); i++)
	{
		if(!strcmp(Instrument[i] -> InstrumentID, pInstrument))
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
int Client::find(const char *InstrumentID, int type, int span)
{
	int count = -1;
	char pInstrument[11];
	strcpy(pInstrument, InstrumentID);
	for (int i = 0; i < Instrument.size(); i++)
	{
		if (strcmp(Instrument[i] -> InstrumentID, pInstrument) == 0\
				&& type == Instrument[i] -> type && span == Instrument[i] -> span)
		{
			count = i;
			break;
		}
	}
	return count;
}

int Client::getOpen(double *Open, char *InstrumentID, int type, int span, int open_size)
{
	int Index = find(InstrumentID, type, span);
	return Instrument[Index] -> myClientBase -> getOpen(Open, open_size); 
}

int Client::getClose(double *Close, char *InstrumentID, int type, int span, int close_size)
{
	int Index = find(InstrumentID, type, span);
	int span_of_type = getSpan(type, span);
	return Instrument[Index] -> myClientBase -> getClose(Close, close_size); 
}
int Client::getHigh(double *High, char *InstrumentID, int type, int span, int High_size)
{
	int Index = find(InstrumentID, type, span);
	int span_of_type = getSpan(type, span);
	return Instrument[Index] -> myClientBase -> getHigh(High, High_size);
}
int Client::getLow(double *Low, char *InstrumentID, int type, int span, int Low_size)
{
	int Index = find(InstrumentID, type, span);
	int span_of_type = getSpan(type, span);
	return Instrument[Index] -> myClientBase -> getLow(Low, Low_size);
}
int Client::getVolume(int *Volume, char *InstrumentID, int type, int span, int Volume_size)
{
	int Index = find(InstrumentID, type, span);
	int span_of_type = getSpan(type, span);
	return Instrument[Index] -> myClientBase -> getVolume(Volume, Volume_size);
}
int Client::getargVolume(int *argVolume, char *InstrumentID, int type, int span, int argVolume_size)
{
	int Index = find(InstrumentID, type, span);
	int span_of_type = getSpan(type, span);
	return Instrument[Index] -> myClientBase -> getargvolume(argVolume, argVolume_size);
}
void Client::DeleteInstrument(int InstrumentNodeNumber)
{
	vector<InstrumentNode*>::iterator itr = Instrument.begin();
	delete Instrument[InstrumentNodeNumber - 1];
	Instrument.erase(itr + InstrumentNodeNumber);
}
