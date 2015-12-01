#ifndef _NAME_GUARD
#define _NAME_GUARD
#include "infrastructure.h"

// stores process name in processesInfo, by populated PROCESSENTRY32 structure 
void PopulateProcesseName(_PROCESS_INFO* processesInfo, PROCESSENTRY32 pe32);

// Take a snapshot of the specified processes,
// as well as the heaps, modules, and threads used by these processes by using CreateToolhelp32Snapshot
BOOL TakeProcessesSnap(HANDLE* hProcessSnap, PROCESSENTRY32* pe32);

#endif