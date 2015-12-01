#include "logger.h"

void _LogMsg(char *message) {
	FILE *logFile;

	if (!LogCreated) {
		logFile = fopen(LOGFILE, "w");
		LogCreated = TRUE;
	}
	else
		logFile = fopen(LOGFILE, "a+");
	if (logFile == NULL) {
		if (LogCreated)
			LogCreated = FALSE;
		return;
	}
	else
		fputs(message, logFile);
}

void LogWarningMassage(char* msg) {
	DWORD eNum;
	char sysMsg[_MAX_BUFFER_LEN];

	eNum = GetLastError();
	sprintf(sysMsg, "\n WARNING: %s failed with error %d", msg, eNum);
	_LogMsg(sysMsg);
}