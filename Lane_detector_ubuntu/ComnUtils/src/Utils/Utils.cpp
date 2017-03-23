#include "Utils.h"
//#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#ifdef __unix__
    #include <dirent.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Errors
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Errors.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	code	The code.
 * @param	msg 	The message.
 * @param	func	The function.
 * @param	file	The file.
 * @param	line	The line.
 *****************************************************************************/
void _Error (const char* code, const string& msg, const string& func, const string& file, int line)
{
	// Get the time
	time_t rawtime;
	time ( &rawtime );
	struct tm * timeinfo = localtime ( &rawtime );

	// Show the error in the console

	Println("%s\n ERROR %s: %s. At %s, %s, line %d.", asctime (timeinfo), code, msg.c_str(), file.c_str(), func.c_str(), line);
	fflush(stdout);

	// Show error in a log file
	FILE* fp = Fopen(ERROR_LOG_FILE, "a");
	Fprintf(fp, "%s\n ERROR %s: %s. At %s, %s, line %d.", asctime (timeinfo), code, msg.c_str(), file.c_str(), func.c_str(), line);
	fclose(fp);
	//Pause();
}


/**************************************************************************//**
 * @brief	Error quit.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	code	The code.
 * @param	msg 	The message.
 * @param	func	The function.
 * @param	file	The file.
 * @param	line	The line.
 *****************************************************************************/
void _ErrorQuit (const char* code, const string& msg, const string& func, const string& file, int line)
{
	// Show the error
	_Error (code, msg, func, file, line);

	// End the program
	system("pause");
	exit(0);
}



///////////////////////////////////////////////////////////////////////////////
// Retrieve paths and extensions
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Retrieves a path.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filePath		Full pathname of the file.
 * @param [in,out]	path	Full pathname of the file.
 * @param [in,out]	file	The file.
 *****************************************************************************/
void RetrievePath(const string& filePath, string &path, string& file)
{
	path = filePath;
	file = filePath;

	int posLastSlash = (int)filePath.rfind('/', filePath.length());
	int posLastBackslash = (int)filePath.rfind('\\', filePath.length());
    int posLast = max(posLastSlash, posLastBackslash);

	path.erase(posLast+1, path.length()-posLast+1);
	file.erase(0, posLast+1);
}


/**************************************************************************//**
 * @brief	Retrieves an extension.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename		   	Filename of the file.
 * @param [in,out]	rawFilename	Filename of the raw file.
 * @param [in,out]	extension  	The extension.
 *****************************************************************************/
void RetrieveExtension(const string& filename, string& rawFilename, string &extension)
{
	int posLastDot = (int)filename.rfind('.', filename.length());
	if (posLastDot!=-1)
	{
		rawFilename = filename.substr(0, posLastDot);
		extension = filename.substr(posLastDot+1, filename.length());
	}
	else
	{
		rawFilename = filename;
		extension = "";
	}
}


/**************************************************************************//**
 * @brief	Retrieves all.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filePath				Full pathname of the file.
 * @param [in,out]	path			Full pathname of the file.
 * @param [in,out]	fullFileName	Filename of the full file.
 * @param [in,out]	rawFilename 	Filename of the raw file.
 * @param [in,out]	extension   	The extension.
 *****************************************************************************/
void RetrieveAll(const string& filePath, string& path, string& fullFileName, string& rawFilename, string& extension)
{
	RetrievePath(filePath, path, fullFileName);
	RetrieveExtension(fullFileName, rawFilename, extension);
}


/**************************************************************************//**
 * @brief	Standarize directory.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 31, 2012
 *
 * @param [in,out]	path	Full pathname of the file.
 *****************************************************************************/
void StandarizeDirectory(string& path)
{
	// Use always this '/' directory separator
	replace(path.begin(), path.end(), '\\', '/');

	// Make sure that the directory ends with '/'
	if (path.size()>0 && path[path.size()-1] != '/') path = path + "/";
}

void WindowsDirectory(string& path)
{
	// Use always this '/' directory separator
	replace(path.begin(), path.end(), '/', '\\');
}


///////////////////////////////////////////////////////////////////////////////
// Exists
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Queries if a given file exists.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	file	The file.
 *
 * @return	true if it succeeds, false if it fails.
 *****************************************************************************/
bool FileExists(const string& file)
{
	FILE*fp=fopen(file.c_str(), "r");
	if (fp==NULL) return false;
	else
	{
		fclose(fp);
		return true;
	}
}


/**************************************************************************//**
 * @brief	Queries if a given directory exists.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	directory_in	The directory in.
 *
 * @return	true if it succeeds, false if it fails.
 *****************************************************************************/
