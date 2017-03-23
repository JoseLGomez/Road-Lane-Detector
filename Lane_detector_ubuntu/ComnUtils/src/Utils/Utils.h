/* ********************************* MODULE **********************************/
/** @defgroup	Utils Utils
 *
 *	@ingroup	LowLayer
 * 
 *	@brief		This is the module Utils
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/

/* ********************************* MODULE **********************************/
/** @defgroup	ComnUtils Common Utils
 *
 *	@ingroup	Utils
 * 			
 *	@brief		This is the module ComnUtils
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/


/* ********************************* FILE ************************************/
/** @file		Utils.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of function utilities
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _UTILS_
#define _UTILS_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
//#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <conio.h>
#else
#include <iostream>
#include <sstream>
#endif
#include <algorithm>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <time.h>
#include <chrono>
#include <thread>
#include "IniFileIO.h"
#include "UniqueIdentifier.h"
#include "MyChrono.h"
using std::vector;
using std::string;
#ifdef __unix__
    #include <unistd.h>
    using std::max;
    using std::min;
    using std::cout;
    using std::endl;
#endif

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
namespace cv
{
	class Mat;
}
template<typename T> class CImageG;
typedef CImageG<float> CImageFloat;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/
// Error types
#define VSys_Ok						"0: Everithing is ok"
#define VSys_Error					"-1: Unknown /unspecified error"
#define VSys_Assert					"-2: Assertion failed"
#define VSys_NotImplemented			"-3: The requested function/feature is not implemented"
#define VSys_ParseError				"-4: Invalid syntax/structure of the parsed file"
#define VSys_OutOfRange				"-5: Some of parameters are out of range"
#define VSys_XMLError				"-6: XML problem"
#define VSys_FileNotFound			"-7: File not found"
#define VSys_UnkINIParam			"-8: Unknown INI parameter"
#define VSys_IncINIParam			"-9: Inconsistetent INI parameter"
#define VSys_UnkFormat				"-10: Unknown Format"
#define VSys_WritingErr				"-11: Problems while writing to disck"
#define VSys_ReadingErr				"-12: Problems while reading from disck"
#define Vsys_NotAllowedOperation	"-13: This operation can not be performed"
#define Vsys_ZeroDivision			"-14: Division by zero"
#define Vsys_Sockets				"-15: Socket communication error"

// Log files
#define ERROR_LOG_FILE			"./ErrorLog.dat"
#define LOG_FILE				"./Log.dat"

#define NOMINMAX

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
enum EOpenModeType {eomt_unknown = -1, eomt_append, eomt_overwrite, eomt_read};



// Format
string format		( const char* format, ... );

// Prints
void Print			(const char* format, ...);
void Println		(const char* format, ...);
void Fprintf		(FILE* stream, char * format, ...);
FILE* Fopen			(string fileName, string mode);

// Errors
void _Error			(const char* code, const string& msg, const string& func, const string& file, int line);
void _ErrorQuit		(const char* code, const string& msg, const string& func, const string& file, int line);

#define VSysError(code, msg) _Error(code, msg, __FUNCTION__, __FILE__, __LINE__)
#define VSysError_(code, args) _Error(code, format args, __FUNCTION__, __FILE__, __LINE__)
#define ErrorQuit(code, msg) _ErrorQuit(code, msg, __FUNCTION__, __FILE__, __LINE__)
#define ErrorQuit_(code, args) _ErrorQuit(code, format args, __FUNCTION__, __FILE__, __LINE__)

// Asserts
void Assert(const bool condition);

// Retrieve paths and extensions
void RetrievePath			(const string& filePath, string& path, string& file);
void RetrieveExtension		(const string& filename, string& rawFilename, string& extension);
void RetrieveAll			(const string& filePath, string& path, string& fullFileName, string& rawFilename, string& extension);
void StandarizeDirectory	(string& path);
void WindowsDirectory		(string& path);

// Exist
bool FileExists				(const string& file);
bool DirectoryExists		(const string& directory_in);
bool Exists					(const string& directory);

// MS-DOS commands: Rename, Move, Delete, Mkdir, PWD
void RenameFile				(const string& fileIn, const string& fileOut);
void MoveFile				(const string& fileIn, const string& fileOut);
void CopyFile				(const string& fileIn, const string& fileOut);
void DeleteFile				(const string& fileIn);
bool CreateDirectory		(const string& pathIn);
string GetWorkingDirectory	();

// String processing
string RTrim				(const string& input, const int number);
string LTrim				(const string& input, const int number);
void Tokenize				(const string& str, vector<string>& tokens, const string& delimiters);
string ComposeFileName		(const string& path, const string& fileName, const int index=-1, const string& extension="dat");
template<typename T> string Num2Str(T num)
{
	std::stringstream ss;
	ss<<num;
	return ss.str();
}

// Merge files
void MergeFiles(string input01, string input02, string output);
void MergeNFiles(vector<string>& vString, string output);

// Read fileLists
void ReadFilesList(const string& listFileName, vector<string>& vFiles, const bool bkeepPath=true, const bool bkeepExtensions=true, const string& extensionToFind="*");
void ReadDirectory(const string& directory, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions, const vector<string>& vExtensionToFind);
void ReadDirectory(const string& directory, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions, const string& extensionToFind="*");
void ReadFilesListFromTXT(const string& listFileName, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions);




///////////////////////////////////////////////////////////////////////////////
// Prints
///////////////////////////////////////////////////////////////////////////////
inline string format( const char* format, ... )
{
    char buf[1 << 16];
    va_list args;
    va_start( args, format );
    vsprintf( buf, format, args );
    return string(buf);
}

inline void Print(const char* format, ...)
{
	va_list arg;					// formatted text
	va_start (arg, format);
	vprintf(format, arg);
	va_end (arg);
    fflush(stdout);
}

inline void Println(const char* format, ...)
{
	va_list arg;					// formatted text
	va_start (arg, format);
	vprintf(format, arg);
	va_end (arg);
	printf("\n");
}

inline void Fprintf (FILE* stream, const char * format, ...)
{
  va_list args;
  va_start (args, format);
  if (vfprintf (stream, format, args)<0) ErrorQuit(VSys_WritingErr, "");
  va_end (args);
}

inline FILE* Fopen(string fileName, string mode)
{
	FILE* fp = fopen(fileName.c_str(), mode.c_str());
	if (fp==NULL) ErrorQuit(VSys_FileNotFound, fileName);
	return fp;
}



///////////////////////////////////////////////////////////////////////////////
// Asserts
///////////////////////////////////////////////////////////////////////////////
inline void Assert(const bool condition)
{
	if (!condition)
	{
		Println("AssertFailed: Abnormal program termination..");
		system("pause");
		exit(0);
	}
}



///////////////////////////////////////////////////////////////////////////////
// Wait and pause
///////////////////////////////////////////////////////////////////////////////
inline void Wait(int miliSeconds)
{

    std::this_thread::sleep_for(std::chrono::milliseconds(miliSeconds));
    //Sleep((DWORD)miliSeconds);
	//clock_t endwait;
	//endwait = clock () + (long)(miliSeconds/1000.0f*CLOCKS_PER_SEC);
	//while (clock() < endwait) {}
}

inline void Pause()
{
	Print("Press a key to continue.\n");
    getchar();
}


template<class T> void PrintMatrix(const vector<vector<T> >& m, const char* name=NULL)
{
    if (name) std::cout << "[" << name << "] = " << std::endl;
	for(int r=0; r<m.size(); r++)
	{
        for(int c=0; c<m[r].size(); c++) std::cout << "\t" << m[r][c];
        std::cout << std::endl;
	}
    std::cout << std::endl;
}

//char* strsep(char** stringp, const char* delim);

inline void FileNameTotimeStamp (const string& fileName, unsigned long& timeStampSec, unsigned long& timeStampMicroSec)
{
	// Get just the filename
	string path, file;
	RetrievePath(fileName, path, file);

	// Extract the fields
	int year			= atoi(file.substr(0,4).c_str());
	int month			= atoi(file.substr(5,2).c_str());
	int day				= atoi(file.substr(8,2).c_str());
	int hour			= atoi(file.substr(11,2).c_str());
	int minute			= atoi(file.substr(14,2).c_str());
	int second			= atoi(file.substr(17,2).c_str());
	timeStampMicroSec	= atoi(file.substr(20,6).c_str());

	// Convert to data struct
	struct tm	timeinfo;
	timeinfo.tm_year	= year-1900;
	timeinfo.tm_mon		= month-1;
	timeinfo.tm_mday	= day;
	timeinfo.tm_hour	= hour;
	timeinfo.tm_min		= minute;
	timeinfo.tm_sec		= second;
	timeinfo.tm_isdst	= 0;
	timeinfo.tm_wday	= 0;

	// Convert to time_t
	time_t timeT = mktime(&timeinfo);
	timeStampSec = (unsigned long)timeT;
}

inline string GetDateString(const unsigned long timeStampSec, const unsigned long timeStampMicroSec)
{
	// Convert to struct
	time_t			rawtime		= timeStampSec;
	struct tm*		timeinfo	= localtime( &rawtime );

	// Convert to Text
	char dateStr[256];
    sprintf(dateStr, "%04d_%02d_%02d_%02d_%02d_%02d_%06lu",  timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeStampMicroSec);
	
	// Return date string
	return string(dateStr);
}

inline string GetDateString()
{
	// Get current date
	time_t rawtime;
	time (&rawtime);
	struct tm*	timeinfo = localtime(&rawtime);

	// Convert to Text
	char dateStr[256];
    sprintf(dateStr, "%04d_%02d_%02d_%02d_%02d_%02d_%06d", timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, 0);

	// Return date string
	return string(dateStr);
}

#endif //_UTILS_
