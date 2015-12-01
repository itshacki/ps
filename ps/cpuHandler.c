#include "cpuHandler.h"
#include "logger.h"

void GetNumOfProcessors(DWORD* numOfProcessors) {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	*numOfProcessors = sysInfo.dwNumberOfProcessors;
}

void InitProcessCPUTimes(_PROCESS_CPU* processesCpu, HANDLE hProcess) {
	FILETIME ftime, fsys, fuser;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&processesCpu->_lastCPU, &ftime, sizeof(FILETIME));
	GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
	memcpy(&processesCpu->_lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&processesCpu->_lastUserCPU, &fuser, sizeof(FILETIME));
}

BOOL InitProcessesCpu(int processesCount, DWORD* pProcessIds, _PROCESS_CPU* processesCpu) {
	HANDLE hProcess;
	int index;
	for (index = 0; index < processesCount; index++) {
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pProcessIds[index]);
		if (hProcess == NULL) {
			LogWarningMassage("InitProcessesCpu->OpenProcess");
			CloseHandle(hProcess);
		}
		InitProcessCPUTimes(&processesCpu[index], hProcess);
		CloseHandle(hProcess);
	}
	return TRUE;
}

void CalculateProcessCPU(_PROCESS_CPU* processesInfo, HANDLE hProcess, DWORD numProcessors) {
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));
	GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));

	percent = (sys.QuadPart - processesInfo->_lastSysCPU.QuadPart) +
		(user.QuadPart - processesInfo->_lastUserCPU.QuadPart);
	percent /= (now.QuadPart - processesInfo->_lastCPU.QuadPart);
	percent /= numProcessors;

	processesInfo->cpuUsage = percent * 100;
}

BOOL UpdateProcessesCPU(int processesCount, _PROCESS_CPU* processesCpu, DWORD* pProcessIds) {
	HANDLE hProcess;
	DWORD numOfProcessors;
	int index;

	GetNumOfProcessors(&numOfProcessors);
	Sleep(_TIME_BETWEEN_PROC_CPU_CALC);
	for (index = 0; index < processesCount; index++) {
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pProcessIds[index]);
		if (hProcess == NULL) {
			LogWarningMassage("UpdateProcessesCPU->OpenProcess");
			CloseHandle(hProcess);
		}
		CalculateProcessCPU(&processesCpu[index], hProcess, numOfProcessors);
		CloseHandle(hProcess);
	}
	return TRUE;
}