bool DirectoryExists(const string& directory_in)
{
#ifdef WIN32
	string directory = directory_in;
	if (directory[directory.size()-1] == '/' || directory[directory.size()-1 ]== '\\')
		directory.resize(directory.size()-1);
	struct stat st;
	stat(directory.c_str(), &st);
	return (st.st_mode & _S_IFDIR)!=0;
#else
    ErrorQuit(VSys_NotImplemented, directory_in);
    return false;
#endif
}


/**************************************************************************//**
 * @brief	Exists.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	directory	Pathname of the directory.
 *
 * @return	true if it succeeds, false if it fails.
 *****************************************************************************/
bool Exists(const string& directory)
{
	struct stat st;
	return (stat(directory.c_str(), &st)==0);
}



///////////////////////////////////////////////////////////////////////////////
// MS-DOS commands: Rename, Move, Delete, Mkdir, PWD
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Rename file.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	fileIn 	The file in.
 * @param	fileOut	The file out.
 *****************************************************************************/
void RenameFile (const string& fileIn, const string& fileOut)
{
	string fileIn2(fileIn);
	WindowsDirectory(fileIn2);

	string fileOut2(fileOut);
	WindowsDirectory(fileOut2);

	string command = "move " + fileIn2 + " " + fileOut2;
	system(command.c_str());
}


/**************************************************************************//**
 * @brief	Move file.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	fileIn 	The file in.
 * @param	fileOut	The file out.
 *****************************************************************************/
void MoveFile (const string& fileIn, const string& fileOut)
{
	string fileIn2(fileIn);
	WindowsDirectory(fileIn2);

	string fileOut2(fileOut);
	WindowsDirectory(fileOut2);

	string command = "move " + fileIn2 + " " + fileOut2;
	system(command.c_str());
}


/**************************************************************************//**
 * @brief	Copies the file.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	fileIn 	The file in.
 * @param	fileOut	The file out.
 *****************************************************************************/
void CopyFile (const string& fileIn, const string& fileOut)
{
	string fileIn2(fileIn);
	WindowsDirectory(fileIn2);

	string fileOut2(fileOut);
	WindowsDirectory(fileOut2);

	string command = "copy " + fileIn2 + " " + fileOut2;
	system(command.c_str());
}


/**************************************************************************//**
 * @brief	Deletes the file described by fileIn.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	fileIn	The file in.
 *****************************************************************************/
void DeleteFile (const string& fileIn)
{
	string fileIn2(fileIn);
	WindowsDirectory(fileIn2);

	string command = "del /q " + fileIn2;
	system(command.c_str());
}


/**************************************************************************//**
 * @brief	Creates a directory.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	pathIn	The path in.
 *
 * @return	The new directory.
 *****************************************************************************/
bool CreateDirectory(const string& pathIn)
{
	// Change characteres
	string path = pathIn;
	StandarizeDirectory(path);

	// Error control: Minimum path must be "U:\"
	int nextPos = 3;
	if (int(path.length())<nextPos) return false;

	// Create the directory by subPaths
	while(nextPos<int(path.length()))
	{
		// Find subPath
		string subpath = path.substr(0, path.find('/', nextPos));
		nextPos = (int)subpath.length()+1;
		if(subpath=="") subpath=path;

		// Create subpath if it does not exist
        #ifdef WIN32
            if(_access(subpath.c_str(), 0)==-1) _mkdir(subpath.c_str());
        #else
            if(access(subpath.c_str(), 0)==-1) mkdir(subpath.c_str(),0777);
        #endif
	}

	// Exit
	return true;

}


/**************************************************************************//**
 * @brief	Gets the working directory.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @return	The working directory.
 *****************************************************************************/
string GetWorkingDirectory()
{
	const int bufferLength=1024;
    #ifdef WIN32
        TCHAR pBuffer[bufferLength];
        GetCurrentDirectory(bufferLength, pBuffer);
    #else
        #ifdef UNICODE
            wchar_t pBuffer[bufferLength];
             getcwd((char *)pBuffer, bufferLength);
        #else
            char pBuffer[bufferLength];
             getcwd(pBuffer, bufferLength);
        #endif
    #endif
	string currentPath(pBuffer);
	StandarizeDirectory(currentPath);
	return currentPath;
}



///////////////////////////////////////////////////////////////////////////////
// String processing
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Trims right.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	input 	The input.
 * @param	number	Number of.
 *
 * @return	.
 *****************************************************************************/
string RTrim(const string& input, const int number)
{
	if (int(input.size())<number)
		return input;
	else
		return string(".." + input.substr(input.size()-number));
}


