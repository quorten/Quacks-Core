//blah, blah, blah... the old file was deleted.
//I don't know what to say about this
//logon to Quacks' Core
#ifndef LOGON_H
#define LOGON_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#include "../common/common.h"

void StartWindowsStuff();
bool CheckUser(string username, string password);
bool AskQuackword(char* programName);
void ExtractSourceCode();
void ExtractWebsiteFiles(bool bUseGuestFiles = false);
void EditInternalResources(const char* pProgramName, const char* pFunction);
BOOL MySystemShutdown(char* pMessage);

vector<string> stringTable;
unsigned long userIndex;
unsigned long numQuackwords;
unsigned long numQWGuesses;
bool guestLogon = false;
bool adminGuestLogon = false;

#endif