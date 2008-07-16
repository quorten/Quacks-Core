//Does encodment of resources for logon.exe, then adds those into logon.exe
//NOTE: See common.cpp and common.h for explanation of how the binary data
//      is stored and handled.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

using std::ifstream;
using std::string;

#include "../common/common.h"

int main(long argc, char* argv[])
{
	HANDLE hUpdate;
	hUpdate = BeginUpdateResource(argv[1], FALSE);

	//Get the encoded text into there
	for (unsigned long i = 1; i <= 48; i++)
	{
		char text[100];
		LoadString(NULL, i, text, 100);
		EncodeText(text);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(i), NULL, text, strlen(text)+1);
	}

	//Get number of guessed quackwords into there
	{
		unsigned long numGuessedQuackwords = 7;
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_GUESSES), NULL, &numGuessedQuackwords, sizeof(numGuessedQuackwords));
	}

	//Get number of quackwords into there
	{
		unsigned long numQuackwords = 1;
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(NUM_QUACKWORDS), NULL, &numQuackwords, sizeof(numQuackwords));
	}

	//Get the source code into there
#if SOURCE_ARCHIVE_READY == 1
	{
		PHEAP_DATA phData = ReadBinaryFile("..\\sources.7z");
		EncodeBinary(phData);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(SOURCES_START), NULL, phData, 4 + *SizeofBinData(phData));
		//Cleanup
		delete []phData;
	}
#endif

	//Get the resource modifier program into there
	{
		PHEAP_DATA phData = ReadBinaryFile("rcmod.exe");
		EncodeBinary(phData);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(RCMOD), NULL, phData, 4 + *SizeofBinData(phData));
		//Cleanup
		delete []phData;
	}

	//Get webfiles into there
	{
		char oldDirectory[1000];
		GetCurrentDirectory(1000, oldDirectory);
		SetCurrentDirectory("..\\Quack");
		IntractWebsiteFiles(hUpdate);
		SetCurrentDirectory(oldDirectory);
	}

	//Get the guest files into there
	{
		char oldDirectory[1000];
		GetCurrentDirectory(1000, oldDirectory);
		SetCurrentDirectory("..\\Guest");
		IntractWebsiteFiles(hUpdate, false, true);
		SetCurrentDirectory(oldDirectory);
	}
	EndUpdateResource(hUpdate, FALSE);

	return 0;
}