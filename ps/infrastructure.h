#ifndef _INFRA_GUARD
#define _INFRA_GUARD

#define LOGFILE	"_ps.log"
#define _MAX_BUFFER_LEN 256
#define _MAX_PROC_NUM 1024
#define _TIME_BETWEEN_PROC_CPU_CALC 1000

#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <tchar.h>

// An identifier for the log file creation
static BOOL LogCreated = FALSE;

// A struct that stores start-CPU-time for a single process 
struct process_cpu {
	double cpuUsage;
	ULARGE_INTEGER _lastCPU, _lastSysCPU, _lastUserCPU;
};

// A struct that stores a single process information
struct process_info {
	DWORD pid;
	TCHAR name[_MAX_BUFFER_LEN];
	TCHAR owner[_MAX_BUFFER_LEN];
	TCHAR ownerDomain[_MAX_BUFFER_LEN];
	SIZE_T workingSetSize;
	double cpuUsage;
};

// aliasing:
typedef struct process_cpu _PROCESS_CPU;
typedef struct process_info _PROCESS_INFO;

#endif