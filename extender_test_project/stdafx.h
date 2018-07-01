#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN      
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "Advapi32.lib")
#pragma comment (lib, "User32.lib")

#include <string>

#include <windows.h>
#include <WinSock2.h>

#include <stdlib.h>
#include <queue>
#include <mbstring.h>
#include <Psapi.h>
#include <direct.h>
#include <strsafe.h>
#include <fstream>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <memory>

#include "NetworkHandler.h"
#include "OpcodeHandlers.h"
#include "SiteParsers.h"

#include "Packets.h"
#include "tools.h"
#include "hook.h"
#include "GameCrypt.h"
#include "CQueue.h"
