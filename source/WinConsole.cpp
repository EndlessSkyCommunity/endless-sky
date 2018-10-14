/* WinConsole.cpp
Copyright (c) 2017 by Frederick Goy IV

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifdef _WIN32



#include "WinConsole.h"

#include "Files.h"

#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <string>

using namespace std;

namespace {
	FILE *consoleLog = nullptr;
	
	string FormatError(const DWORD errorCode)
	{
		LPSTR msgBuffer = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			nullptr, 
			errorCode, 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			reinterpret_cast<LPSTR>(&msgBuffer), 
			0, 
			nullptr);
		
		string message;
		if(!msgBuffer || !*msgBuffer)
			message = "Failed to format message.";
		else
		{
			message = msgBuffer;
			
			// Get rid of CR or we end up with CRCRLF
			message.erase(remove(message.begin(), message.end(), '\r'), message.end());
		}
		
		LocalFree(msgBuffer);
		msgBuffer = nullptr;
		
		return message;
	}
	void WriteConsoleLog(const string &message)
	{
		if(!consoleLog)
			consoleLog = Files::Open("consoleLog.txt", true);
		
		Files::Write(consoleLog, "Failed to initialize console: " + message);
		fflush(consoleLog);
	}
}

void WinConsole::Init()
{
	try {
		// Find out what to redirect
		bool redirStdout = _fileno(stdout) == -2;
		bool redirStderr = _fileno(stderr) == -2;
		
		// Everything is being redirected at the command line
		if(!redirStdout && !redirStderr)
			return;
			
		// Attach the parent console, and handle the case where we launched with
		// arguments, but without a console (like a shortcut with target args)
		if(!AttachConsole(ATTACH_PARENT_PROCESS))
		{
			if(AllocConsole())
			{
				redirStdout = false;
				redirStderr = false;
			}
			else
				throw runtime_error(FormatError(GetLastError()));
		}
		
		// Get a console handle
		const HANDLE conoutHandle = CreateFile(
			"CONOUT$", 
			GENERIC_READ | 
			GENERIC_WRITE, 
			FILE_SHARE_READ | 
			FILE_SHARE_WRITE, 
			0, 
			OPEN_EXISTING, 
			0, 
			nullptr);
		if(conoutHandle != INVALID_HANDLE_VALUE)
		{
			// Set console's max lines for large output (--ships, --weapons)
			CONSOLE_SCREEN_BUFFER_INFO conBufferInfo;
			GetConsoleScreenBufferInfo(conoutHandle, &conBufferInfo);
			
			// Make sure the user doesn't already have a larger screenbuffer size
			if(conBufferInfo.dwSize.Y < 750)
			{
				conBufferInfo.dwSize.Y = 750;
				SetConsoleScreenBufferSize(conoutHandle, conBufferInfo.dwSize);
			}
			
			CloseHandle(conoutHandle);
		}
		else
			throw runtime_error(FormatError(GetLastError()));
		
		// Redirect
		if(redirStdout)
		{
			freopen("CONOUT$", "w", stdout);
			setvbuf(stdout, nullptr, _IOFBF, 4096);
		}
		
		if(redirStderr)
		{
			freopen("CONOUT$", "w", stderr);
			setvbuf(stderr, nullptr, _IOLBF, 1024);
		}
	}
	catch(const runtime_error &error)
	{
		WriteConsoleLog(error.what());
	}
}



#endif
