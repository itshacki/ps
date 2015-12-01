#include "userHandler.h"
#include "logger.h"

BOOL GetLogonFromToken(HANDLE hToken, _PROCESS_INFO* processInfo) {
	DWORD dwSize, dwLength;
	PTOKEN_USER ptu = NULL;
	if (!GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, 0, &dwLength)) {
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			LogWarningMassage("GetLogonFromToken->GetTokenInformation");
			return FALSE;
		}
		ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLength);
		if (ptu == NULL) {
			LogWarningMassage("GetLogonFromToken->PtuHeapAlloc");
			return FALSE;
		}
	}
	if (!GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, dwLength, &dwLength)) {
		LogWarningMassage("GetLogonFromToken->GetTokenInformation");
		HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
		return FALSE;
	}
	SID_NAME_USE SidType;
	if (!LookupAccountSid(NULL, ptu->User.Sid, processInfo->owner, &dwSize, processInfo->ownerDomain, &dwSize, &SidType)) {
		LogWarningMassage("GetLogonFromToken->LookupAccountSid");
		HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
		return FALSE;
	}
	HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
	return TRUE;
}

BOOL PopulateProcessOwner(const DWORD procId, _PROCESS_INFO* processInfo)
{
	HANDLE hProcess;
	HANDLE hToken = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procId);
	if (hProcess == NULL) {
		LogWarningMassage("GetUserFromProcess->OpenProcess");
		return FALSE;
	}
	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
		LogWarningMassage("GetUserFromProcess->OpenProcessToken");
		CloseHandle(hProcess);
		return FALSE;
	}
	GetLogonFromToken(hToken, processInfo);
	CloseHandle(hToken);
	CloseHandle(hProcess);
	return TRUE;
}