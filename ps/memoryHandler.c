#include "memoryHandler.h"

BOOL PopulateWorkingSetSize(_PROCESS_INFO* processesInfo, HANDLE hProcess) {
	PROCESS_MEMORY_COUNTERS pmc;
	if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
		LogWarningMassage("PopulateWorkingSetSize -> GetProcessMemoryInfo");
		CloseHandle(hProcess); //refactor
		return(FALSE);
	}
	processesInfo->workingSetSize = (pmc.WorkingSetSize / _MAX_PROC_NUM);
}