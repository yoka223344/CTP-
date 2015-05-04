#include<iostream>
#include "strategy.h"
using namespace std;

int main()
{
	Strategy client = Strategy("127.0.0.1", "127.0.0.1", 9000, 9001);
	client.Connect(0);
	char Instrument[] = "cu1501";
	cout << client.send_data(Instrument, 0) <<endl;
}
