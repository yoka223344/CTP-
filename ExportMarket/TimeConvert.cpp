/*************************************************************************
    > File Name: TimeConvert.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 13 Apr 2015 02:07:32 PM UTC
 ************************************************************************/

#include<iostream>
#include <ctime>
#include <cstring>
using namespace std;

time_t TimeStamp_Convert(char *day, char *time)
{
	tm ptr;
	time_t timestamp;
	char datetime[20];
	strcpy(datetime, day);
	strcat(datetime, time);
	datetime[16] = '\0';
	strptime(datetime, "%Y%m%d%H:%M:%S", &ptr);
	timestamp = mktime(&ptr);
	return timestamp;
}

