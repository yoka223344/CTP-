#ifndef _Node_ClientBase_H_
#define _Node_ClientBase_H_
#include<iostream>
using namespace std;
class Node
{
	public:
		//static int count;
		double Last;
		double High;
		double Low;
		double Open;
		double Close;
		int OpenInterest;
		int Volume;
		int ArgVolume;
		char Day[9];
		char Time[11];
		Node *Next;
		Node *Front;
		Node()
		{
			Volume = 0;
			ArgVolume = 0;
			Next = NULL;
			Front = NULL;
		}
		void reset(Node *node);
		void UpdateNode(Node *node);
};
class ClientBase
{
	private:
		template<class T> int getData(T *result, T Node::* p, int size);
	public:
	    int node_number;
		Node *NodeList;
		ClientBase(int MaxSize = 1000);
		int getOpen(double *result, int size);
		int getClose(double *result, int size);
		int getHigh(double *result, int size);
		int getLow(double *result, int size);
		int getVolume(int *result, int size);
		int getargvolume(int *result, int size);
		void UpdateData(Node node);
		~ClientBase();
};
#endif
