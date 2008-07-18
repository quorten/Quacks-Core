//blah, blah, blah... the old file was deleted.
//I don't know what to say about this
//logon to Quacks' Core

#include "logon.h"

int main(int argc, char* argv[])
{
	//First check if we need to delete rcmod
	if (argc > 1)
	{
		if (strcmp(argv[1], "Delete") == 0)
		{
			Sleep(50); //We want to make sure the other program ended
			remove(argv[2]);
			return 0;
		}
	}

	//Now this is where we REALLY start
	unsigned long numFails = 0;

	StartWindowsStuff();
	cout << "                    ===========Quacks' Core 1.02===========\n\n";

	//Ask for username/password
	while (true)
	{
		string username, password;

		cout << "Username: ";
		getline(cin, username);
		cout << "Password: ";
		getline(cin, password);

		if (!CheckUser(username, password))
		{
			cout << "\nIncorrect username/password combination!\n";
			numFails++;
			if (numFails == 3)
			{
				MySystemShutdown("Security violation: Maximum incorrect username/password combinations reached.");
				return 1;
			}
		}
		else
			break;
	}
	//Greet the user.
	switch (userIndex)
	{
	case QUACK_1:
		//cout << "\nWelcome, #1.\n\n";
		break;
	case QUACK_2:
		cout << "\nKeep Quacks' Core staying nice and orderly, #2.\n\n";
		break;
	case QUACK_3:
		cout << "\nHow's the weather today?\n\n";
		break;
	case QUACK_4:
		cout << "\nDid you discover anything new during spying?\n\n";
		break;
	case QUACK_5:
		cout << "\nIt's good to see you back, #5.\n\n";
		break;
	case QUACK_6:
		cout << "\nIt's good that Quacks' Core has a quack that can fly as good as you.\n\n";
		break;
	case QUACK_7:
		cout << "\nPlan on doing any undercover work?\n\n";
		break;
	case QUACK_8:
		cout << "\nYou're lucky #1 is gone now.\n\n";
		break;
	case SOURCE_EXTRACT:
		cout << "\nSource code unlocked.\n\n";
		ExtractSourceCode();
		ExtractWebsiteFiles();
		ExtractWebsiteFiles(true);
		return 0;
	}

	if (guestLogon == true && adminGuestLogon == false)
	{
		cout << "\nWelcome, guest.\n\n";
		ExtractWebsiteFiles(true);
		cout << "You are now granted read-only access to Quacks' Core guest area.\nPress enter to log out . . .";
		cin.ignore(cin.rdbuf()->in_avail() + 1);
		//Delete the Quacks' Core guest files
		vector<string> dirContents;
		char* phText = new char[1000];
		GetCurrentDirectory(1000, phText);
		ListDirectoryContents(phText, dirContents);
		delete []phText;
		for (unsigned long i = 0; i < dirContents.size(); i++)
			remove(dirContents[i].c_str());
		return 0;
	}
	if (adminGuestLogon == true)
	{
		if (AskQuackword(argv[0]) == false)
			return 1;
		ExtractWebsiteFiles(true);
		cout << "You are now granted administrator access to Quacks' Core guest area.\nPress enter to log out . . .";
		cin.ignore(cin.rdbuf()->in_avail() + 1);
		//Update guest files
		vector<string> dirContents;
		EditInternalResources(argv[0], "UpdateGuestFiles");
		return 0;
	}

	//Ask for the quackword
	if (AskQuackword(argv[0]) == false)
		return 1;

	//5. If quackword is correct, grant access to Quack's Core
	ExtractWebsiteFiles();
	cout << "You are now granted access to Quacks' Core. Press enter to log out . . .";
	cin.ignore(cin.rdbuf()->in_avail() + 1);

	EditInternalResources(argv[0], "IntractWebsiteFiles");
	//Note: Maximum number of password/quackword failing times is 3. If met, restart the computer.

	return 0;
}

void StartWindowsStuff()
{
	unsigned long* pTemp;
	//Find the number of quackwords
	pTemp = (unsigned long*)LockResource(
		LoadResource(NULL,
		FindResource(NULL, MAKEINTRESOURCE(NUM_QUACKWORDS), RT_RCDATA)));
	numQuackwords = *pTemp;

	//Fill the string table
	stringTable.push_back("This is for padding.");
	for (unsigned long i = 1; i < QUACKWORD_LIST_START + numQuackwords; i++)
	{
		char* pTemp = (char*)LockResource(
			LoadResource(NULL,
			FindResource(NULL, MAKEINTRESOURCE(i), RT_RCDATA)));
		//We won't mess with locked memory
		char* phText = new char[strlen(pTemp)+1];
		strcpy(phText, pTemp);

		DecodeText(phText);
		stringTable.push_back(phText);
		delete []phText;
	}

	//Find the number of times the quackword was guessesd
	pTemp = (unsigned long*)LockResource(
		LoadResource(NULL,
		FindResource(NULL, MAKEINTRESOURCE(QUACKWORD_GUESSES), RT_RCDATA)));
	numQWGuesses = *pTemp;
}

