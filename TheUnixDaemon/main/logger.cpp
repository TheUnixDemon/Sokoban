#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include "logger.hpp"
 
Logger *Logger::theInstance = NULL;
 
Logger::Logger() {
	logFile = fopen("log.txt", "wt");
}
 
Logger::~Logger() {
	if (logFile) {
		fflush(logFile);
		fclose(logFile);
		logFile = NULL;
	}
}
 
Logger* Logger::getInstance() {
	if (theInstance == NULL) {
		theInstance = new Logger();
	}
	return theInstance;
}
 
void Logger::release() {
	if (theInstance != NULL) {
		delete theInstance;
	}
	theInstance = NULL;
}
 
bool Logger::log(char* _log, ...) {
	timeval curTime;
	char buffer [80];
	char currentTime[84] = "";
	char acText[1024];
	va_list VAList;
 
	va_start(VAList, _log);
	vsprintf(acText, _log, VAList);
	va_end(VAList);

	gettimeofday(&curTime, NULL);
	int milli = curTime.tv_usec / 1000;
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));
	sprintf(currentTime, "%s:%d", buffer, milli);

	fprintf(Logger::logFile, "(%s): %s\n",
				currentTime, acText);
 
	return true;
}