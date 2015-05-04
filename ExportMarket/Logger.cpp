#include<iostream>
#include "log4cpp/Category.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/FileAppender.hh"
#include "Logger.h"
using namespace std;

Logger::Logger(char * str, int priority)
{
	log4cpp::Category&root = log4cpp::Category::getRoot();
	_FileAppender = new log4cpp::FileAppender("FileAppender",str);
	_pLayout = new log4cpp::PatternLayout();
	_pLayout -> setConversionPattern("%d: %p %c %x: %m%n");
	_FileAppender -> setLayout(_pLayout);
	_priority = priority;
};

void Logger::Logger_Error(char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.error(str);
}

void Logger::Logger_Warning(const char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.warn(str);
}

void Logger::Logger_Info(const char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.info(str);
}

void Logger::Logger_Fatal(const char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.fatal(str);
}
void Logger::Logger_Error(const char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.error(str);
}

void Logger::Logger_Warning(char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.warn(str);
}

void Logger::Logger_Info(char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.info(str);
}

void Logger::Logger_Fatal(char *str)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(_FileAppender);
	root.setPriority(_priority);
	root.fatal(str);
}
