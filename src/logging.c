
#include "logging.h"
#include "common.h"
#include "string.h"
#include <stdarg.h>
#include <stdio.h>

Logger             GlobalLogger;

static inline void sta_writeToLogFile(Logger* logger, String msg)
{
  if (logger && logger->filePtr)
  {
    fprintf(logger->filePtr, "%.*s\n", (i32)msg.len, msg.buffer);
  }
}
static inline void sta_sendLogMessageVar(Logger* logger, const char* msg, char* color, va_list args)
{
  fprintf(stderr, "%s", color);
  vfprintf(stderr, msg, args);
}

static inline void sta_sendLogMessage(Logger* logger, String msg, char* color, ...)
{
  fprintf(stderr, "%s%.*s\n", color, (i32)msg.len, msg.buffer);
  sta_writeToLogFile(logger, msg);
}

void sta_logVar(Logger* logger, LoggingLevel level, const char* msg, ...)
{

  va_list args;
  va_start(args, msg);

  switch (level)
  {
  case LOGGING_LEVEL_INFO:
  {
    sta_sendLogMessageVar(logger, msg, ANSI_COLOR_GREEN, args);
    break;
  }
  case LOGGING_LEVEL_WARNING:
  {
    sta_sendLogMessageVar(logger, msg, ANSI_COLOR_YELLOW, args);
    break;
  }
  case LOGGING_LEVEL_ERROR:
  {
    sta_sendLogMessageVar(logger, msg, ANSI_COLOR_RED, args);
    break;
  }
  }
  va_end(args);
  printf(ANSI_COLOR_RESET);
}

void sta_log(Logger* logger, LoggingLevel level, String msg)
{
  switch (level)
  {
  case LOGGING_LEVEL_INFO:
  {
    sta_sendLogMessage(logger, msg, ANSI_COLOR_GREEN);
    break;
  }
  case LOGGING_LEVEL_WARNING:
  {
    sta_sendLogMessage(logger, msg, ANSI_COLOR_YELLOW);
    break;
  }
  case LOGGING_LEVEL_ERROR:
  {
    sta_sendLogMessage(logger, msg, ANSI_COLOR_RED);
    break;
  }
  }
  printf(ANSI_COLOR_RESET);
}

void sta_initGlobalLogger()
{
  GlobalLogger.filePtr = 0;
}

bool sta_initLogger(Logger* logger, String fileName)
{
  char file[256];
  strncpy(file, fileName.buffer, fileName.len);
  logger->filePtr = fopen(file, "a");
  return logger->filePtr != 0;
}

bool sta_destroyLogger(Logger* logger)
{
  return fclose(logger->filePtr);
}
