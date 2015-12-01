#ifndef _USER_GUARD
#define _USER_GUARD
#include "infrastructure.h"

//retrieve a specified type of information about an access token twice (second time after dwLength populated)
// and retrieve the name of the account for this SID and
// the name of the first domain on which this SID is found, by using LookupAccountSid
BOOL GetLogonFromToken(HANDLE hToken, _PROCESS_INFO* processInfo);

// Gets a token with OpenProcessToken and pass it to GetLogonFromToken in order to store it at processInfo
BOOL PopulateProcessOwner(const DWORD procId, _PROCESS_INFO* processInfo);

#endif