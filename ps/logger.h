#ifndef _LOG_GUARD
#define _LOG_GUARD

#pragma warning(disable:4996)
#include "infrastructure.h"

// opens the log file in append mode (or opens a new one if log-file doesn't exists (LogCreated = FALSE))
// and writes the message.
void _LogMsg(char *message);

// logs a warning message, and concat the last sys error code
void LogWarningMassage(char* msg);

#endif
