#include "nameHandler.h"

void PopulateProcesseName(_PROCESS_INFO* processesInfo, PROCESSENTRY32 pe32) {
	memcpy(processesInfo->name, pe32.szExeFile, sizeof(pe32.szExeFile));
}

BOOL TakeProcessesSnap(HANDLE* hProcessSnap, PROCESSENTRY32* pe32) {
	*hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (*hProcessSnap == INVALID_HANDLE_VALUE) {
		LogWarningMassage("TakeProcessesSnap->CreateToolhelp32Snapshot");
		return(FALSE);
	}
	pe32->dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(*hProcessSnap, pe32)) {
		LogWarningMassage("TakeProcessesSnap->Process32First");
		CloseHandle(*hProcessSnap);
		return(FALSE);
	}
	return TRUE;
}