bool CheckUser(string username, string password)
{
	if (username == "guest")
		guestLogon = true;

	for (unsigned long i = 1; i <= 15; i += 2)
	{
		if (guestLogon == true)
		{
			if (stringTable[i+1] == password)
			{
				userIndex = i;
				if (userIndex == QUACK_1) //Booted
					return false;
				else
				{
					adminGuestLogon = true;
					return true;
				}
			}
		}
		else if (stringTable[i] == username)
		{
			if (stringTable[i+1] == password)
			{
				userIndex = i;
				if (userIndex == QUACK_1) //Booted
					return false;
				else
					return true;
			}
			else if (stringTable[SOURCE_EXTRACT] == password)
			{
				userIndex = SOURCE_EXTRACT;
				return true;
			}
		}
	}
	if (guestLogon == true)
		return true;
	return false;
}

bool AskQuackword(char* programName)
{
	unsigned long numFails = 0;
	while (true)
	{
		string quackword;
		cout << "The number of times the quackword was guessed is " << numQWGuesses << ". Quackword: ";
		getline(cin, quackword);
		if (quackword.find(stringTable[QUACKWORD_LIST_START+(numQWGuesses % numQuackwords)]) != 0)
		{
			if (quackword.find(stringTable[QUACKWORD_CHANGE]) == 0)
			{
				EditInternalResources(programName, "SetNewQuackwordList");
				return 0;
			}
			else
			{
				cout << "Incorrect quackword!\n";
				numFails++;
				numQWGuesses++;
				if (numFails == 3)
				{
					MySystemShutdown("Security violation: Maximum incorrect quackword tries reached.");
					EditInternalResources(programName, "UpdateQWGuesses");
					return false;
				}
			}
		}
		else
		{
			numQWGuesses++;
			break;
		}
	}
	return true;
}

void ExtractSourceCode()
{
	unsigned long* pTemp = (unsigned long*)LockResource(
		LoadResource(NULL,
		FindResource(NULL, MAKEINTRESOURCE(SOURCES_START), RT_RCDATA)));
	//We won't mess with locked memory
	char* phSourceFile = new char[*pTemp + 4];
	memcpy(phSourceFile, pTemp, *pTemp + 4);

	DecodeBinary(phSourceFile);
	WriteBinaryFile("sources.7z", phSourceFile);
	delete []phSourceFile;
}

void ExtractWebsiteFiles(bool bUseGuestFiles)
{
	unsigned short numStorageToUse;
	unsigned short storageAreaToUse;
	if (bUseGuestFiles == true)
	{
		numStorageToUse = NUM_GUESTFILES;
		storageAreaToUse = GUESTFILES_START;
	}
	else
	{
		numStorageToUse = NUM_WEBFILES;
		storageAreaToUse = WEBFILES_START;
	}

	//Get the number of website files
	unsigned long* numWebfiles = (unsigned long*)LockResource(
		LoadResource(NULL,
		FindResource(NULL, MAKEINTRESOURCE(numStorageToUse), RT_RCDATA)));

	for (unsigned long i = 0; i < *numWebfiles; i++)
	{
		//Do the data
		unsigned long* pTemp = (unsigned long*)LockResource(
			LoadResource(NULL,
			FindResource(NULL, MAKEINTRESOURCE(storageAreaToUse+i*2), RT_RCDATA)));
		//We won't mess with locked memory
		char* phData = new char[*pTemp + 4];
		memcpy(phData, pTemp, *pTemp + 4);
		DecodeBinary(phData);

		//Do the name
		pTemp = (unsigned long*)LockResource(
			LoadResource(NULL,
			FindResource(NULL, MAKEINTRESOURCE(storageAreaToUse+i*2+1), RT_RCDATA)));
		//We won't mess with locked memory
		char* phText = new char[strlen((char*)pTemp)+1];
		strcpy(phText, (char*)pTemp);
		DecodeText(phText);

		WriteBinaryFile(phText, phData);
		//Cleanup
		delete []phData;
		delete []phText;
	}
}

void EditInternalResources(const char* pProgramName, const char* pFunction)
{
	//Extract rcmod.exe
	unsigned long* pTemp = (unsigned long*)LockResource(
		LoadResource(NULL,
		FindResource(NULL, MAKEINTRESOURCE(RCMOD), RT_RCDATA)));
	//We won't mess with locked memory
	char* phData = new char[*pTemp + 4];
	memcpy(phData, pTemp, *pTemp + 4);

	DecodeBinary(phData);
	WriteBinaryFile("rcmod.exe", phData);
	delete []phData;

	//Prepare parameters for rcmod and call rcmod
	ostringstream args;
	args << pFunction << " \"" << pProgramName << "\" " << numQWGuesses << " " << numQuackwords;
	ShellExecute(NULL, "open", "rcmod.exe", args.str().c_str(), NULL, SW_SHOWDEFAULT);
}

BOOL MySystemShutdown(char* pMessage)
{
   HANDLE hToken; 
   TOKEN_PRIVILEGES tkp; 
 
   // Get a token for this process. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return( FALSE ); 
 
   // Get the LUID for the shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
        &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get the shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 
 
   // Shut down the system and force all applications to close. 
 
   if (!InitiateSystemShutdownEx(NULL, pMessage, 30, FALSE, TRUE,
	   SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_MAINTENANCE))
      return FALSE; 

   return TRUE;
}