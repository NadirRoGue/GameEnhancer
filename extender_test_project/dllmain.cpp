/*
 * author: BiggBoss
 */
#include "stdafx.h"

__declspec(dllexport) BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH) {
		if(L2ExeExists()) {
			DisableThreadLibraryCalls(hModule);
			ls_port = -1;
			gs_port = -1;
			gs_crypt.enabled = FALSE;
			client_crypt.enabled = FALSE;
			PerformHooks();
			InitializePacketHandler();
			InitializePacketHandlers();
		} else
			exit(0);
	}
	
	return TRUE;
}