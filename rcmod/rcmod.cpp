//Bootstrapper for modifying resources
//NOTE: See common.cpp and common.h for explanation of how the binary data
//      is stored and handled.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "../common/common.h"

void SetNewQuackwordList(HANDLE hUpdate, unsigned long oldNumQuackwords);

int main(long argc, char* argv[])
{
	Sleep(50); //We want to make sure the other program ended

	HANDLE hUpdate = BeginUpdateResource(argv[2], FALSE);
	//Check which function should be called
	if (strcmp(argv[1], "IntractWebsiteFiles") == 0)
	{
		IntractWebsiteFiles(hUpdate, true);
		//Update the number of guessed quackwords
		unsigned long numQWGuesses = atoi(argv[3]);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_GUESSES), NULL, &numQWGuesses, 4);
	}

	if (strcmp(argv[1], "SetNewQuackwordList") == 0)
	{
		SetNewQuackwordList(hUpdate, atoi(argv[4]));
		unsigned long numQWGuesses = 0;
		//Reset the number of guessed quackwords
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_GUESSES), NULL, &numQWGuesses, 4);
	}

	if (strcmp(argv[1], "UpdateQWGuesses") == 0) //This will be called if security is violated
	{
		//Update the number of guessed quackwords
		unsigned long numQWGuesses = atoi(argv[3]);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_GUESSES), NULL, &numQWGuesses, 4);
	}

	if (strcmp(argv[1], "UpdateGuestFiles") == 0)
	{
		IntractWebsiteFiles(hUpdate, true, true);
		//Update the number of guessed quackwords
		unsigned long numQWGuesses = atoi(argv[3]);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_GUESSES), NULL, &numQWGuesses, 4);
	}

	EndUpdateResource(hUpdate, FALSE);
	std::ostringstream args;
	args << "Delete" << " \"" << argv[0] << "\"";
	ShellExecute(NULL, "open", argv[2], args.str().c_str(), NULL, SW_SHOWDEFAULT);
	return 0;
}

void SetNewQuackwordList(HANDLE hUpdate, unsigned long oldNumQuackwords)
{
	using std::vector;
	using std::string;
	using std::getline;
	using std::cin;
	using std::cout;

	vector<string> quackwords;
	unsigned long numQuackwords;
	string input;
	char* phText; //Temporary storage for encoding phText

	//First we want to delete the old quackwords (not sure if this works)
	for (unsigned long i = 0; i < oldNumQuackwords; i++)
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_LIST_START+i), NULL, NULL, 0);

	//Now we get the quackwords from the user
	cout << "Enter the new quackword-changing quackword: ";
	getline(cin, input);
	quackwords.push_back(input);
	cout << "Enter the number of new quackwords you want: ";
	cin >> numQuackwords;
	cin.ignore(); //Skip the newline character left in the buffer

	for (unsigned long i = 0; i < numQuackwords; i++)
	{
		cout << "Quackword #" << i+1 << ": ";
		getline(cin, input);
		quackwords.push_back(input);
	}

	//Add the new resources into our executable
	phText = new char[quackwords[0].size()+1];
	strcpy(phText, quackwords[0].c_str());
	EncodeText(phText);
	UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_CHANGE), NULL, phText, strlen(phText)+1);
	delete []phText;
	UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(NUM_QUACKWORDS), NULL, &numQuackwords, sizeof(numQuackwords));
	for(unsigned long i = 1; i <= numQuackwords; i++)
	{
		phText = new char[quackwords[i].size()+1];
		strcpy(phText, quackwords[i].c_str());
		EncodeText(phText);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(QUACKWORD_LIST_START+i-1), NULL, phText, strlen(phText)+1);
		delete []phText;
	}
}