#ifndef _MEM_GUARD
#define _MEM_GUARD
#include "infrastructure.h"

// stores process memory usage in processesInfo, by using GetProcessMemoryInfo func
BOOL PopulateWorkingSetSize(_PROCESS_INFO* processesInfo, HANDLE hProcess);

#endif