/**************************************************************************//**
 * @brief	Trims left.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	input 	The input.
 * @param	number	Number of.
 *
 * @return	.
 *****************************************************************************/
string LTrim(const string& input, const int number)
{
	if (int(input.size())<number)
		return input;
	else
		return string(input.substr(0, number) + "..");
}


/**************************************************************************//**
 * @brief	Tokenizes.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	str			  	The.
 * @param [in,out]	tokens	The tokens.
 * @param	delimiters	  	The delimiters.
 *****************************************************************************/
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
	// Clear the tokens vector
	tokens.clear();

    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);

        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


/**************************************************************************//**
 * @brief	Compose file name.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	path	 	Full pathname of the file.
 * @param	fileName 	Filename of the file.
 * @param	index	 	Zero-based index of the.
 * @param	extension	The extension.
 *
 * @return	.
 *****************************************************************************/
string ComposeFileName(const string& path, const string& fileName, const int index, const string& extension)
{
	string stdPath = path;
	StandarizeDirectory(stdPath);

	char file[1024];
	if (index == -1)
		sprintf (file, "%s%s.%s", stdPath.c_str(), fileName.c_str(), extension.c_str());
	else
		sprintf (file, "%s%s%d.%s", stdPath.c_str(), fileName.c_str(), index, extension.c_str());
	string strFile = file;
	return strFile;
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Merge files
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Merge files.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	input01	The first input 0.
 * @param	input02	The second input 0.
 * @param	output 	The output.
 *****************************************************************************/
void MergeFiles(string input01, string input02, string output)
{
	FILE* f1 = Fopen(input01, "r");
	FILE* f2 = Fopen(input02, "r");
	FILE* f3 = Fopen(output, "w");

	char a;
	int flag = fscanf(f1, "%c", &a);
	while(!feof(f1) && flag==1)
	{
		Fprintf(f3, "%c", a);
		flag=fscanf(f1, "%c", &a);
	}

	flag=fscanf(f2, "%c", &a);
	while(!feof(f2) && flag==1)
	{
		Fprintf(f3, "%c", a);
		flag=fscanf(f2, "%c", &a);
	}

	fclose(f1);
	fclose(f2);
	fclose(f3);
}


/**************************************************************************//**
 * @brief	Merge n files.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	vString	The string.
 * @param	output		   	The output.
 *****************************************************************************/
void MergeNFiles(vector<string>& vString, string output)
{
	FILE*foutput=fopen(output.c_str(), "w");
	if (foutput==NULL)
	{
		ErrorQuit(VSys_FileNotFound, output);
		return;
	}
		
	for (unsigned int i=0; i<vString.size(); i++)
	{
		FILE*finput=fopen(vString[i].c_str(), "r");
		if (finput==NULL)
			ErrorQuit(VSys_FileNotFound, vString[i]);
		else
		{
			char a;
			int flag=fscanf(finput, "%c", &a);
			while(!feof(finput) && flag==1)
			{
				Fprintf(foutput, "%c", a);
				flag=fscanf(finput, "%c", &a);
			}
			fclose(finput);
		}
	}

	fclose(foutput);
}



///////////////////////////////////////////////////////////////////////////////
// Read fileLists
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Reads the files list.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	listFileName   	Filename of the list file.
 * @param [in,out]	vFiles 	The files.
 * @param	bkeepPath	   	Full pathname of the bkeep file.
 * @param	bkeepExtensions	The bkeep extensions.
 * @param	extensionToFind	The extension to find.
 *****************************************************************************/
void ReadFilesList(const string& listFileName, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions, const string& extensionToFind)
{
	// 1. Retrieve Path and extension
	string file, path, rawfilename, extension;
	RetrievePath(listFileName, path, file);
	RetrieveExtension(file, rawfilename, extension);

	// 2. Load the sample list
	if		(extension == "")
		ReadDirectory(listFileName, vFiles, bkeepPath, bkeepExtensions, extensionToFind);
	else if	(rawfilename == "*")
		ReadDirectory(path, vFiles, bkeepPath, bkeepExtensions, extension);
	else if	(extension == "txt")
		ReadFilesListFromTXT(listFileName, vFiles, bkeepPath, bkeepExtensions);
	else
        ErrorQuit_(VSys_Error, ("Invalid extension \"%s\" type.", extension.c_str()));
}


/**************************************************************************//**
 * @brief	Reads a directory.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	directory	   	Pathname of the directory.
 * @param [in,out]	vFiles 	The files.
 * @param	bkeepPath	   	Full pathname of the bkeep file.
 * @param	bkeepExtensions	The bkeep extensions.
 * @param	extensionToFind	The extension to find.
 *****************************************************************************/
void ReadDirectory(const string& directory, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions, const vector<string>& vExtensionToFind)
{
	vector<string> vFilesAux;
	vFiles.clear();
	for (unsigned int i=0; i<vExtensionToFind.size(); i++)
	{
		ReadDirectory(directory, vFilesAux, bkeepPath, bkeepExtensions, vExtensionToFind[i]);
		vFiles.insert(vFiles.end(), vFilesAux.begin(), vFilesAux.end());
	}
}

void ReadDirectory(const string& directory, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions, const string& extensionToFind)
{
#ifdef WIN32
	// 0. Auxiliar variables
	char			templateToFind[1024];
	WIN32_FIND_DATA foundFileName;
	string			fileToAdd, fileName, rawFileName, extension;
	string			stdDirectory = directory;

	StandarizeDirectory(stdDirectory);

	// 1. Set the template files to find in the directory
	sprintf(templateToFind, "%s*.%s", stdDirectory.c_str(), extensionToFind.c_str());

	// 2. Find first file
    HANDLE hFind = FindFirstFile((LPCSTR)templateToFind, &foundFileName);
    if(hFind  == INVALID_HANDLE_VALUE)
		//ErrorQuit(VSys_FileNotFound, templateToFind);
		return;

	// 3. Find next files
	do {
		// Compose the name of the file to save
		fileToAdd = foundFileName.cFileName;
		if (fileToAdd.compare(".") !=0 && fileToAdd.compare("..") !=0)
		{
			if (!bkeepExtensions)
			{
				RetrieveExtension(fileToAdd, rawFileName, extension);
				fileToAdd = rawFileName;
			}
			if (bkeepPath)
				fileToAdd = stdDirectory + fileToAdd;

			// Include the file to the list
			vFiles.push_back(fileToAdd);
		}
	} while(FindNextFile(hFind, &foundFileName));

	FindClose(hFind);
#else
    struct dirent **namelist;
    int n;

   n = scandir(directory.c_str(), &namelist, NULL, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        for (int i = 0; i < n; i++) {
            string rawFileName, extension;
            string fileToAdd = namelist[i]->d_name;
            string stdDirectory = directory;
            if (fileToAdd.compare(".") !=0 && fileToAdd.compare("..") !=0)
            {
                if (!bkeepExtensions)
                {
                    RetrieveExtension(fileToAdd, rawFileName, extension);
                    fileToAdd = rawFileName;
                }
                if (bkeepPath)
                    fileToAdd = stdDirectory + fileToAdd;
                vFiles.push_back(fileToAdd);
                free(namelist[i]);
            }
        }
        free(namelist);
    }
#endif
}


/**************************************************************************//**
 * @brief	Reads the files list from text.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	listFileName   	Filename of the list file.
 * @param [in,out]	vFiles 	The files.
 * @param	bkeepPath	   	Full pathname of the bkeep file.
 * @param	bkeepExtensions	The bkeep extensions.
 * @param	extensionToFind	The extension to find.
 *****************************************************************************/
void ReadFilesListFromTXT(const string& listFileName, vector<string>& vFiles, const bool bkeepPath, const bool bkeepExtensions)
{
	// 1. Retrieve Path and extension
	string file, fileToAdd, path, rawFileName, extension;
	char fileName[1024];
	RetrievePath(listFileName, path, file);
	RetrieveExtension(file, rawFileName, extension);

	// 2. Open the file
	FILE* fp = fopen(listFileName.c_str(), "r");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, listFileName);
		return;
	}

	// 3. Read the files in the list
	while(!feof(fp))
	{
		// 3.1 Read the name of the file
		fscanf(fp, "%s", fileName);
		fscanf(fp, "\n");

		// 3.2 Compose the name of the file to save
		fileToAdd = fileName;
		if (!bkeepExtensions)
		{
			RetrieveExtension(fileToAdd, rawFileName, extension);
			fileToAdd = rawFileName;
		}
		if (bkeepPath)
			fileToAdd = path + fileToAdd;

		// 3.3 Include the file to the list
		vFiles.push_back(fileToAdd);
	}

	// 3. Close the file
	fclose(fp);
}


char* strsep(char** stringp, const char* delim)
{
  char* start = *stringp;
  char* p;

  p = (start != NULL) ? strpbrk(start, delim) : NULL;

  if (p == NULL)
  {
    *stringp = NULL;
  }
  else
  {
    *p = '\0';
    *stringp = p + 1;
  }

  return start;
}
