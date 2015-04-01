/*************************************************************************
    > File Name: Logger.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Wed 18 Mar 2015 04:56:25 PM UTC
 ************************************************************************/

#include<iostream>
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/FileAppender.hh"
using namespace std;
class Logger
{

	private :
		log4cpp::PatternLayout* _pLayout;
		log4cpp::Appender *_FileAppender;
		int _priority;
	public:
		Logger(char * str, int priority);
		void Logger_Error(char *str);
		void Logger_Warning(char *str);
		void Logger_Fatal(char *str);
		void Logger_Info(char *str);
		void Logger_Error(const char *str);
		void Logger_Warning(const char *str);
		void Logger_Fatal(const char *str);
		void Logger_Info(const char *str);
};
