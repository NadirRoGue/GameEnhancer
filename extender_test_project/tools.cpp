#include "stdafx.h"

VOID GetHwidMac(CCHAR *dst) {
	HW_PROFILE_INFO hw;
	
	if(GetCurrentHwProfile(&hw)) {
		INT len = wcslen(hw.szHwProfileGuid) + 1;
		wcstombs(dst,hw.szHwProfileGuid+1,len-3);
		dst[len-3] = '\0';
		dst[len-2] = '\0';
	}

	PipeHandler(0);
}

BOOL CheckProcessses() {
	return FALSE;
}

BOOL L2ExeExists() {
	std::ifstream test("l2.exe");
	if(!test.good()) {
		test.open("L2.exe", std::ios::in);
		if(!test.good()) {
			MessageBox(NULL,L"L2.exe could not be found, please, fix your files",L"Lineage ][ LastWar Reborn",MB_OK | MB_ICONHAND | MB_DEFBUTTON1);
			return FALSE;
		}
	}

	if(test)
		test.close();

	return TRUE;
}

VOID ClearUnusedDlls()
{
	/*WIN32_FIND_DATAA fd;
	HANDLE fHandle;

	fHandle = FindFirstFileA(dir, &fd);

	if(fHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// Is folder
	if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		inflate_folder(fd.cFileName, out);

		DWORD32 filesize = 0;

		CCHAR dir_route[FILE_NAME_BUFFER_SIZE];
		strcpy(dir_route,dir);
		strcat(dir_route,"\\*");
		
		WIN32_FIND_DATAA fdd;
		HANDLE fHandle2 = FindFirstFileA(dir_route, &fdd);
		if(fHandle2 != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(strcmp(fdd.cFileName, ".") != 0 && strcmp(fdd.cFileName,"..") != 0)
				{
					filesize++;
				}
			}
			while(FindNextFileA(fHandle2, &fdd));
		}

		out << filesize;

		if(filesize > 0)
		{
			CCHAR buf[FILE_NAME_BUFFER_SIZE];
			fHandle2 = FindFirstFileA(dir_route, &fdd);
			if(fHandle2 != INVALID_HANDLE_VALUE)
			{
				do
				{
					if(strcmp(fdd.cFileName, ".") != 0 && strcmp(fdd.cFileName,"..") != 0)
					{
						strcpy(buf,dir);
						strcat(buf,"\\");
						strcat(buf, fdd.cFileName);
						inflate_block(buf, out);
					}
				}
				while(FindNextFileA(fHandle2, &fdd));
			}
		}

	}
	else
	{
		inflate_file(dir, out);
	}*/
}

HANDLE CreatePipe()
{
	return CreateNamedPipeA("vote_plug_in", 
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 
		BUFSIZE, BUFSIZE, 0,NULL);      
}

VOID ClosePipe(HANDLE &pipe)
{
	FlushFileBuffers(pipe); 
	DisconnectNamedPipe(pipe); 
	CloseHandle(pipe);
}

BOOL HandlePacket(INT packet)
{
	TCPPacket tcp;
	if(packet >= 1 && packet <= 6)
	{
		Packet_SendSiteVoted(tcp, packet);
		SendPacket(tcp);
		return TRUE;
	}

	Packet_SendSiteVoted(tcp, -1);
	SendPacket(tcp);

	return FALSE;
}

VOID StartVotePlugIn()
{
	STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	CreateProcessA("vote_plugin.exe",   // No module name (use command line)
        NULL,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi );      
}

DWORD WINAPI ThreadWarpPipeHandler(LPVOID starting_site)
{
	INT starting = (INT)starting_site;
	PipeHandler(starting);
	return 0;
}

// main pipe func
VOID PipeHandler(INT starting_site)
{
	SetPacketHandler((send_packet_func)SendPacket);

	HANDLE pipe = CreatePipe();

	if(pipe == INVALID_HANDLE_VALUE)
	{
		INT last_error = GetLastError();
		FILE *f = fopen("AAA_pipe_debug.txt","a");
		fprintf(f,"CreateNamedPipeA error: %d\n", last_error);
		fclose(f);
		::MessageBoxA(NULL,"Vote plug-in module could not be loaded.\nTry restarting your computer.", "Lineage ][ LastWar", MB_OK | MB_ICONHAND | MB_DEFBUTTON1);
		return;
	}

	StartVotePlugIn();

	if(ConnectNamedPipe(pipe, NULL))
	{
		HANDLE hHeap = GetProcessHeap();
		TCHAR *req = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
		TCHAR *reply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
		INT packet = 0;
		DWORD readed = 0;
		DWORD written = 0;

		if(WriteFile(pipe,&starting_site,4,&written,NULL))
		{
			do
			{		
				if(!ReadFile(pipe,&packet,4,&readed,NULL))
				{
					::MessageBoxA(NULL,"Vote plug-in failed.\nTry restarting your computer.", "Lineage ][ LastWar", MB_OK | MB_ICONHAND | MB_DEFBUTTON1);
					break;
				}
			}
			while(HandlePacket(packet));
		}
		else
		{
			::MessageBoxA(NULL,"Vote plug-in failed.\nTry restarting your computer.", "Lineage ][ LastWar", MB_OK | MB_ICONHAND | MB_DEFBUTTON1);
		}

		if(req != NULL)
			HeapFree(hHeap,0,req);
		if(reply != NULL)
			HeapFree(hHeap,0,reply);
	}
	else
	{
		INT last_error = GetLastError();
		FILE *f = fopen("AAA_pipe_debug.txt","a");
		fprintf(f,"ConnectNamedPipe error: %d\n", last_error);
		fclose(f);
		::MessageBoxA(NULL,"Vote plug-in module could not be loaded.\nTry restarting your computer.", "Lineage ][ LastWar", MB_OK | MB_ICONHAND | MB_DEFBUTTON1);
	}

	ClosePipe(pipe);

	SetPacketHandler((send_packet_func)ASyncSendPacket);
}