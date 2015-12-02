
#include "infrastructure.h"

void PrintHeader() {
	printf("\n------------------------------------------------------------------------");
	printf("\nNAME\t\tPID\tOWNER(User)\tOwnerDomain\tCPU\tMemory\n");
	printf("\n------------------------------------------------------------------------\n");
}

void PrintProcessInfo(_PROCESS_CPU processesCpu, _PROCESS_INFO processesInfo) {
	wprintf(TEXT("%s\t\t%d\t%s\t%s"), processesInfo.name, processesInfo.pid,
		processesInfo.owner, processesInfo.ownerDomain);
	printf("\t%1.3f %%\t\t%d KB\n", processesCpu.cpuUsage, processesInfo.workingSetSize);
}

BOOL InitProcessesID(DWORD* pProcessIds, DWORD pIdSize, DWORD* processesCount) {
	DWORD pBytesReturned;
	if (!EnumProcesses(pProcessIds, sizeof(*pProcessIds)*pIdSize, &pBytesReturned)) {
		LogWarningMassage("InitProcessesID->EnumProcesses");
		return (FALSE);
	}
	*processesCount = pBytesReturned / sizeof(DWORD);
	return TRUE;
}

void InitProcessInfo(_PROCESS_INFO* processInfo, DWORD pid) {
	processInfo->pid = pid;
	processInfo->name[0] = '\0';
	processInfo->owner[0] = '\0';
	processInfo->ownerDomain[0] = '\0';
	processInfo->workingSetSize = 0;
}

BOOL WalkThroughProcesses(int processesCount, _PROCESS_CPU* processesCPU, DWORD* pProcessIds) {
	HANDLE hProcess;
	HANDLE hProcessSnap;
	_PROCESS_INFO processInfo;
	PROCESSENTRY32 pe32;
	int index;

	PrintHeader();
	TakeProcessesSnap(&hProcessSnap, &pe32);
	for (index = 0; index < processesCount; index++) {
		InitProcessInfo(&processInfo, pProcessIds[index]);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pProcessIds[index]);
		if (hProcess == NULL) {
			LogWarningMassage("WalkThroughProcesses->OpenProcess");
			CloseHandle(hProcess);
		}
		PopulateProcesseName(&processInfo, pe32);
		PopulateWorkingSetSize(&processInfo, hProcess);
		PopulateProcessOwner(pProcessIds[index], &processInfo);
		Process32Next(hProcessSnap, &pe32);
		CloseHandle(hProcess);
		PrintProcessInfo(processesCPU[index], processInfo);
	}
	CloseHandle(hProcessSnap);
	return TRUE;
}

int main() {
	DWORD pProcessIds[_MAX_PROC_NUM], processesCount;
	_PROCESS_CPU processesCpu[_MAX_PROC_NUM];
	
	// init process identifier for each process object in the system
	if (!InitProcessesID(pProcessIds, _MAX_PROC_NUM, &processesCount))
		return TRUE;
	// init all processes times in order to store start-time
	if (!InitProcessesCpu(processesCount, pProcessIds, processesCpu))
		return TRUE;
	// calculate and store all processes cpu time (usage)
	if (!UpdateProcessesCPU(processesCount, processesCpu, pProcessIds))
		return TRUE;
	// move on every process, get its parameters and display them
	if (!WalkThroughProcesses(processesCount, processesCpu, pProcessIds))
		return TRUE;
	
	return (FALSE);
}