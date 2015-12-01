#ifndef _CPU_GUARD
#define _CPU_GUARD
#include "infrastructure.h"

// determine the machine number of processors
void GetNumOfProcessors(DWORD* numOfProcessors);

// init start cpu-time for a simgle process, and store it in  processesCpu
void InitProcessCPUTimes(_PROCESS_CPU* processesCpu, HANDLE hProcess);

// init start cpu-time for ALL processes
BOOL InitProcessesCpu(int processesCount, DWORD* pProcessIds, _PROCESS_CPU* processesCpu);

// calculate the total process cpu time (usage) for a simgle process by:
//		(currentSysCpu - lastSysCpu) + (currentUserCpu - lastUserCpu)
//   -------------------------------------------------------------------
//            (currentCpuTimes - lastCpuTimes) * numOfProcessors
void CalculateProcessCPU(_PROCESS_CPU* processesInfo, HANDLE hProcess, DWORD numProcessors);

// calculate the total process cpu time for ALL processes
UpdateProcessesCPU(int processesCount, _PROCESS_CPU* processesCpu, DWORD* pProcessIds);

#endif