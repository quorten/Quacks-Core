//Functions common to all programs

#include "common.h"

using std::vector;
using std::string;

//Function implimentions
void EncodeText(char* pData)
{
	for (unsigned long i = 0; pData[i] != 0; i++)
		pData[i] += 128;
}

void DecodeText(char* pData)
{
	for (unsigned long i = 0; pData[i] != 0; i++)
		pData[i] -= 128;
}

/*Note for out binary handling functions:
  The binary data handling functions are 
  meant to handle unknown "chunks" of data.
  The data is stored as follows:
  1. A pointer points to an unsigned 32-bit integer.
  2. That integer indicates the size of the preceding
	 unknown data.*/

void EncodeBinary(void* pData)
{
	char* pData2 = (char*)pData; //All the casting to char* was moved to here
	unsigned long* pDataSize = SizeofBinData(pData2);
	pData2 += 4;
	for (unsigned long i = 0; i < *pDataSize; i++)
		pData2[i] += 128;
	pData2 -= 4;
}

void DecodeBinary(void* pData)
{
	char* pData2 = (char*)pData; //All the casting to char* was moved to here
	unsigned long* pDataSize = SizeofBinData(pData2);
	pData2 += 4;
	for (unsigned long i = 0; i < *pDataSize; i++)
		pData2[i] -= 128;
	pData2 -= 4;
}
inline unsigned long* SizeofBinData(void* pData)
{ return reinterpret_cast<unsigned long*>(pData); }

PHEAP_DATA ReadBinaryFile(const char* pFilename)
{
	//Binary file handling is done with windows API's (but it shouldn't be)
	unsigned long bytesRead;
	HANDLE hFile = CreateFile(pFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);

	unsigned long dataSize = GetFileSize(hFile, NULL);
	char* phInData = new char[dataSize];
	ReadFile(hFile, phInData, dataSize, &bytesRead, NULL);
	CloseHandle(hFile);

	unsigned char* phOutData = new unsigned char[4+dataSize];
	memcpy(phOutData, &dataSize, 4);
	memcpy(phOutData + 4, phInData, dataSize);
	delete []phInData;

	return phOutData;
}

bool WriteBinaryFile(const char* pFilename, void* pData)
{
	//Binary file handling is done with windows API's (but it shouldn't be)
	unsigned long bytesWritten;
	HANDLE hFile = CreateFile(pFilename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
		return false;
	WriteFile(hFile, (char*)pData + 4, *SizeofBinData(pData), &bytesWritten, NULL);
	CloseHandle(hFile);

	return true;
}

void IntractWebsiteFiles(HANDLE hUpdate, bool bDeleteExternal, bool bUseGuestFiles)
{
	vector<string> dirContents;
	unsigned short numStorageToUse;
	unsigned short storageAreaToUse;
	char* phText; //Temporary storage for encoding phText

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

	phText = new char[1000];
	GetCurrentDirectory(1000, phText);
	ListDirectoryContents(phText, dirContents);
	delete []phText;
	unsigned long numWebFiles = dirContents.size();
	UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(numStorageToUse), NULL, &numWebFiles, 4);

	for (unsigned long i = 0; i < dirContents.size(); i++)
	{
		//Do the data
		PHEAP_DATA phData = ReadBinaryFile(dirContents[i].c_str());
		EncodeBinary(phData);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(storageAreaToUse+i*2), NULL, phData, 4 + *SizeofBinData(phData));
		delete []phData;

		//Do the name
		phText = new char[dirContents[i].size()+1];
		strcpy(phText, dirContents[i].c_str());
		EncodeText(phText);
		UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(storageAreaToUse+i*2+1), NULL, phText, dirContents[i].size()+1);
		delete []phText;

		if (bDeleteExternal)
			remove(dirContents[i].c_str());
	}
}

bool ListDirectoryContents(char* targetDir, vector<string>& fileList)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   char DirSpec[MAX_PATH + 1];  // directory specification
   DWORD dwError;

   //printf ("Target directory is %s.\n", targetDir);
   if (strlen(targetDir) + 3 < MAX_PATH)
   {
	strncpy (DirSpec, targetDir, strlen(targetDir)+1);
	strncat (DirSpec, "\\*", 3);
   }
   else
	   return false;

   hFind = FindFirstFile(DirSpec, &FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) 
   {
      //printf ("Invalid file handle. Error is %u\n", GetLastError());
      return false;
   } 
   else 
   {
	   string filename = FindFileData.cFileName;
	   if (filename.find(".") != 0 && filename.find(".exe") == string::npos) //We don't want ".", "..", or logon.exe
		fileList.push_back(FindFileData.cFileName);
      while (FindNextFile(hFind, &FindFileData) != 0) 
      {
		filename = FindFileData.cFileName;
		if (filename.find(".") != 0 && filename.find(".exe") == string::npos) //We don't want ".", "..", or logon.exe
		fileList.push_back(FindFileData.cFileName);
      }
    
      dwError = GetLastError();
      FindClose(hFind);
      if (dwError != ERROR_NO_MORE_FILES) 
      {
         //printf ("FindNextFile error. Error is %u\n", dwError);
         return false;
      }
   }
   return true;
}