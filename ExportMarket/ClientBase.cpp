#include<iostream>
#include <cfloat>
#include <cstring>
#include "ClientBase.h"
using namespace std;
ClientBase::ClientBase(int MaxSize)
{
	node_number = MaxSize;
	NodeList = new Node;
	Node *pNode = NodeList;
	for(int i = 1; i < MaxSize; i++)
	{
		Node *t_Node = new Node;
		pNode -> Next = t_Node;
		t_Node -> Front = pNode;
		pNode = pNode -> Next;
		pNode -> Next =NULL;
	}
	pNode -> Next = NodeList;
	NodeList -> Front = pNode;
}
void ClientBase::UpdateData(Node node)
{
	NodeList = NodeList -> Front;
    NodeList -> Last = node.Last;
    NodeList -> OpenInterest = node.OpenInterest;
    NodeList -> ArgVolume = node.ArgVolume;
    strcpy(NodeList ->Day, node.Day);
    strcpy(NodeList ->Time, node.Time);
    NodeList -> TimeStamp = node.TimeStamp;
}
template<class T> int ClientBase::getData(T *result, T Node:: *p, int span, int size)
{
	int length = size;
	Node *pNode = NodeList;
	long tmp_timestamp = pNode -> TimeStamp;
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		result[i] =  *pNode.*p;
		tmp_timestamp -= span;
		++count;
		while(pNode -> TimeStamp > tmp_timestamp)
		{
			pNode = pNode -> Next;
			if (pNode == NodeList)
				break;
		}
		if (pNode == NodeList)
			break;
	}
	return count;
}
template<class T> int ClientBase::getData(T *result, T Node:: *p, int size)
{
	Node *pNode = NodeList;
	int count = 0;
	while(count < size && pNode->Next != NodeList)
	{
		result[count] = *pNode.*p;
		count++;
		pNode = pNode -> Next;
	}
	return count;
}
int ClientBase::getOpen(double *result, int span, int size)
{
	if(span == 0)
	{
		double Node::*offsetLast = &Node::Last;
		return getData(result, offsetLast, size);
	}
    int length = size;
	Node *pNode = NodeList;
	long tmp_timestamp = pNode -> TimeStamp - span;
	double Open = 0;
	int count = 0;
    while(pNode ->Next != NodeList && count < length)
    {
        if( tmp_timestamp >= pNode -> TimeStamp )
        {
            Open =  pNode ->Front -> Last;
            result[count] = Open;
            count ++;
            tmp_timestamp -= span;
        }
        pNode = pNode -> Next;
    }
	return count;
}
int ClientBase::getClose(double *result, int span, int size)
{
	if(span == 0)
	{
		double Node::*offsetLast = &Node::Last;
		return getData(result, offsetLast, size);
	}
    int length = size;
	Node *pNode = NodeList;
	long tmp_timestamp = pNode -> TimeStamp;
	double Close = 0;
	int count = 0;
    while(pNode -> Next != NodeList && count < size)
    {
        if( tmp_timestamp <= pNode -> TimeStamp )
        {
            Close =  pNode -> Last;
            result[count] = Close;
            count ++;
        }
        pNode = pNode -> Next;
    }
	return count;
}
int ClientBase::getHigh(double *result, int span, int size)
{
	if(span == 0)
	{
		double Node::*offsetLast = &Node::Last;
		return getData(result, offsetLast, size);
	}
    int length = size;
	Node *pNode = NodeList;
	long tmp_timestamp = pNode -> TimeStamp;
	double High = 0;
	int count = 0;
    while(pNode -> Next != NodeList && count < length)
    {
        if (High < pNode -> Last)
            High = pNode -> Last;
        if( tmp_timestamp >= pNode -> TimeStamp )
        {
            result[count] = High;
            count ++;
        }
        pNode = pNode -> Next;
    }
	return count;
}
int ClientBase::getLow(double *result, int span, int size)
{
	if(span == 0)
	{
		double Node::*offsetLast = &Node::Last;
		return getData(result, offsetLast, size);
	}
    int length = size;
	Node *pNode = NodeList;
	long tmp_timestamp = pNode -> TimeStamp;
	double Low = DBL_MAX ;
	int count = 0;
    while(pNode -> Next != NodeList && count < length)
    {
        if (Low > pNode -> Last)
            Low = pNode -> Last;
        if( tmp_timestamp >= pNode -> TimeStamp )
        {
            result[count] = Low;
            count ++;
        }
        pNode = pNode -> Next;
    }
	return count;
}
int ClientBase::getVolume(int *result, int span, int size)
{
	int count = 0;
	if(span == 0)
	{
		int Node::*offsetVolume = &Node::ArgVolume;
		count = getData(result, offsetVolume, size);
	}
	else
	{

		int length = size;
		int Node::*offsetVolume = &Node::ArgVolume;
		count = getData(result, offsetVolume, span, size);
	}
	for (int i = 0; i < count - 1; i++)
    {
        result[i] -= result[i+1];
    }
	return count;
}
int ClientBase::getargvolume(int *result, int span, int size)
{
	int count = 0;
	int Node::*offsetargVolume = &Node::ArgVolume;
	if(span == 0)
	{
		count = getData(result, offsetargVolume, span, size);
	}
	count = getData(result, offsetargVolume, span, size);
	return count;
}
ClientBase::~ClientBase()
{
	Node *pNode = NodeList;
	while(pNode -> Next != NodeList)
	{
		Node *p = pNode;
		pNode = pNode -> Next;
		delete p;
	}
	delete NodeList;
	NodeList = NULL;
	/*
	for(int i = 0; i < node_number; i++)
	{
		Node *t_Node = pNode -> Next;
		if (pNode != NULL)
			delete pNode;
		pNode = t_Node;
	}*/
}
