#ifndef STA_LOGGING_H
#define STA_LOGGING_H

#include "string.h"
#include <stdio.h>

struct Logger
{
  FILE* filePtr;
};
typedef struct Logger Logger;

extern Logger         GlobalLogger;

enum LoggingLevel
{
  LOGGING_LEVEL_INFO,
  LOGGING_LEVEL_WARNING,
  LOGGING_LEVEL_ERROR,
};
typedef enum LoggingLevel LoggingLevel;

void                      sta_log(Logger* logger, LoggingLevel level, String msg);

void                      sta_initGlobalLogger();
void sta_logVar(Logger* logger, LoggingLevel level, const char* msg, ...);
bool                      sta_initLogger(Logger* logger, String fileName);
bool                      sta_destroyLogger(Logger* logger);

#endif
