#pragma once
 
#include <cstdio>
 
class Logger {
	private:
		Logger();
		~Logger();
 
		FILE* logFile;
 
		static Logger *theInstance;
	public:
		static Logger *getInstance();
		static void release();
		bool log(char* _log, ...);
};