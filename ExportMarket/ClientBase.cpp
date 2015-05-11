#include<iostream>
#include <cfloat>
#include <cstring>
#include "ClientBase.h"
using namespace std;
void Node::reset(Node *node)
{
	Last = node -> Last;
	High = node -> Last;
	Low = node -> Last;
	Open = node -> Last;
	Close = node -> Last;
	OpenInterest = node -> OpenInterest;
	Volume = 0;
	ArgVolume = node -> ArgVolume;
}
void Node::UpdateNode(Node *node)
{
	Last = node -> Last;
	if (High < Last)
		High = Last;
	if (Low > Last)
		Low = Last;
	Close = Last;
	OpenInterest = node -> OpenInterest;
	ArgVolume = node -> ArgVolume;
}
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
	NodeList -> High = node.High;
	NodeList -> Low = node.Low;
	NodeList -> Open = node.Open;
	NodeList -> Close = node.Close;
	NodeList -> Volume = node.Volume;
    strcpy(NodeList ->Day, node.Day);
    strcpy(NodeList ->Time, node.Time);
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

int ClientBase::getOpen(double *result, int size)
{
	double Node::*offsetLast = &Node::Open;
	return getData(result, offsetLast, size);
}

int ClientBase::getClose(double *result, int size)
{
	double Node::*offsetLast = &Node::Close;
	return getData(result, offsetLast, size);
}

int ClientBase::getHigh(double *result, int size)
{
	double Node::*offsetLast = &Node::High;
	return getData(result, offsetLast, size);
}

int ClientBase::getLow(double *result, int size)
{
	double Node::*offsetLast = &Node::Low;
	return getData(result, offsetLast, size);
}

int ClientBase::getVolume(int *result, int size)
{
	int Node::*offsetVolume = &Node::Volume;
	return getData(result, offsetVolume, size);
}

int ClientBase::getargvolume(int *result, int size)
{
	int Node::*offsetargVolume = &Node::ArgVolume;
	return getData(result, offsetargVolume, size);
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
