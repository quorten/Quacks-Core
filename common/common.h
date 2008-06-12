//Functions common to all programs
#ifndef COMMON_H
#define COMMON_H

//Includes for common.cpp
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef  WIN32_LEAN_AND_MEAN //For saftey
#endif

#include <string>
#include <vector>

//Resource definitions
#define QUACK_1					1	//Fling Fling
#define QUACK_2					3	//Dafy Duck
#define QUACK_3					5	//Rain Quacky
#define QUACK_4					7	//Bathtoy Quacky
#define QUACK_5					9	//Washcloth Quacky
#define QUACK_6					11	//Jake
#define QUACK_7					13	//Rubber Ducky
#define QUACK_8					15	//Keychain Quacky
#define SOURCE_EXTRACT			17	//special password
#define QUACKWORD_CHANGE		18	//used to change the current quackword
#define QUACKWORD_LIST_START	19	//ID for first quackword
#define QUACKWORD_GUESSES		100 //holds number of guessed quackwords
#define NUM_QUACKWORDS			101 //holds number of different quackwords stored
#define SOURCES_START			102 //ID that holds the source code archive
#define SOURCE_ARCHIVE_READY	0	//Whether the source code archive is ready or not
#define RCMOD					103 //Resource modifying program for self-modifications
#define NUM_WEBFILES			104 //holds number of webfiles
#define WEBFILES_START			105 //Beginning ID where website files are
#define NUM_GUESTFILES			200 //holds number of guest files
#define GUESTFILES_START		201 //Beginning ID whwere guest files are

//Type defintions
typedef void* PHEAP_DATA;

//Prototypes
void		EncodeText(char* pData);
void		DecodeText(char* pData);
void		EncodeBinary(void* pData);
void		DecodeBinary(void* pData);
inline unsigned long* SizeofBinData(void* pData);
PHEAP_DATA	ReadBinaryFile(const char* pFilename);
bool		WriteBinaryFile(const char* pFilename, void* pData);
void		IntractWebsiteFiles(HANDLE hUpdate, bool bDeleteExternal = false, bool bUseGuestFiles = false);
bool ListDirectoryContents(char* targetDir, std::vector<std::string>& fileList);

#endif