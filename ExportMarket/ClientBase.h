#ifndef _Node_ClientBase_H_
#define _Node_ClientBase_H_
#include<iostream>
using namespace std;
class Node
{
	public:
		//static int count;
		double Last;
		int OpenInterest;
		int ArgVolume;
		char Day[9];
		char Time[11];
		int TimeStamp;
		Node *Next;
		Node *Front;
		Node()
		{
			Next = NULL;
			Front = NULL;
			//cout << "create" << ++count <<endl;
		}
		/*~Node()
		{
			cout << "delete" << --count <<endl;
		}*/
};
class ClientBase
{
	private:
		template<class T> int getData(T *result, T Node::* p, int span, int size);
		template<class T> int getData(T *result, T Node::* p, int size);
	public:
	    int node_number;
		Node *NodeList;
		ClientBase(int MaxSize = 1000);
		int getOpen(double *result, int span, int size);
		int getClose(double *result, int span, int size);
		int getHigh(double *result, int span, int size);
		int getLow(double *result, int span, int size);
		int getVolume(int *result, int span, int size);
		int getargvolume(int *result, int span, int size);
		void UpdateData(Node node);
		~ClientBase();
};
#endif
