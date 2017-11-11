// ======================================================================
//
// WinMain.cpp
//
// ======================================================================

#include "FirstSwgClient.h"

#include "ClientMain.h"

#include "LocalizedString.h"
#include "StringId.h"

#include "clientGame/Game.h"
#include "../../../../../../engine/shared/library/sharedFoundation/include/public/sharedFoundation/Production.h"
#include "../../../../../../engine/shared/library/sharedGame/include/public/sharedGame/PlatformFeatureBits.h"

#include <shellapi.h>
#include <ios>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>


extern void externalCommandHandler(const char*);

// ======================================================================

static bool SetUserSelectedMemoryManagerTarget()
{
	char buffer[32];
	DWORD result = GetEnvironmentVariable("SWGCLIENT_MEMORY_SIZE_MB", buffer, sizeof(buffer));

	// make sure the environment variable was set
	if (result <= 0 || result >= sizeof(buffer))
		return false;

	// inline atoi() because the crt hasn't been initialized yet
	int megabytes = 0;
	for (char const * b = buffer; *b; ++b)
	{
		// handle bad characters in the environment variable by ignoring the whole thing
		if (*b < '0' || *b > '9')
			return false;

		megabytes = (megabytes * 10) + (*b - '0');
	}

	MemoryManager::setLimit(megabytes, false, false);
	return true;
}

// ----------------------------------------------------------------------

static void SetDefaultMemoryManagerTargetSize()
{
	// we use 75% of the available ram, up to 1536mb in the case of 2gb (32 bit without PAE limit)
	MEMORYSTATUSEX memoryStatus = { sizeof memoryStatus };
	GlobalMemoryStatusEx(&memoryStatus);
	int ramMB = (memoryStatus.ullTotalPhys / 1048576);

	// without PAE enabled 2048 is the max we can do, but SWG crashes if we give it all the RAM sometimes
	if (ramMB >= 2048)
	{
		ramMB = 1536;
	}
	else 
	{
		ramMB = (ramMB * .75);
	}

	MemoryManager::setLimit(ramMB, false, false);
}

void externalCommandHandler(const char* command)
{
	const StringId trialNagId("client", "npe_nag_url_trial");
	const StringId rentalNagId("client", "npe_nag_url_rental");

	Unicode::String url;

	if ((Game::getSubscriptionFeatureBits() & ClientSubscriptionFeature::NPENagForTrial) != 0)
	{
		url = trialNagId.localize();
	}

	if (!url.empty())
	{
		Unicode::NarrowString url8 = Unicode::wideToNarrow( url );

		HINSTANCE result = ShellExecute(NULL, "open", url8.c_str(), NULL, NULL, SW_SHOWNORMAL);

		if (reinterpret_cast<int>(result) < 32) //Pulled straight from MSDN -ARH
		{
			WARNING(true, ("could not launch external application (%d)", reinterpret_cast<int>(result)));
		}
		else
		{
			Game::quit();
		}
	}
}

// ======================================================================
// Entry point for the application
//
// Return Value:
//
//   Result code to return to the operating system
//
// Remarks:
//
//   This routine should set up the engine, invoke the main game loop,
//   and then tear down the engine.

int WINAPI WinMain(
	HINSTANCE hInstance,      // handle to current instance
	HINSTANCE hPrevInstance,  // handle to previous instance
	LPSTR     lpCmdLine,      // pointer to command line
	int       nCmdShow        // show state of window
	)
{
	if (!SetUserSelectedMemoryManagerTarget())
		SetDefaultMemoryManagerTargetSize();

	// Create a console
	AllocConsole();

	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE *fp;
	const unsigned int MAX_CONSOLE_LINES = 500;
	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();

	return ClientMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

// ======================================================================
