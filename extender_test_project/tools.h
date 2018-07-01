#ifndef TOOLS_H_
#define TOOLS_H_

#include <Windows.h>

#define BUFSIZE 128

VOID GetHwidMac(CCHAR *dst);
BOOL CheckProcessses();
BOOL L2ExeExists();
// vote plug in communication handlers
DWORD WINAPI ThreadWarpPipeHandler(LPVOID starting_site);
VOID PipeHandler(INT starting_site);

#endif