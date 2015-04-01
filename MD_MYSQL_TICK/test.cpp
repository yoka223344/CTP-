/*************************************************************************
    > File Name: test.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Fri 27 Mar 2015 11:07:43 AM UTC
 ************************************************************************/

#include <iostream>
#include <string.h>
//#include "MdSpi.cpp"
using namespace std;

int main()
{
//	while(1)
//	{
	char table[15];
	char str1[9] = "zn1601";
	//char str3[9] = "p1601";
	char str2[3];
	if ( (int)(str1[1]-'A') >= 0 )
	{
		char lin[3] = {str1[0], str1[1], '\0'};
		cout << lin <<endl;
		strcpy(str2, lin);
		cout << "1 " << str2 <<endl;
	}
	else
	{
		char lin[3] = {str1[0], '\0', '\0'};
		strcpy(str2, lin);
		cout << "2 " << str2 << endl;

	}
	//GetTableName(str2, table);
	//cout<< table <<endl;
	char str3[7];
	strcpy(str3, str2);
	strcat(str3, "0001");
	cout << "3" << str3 << endl;
	/*char table_1[19];
	strcpy(table_1, table);
	strcat(table_1,"_MIN");
	cout<< table_1  <<" "<< table <<endl;
	char table_2[19];
	strcpy(table_2, table);
	strcat(table_2,"_DAY");
	cout<< table_2  <<" "<< table <<endl;
*/
//	}
}

