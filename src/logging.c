
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

void sta_log(Logger* logger, LoggingLevel level, const char* msg, ...)
{

  va_list args;
  va_start(args, msg);

  switch (level)
  {
  case LOGGING_LEVEL_INFO:
  {
    fprintf(stderr, "%sINFO:", ANSI_COLOR_GREEN);
    vfprintf(stderr, msg, args);
    break;
  }
  case LOGGING_LEVEL_WARNING:
  {
    fprintf(stderr, "%sWARNING:", ANSI_COLOR_YELLOW);
    vfprintf(stderr, msg, args);
    break;
  }
  case LOGGING_LEVEL_ERROR:
  {
    fprintf(stderr, "%sERROR:", ANSI_COLOR_RED);
    vfprintf(stderr, msg, args);
    break;
  }
  }
  va_end(args);
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
