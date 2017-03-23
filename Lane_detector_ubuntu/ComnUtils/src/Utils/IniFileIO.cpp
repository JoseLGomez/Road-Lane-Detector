#ifdef __unix__
#define MAX_LINE_LENGTH 2048
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "unistd.h"
#include <string>
#include <string.h>

/*****************************************************************
* Function:     read_line()
* Arguments:    <FILE *> fp - a pointer to the file to be read from
*               <char *> bp - a pointer to the copy buffer
* Returns:      TRUE if successful FALSE otherwise
******************************************************************/
int read_line(FILE *fp, char *bp) {
  char c = '\0';
  int i = 0;
  /* Read one line from the source file */
  while( (c = getc(fp)) != '\n' ) {
    if( c == EOF )         /* return FALSE on unexpected EOF */
      return(0);
    if (c != '\r')  /* Again, MS sucks, ignore CR */
      bp[i++] = c;
  }
  bp[i] = '\0';
  return(1);
}



/*************************************************************************
 * Function:    write_private_profile_string()
 * Arguments:   <char *> section - the name of the section to search for
 *              <char *> entry - the name of the entry to find the value of
 *              <char *> buffer - pointer to the buffer that holds the string
 *              <char *> file_name - the name of the .ini file to read from
 * Returns:     TRUE if successful, otherwise FALSE
 *************************************************************************/
int WritePrivateProfileString(const char *section, const char *entry,const char *buffer, const char *file_name)
{
    FILE *rfp, *wfp;
    char tmp_name[15] = "PRIVPROFXXXXXX";
    char buff[MAX_LINE_LENGTH];
    char t_section[MAX_LINE_LENGTH];
    int len = strlen(entry);
    int tmpfd = mkstemp(tmp_name); /* Get a temporary file name to copy to */
    if (tmpfd == -1) return 0;
    sprintf(t_section,"[%s]",section);/* Format the section name */
    if( !(rfp = fopen(file_name,"r")) )  /* If the .ini file doesn't exist */
    {   if( !(wfp = fopen(file_name,"w")) ) /*  then make one */
        {   return(0);   }
        fprintf(wfp,"%s\n",t_section);
        fprintf(wfp,"%s=%s\n",entry,buffer);
        fclose(wfp);
        return(1);
    }
    if( !(wfp = fdopen(tmpfd,"w")) )
    {   fclose(rfp);
        return(0);
    }

    /* Move through the file one line at a time until a section is
     * matched or until EOF. Copy to temp file as it is read. */

    do
    {   if( !read_line(rfp,buff) )
        {   /* Failed to find section, so add one to the end */
            //fprintf(wfp,"%s\n",buff);
            fprintf(wfp,"\n%s\n",t_section);
            fprintf(wfp,"%s=%s\n",entry,buffer);
            /* Clean up and rename */
            fclose(rfp);
            fclose(wfp);
            unlink(file_name);
            rename(tmp_name,file_name);
            return(1);
        }
        fprintf(wfp,"%s\n",buff);
    } while( strncasecmp(buff,t_section, strlen(t_section))!=0 );

    /* Now that the section has been found, find the entry. Stop searching
     * upon leaving the section's area. Copy the file as it is read
     * and create an entry if one is not found.  */

    std::string entryNoSpaces;
    entryNoSpaces = std::string(entry);
    entryNoSpaces.erase(0, entryNoSpaces.find_first_not_of(" \t\r\n"));

    while( 1 )
    {   if( !read_line(rfp,buff) || strncasecmp(buff,"[",1)==0)
        {   /* EOF without an entry so make one */
            // if (strncasecmp(buff,"[",1)!=0) fprintf(wfp,"%s\n",buff);
            fprintf(wfp,"%s=%s\n",entry,buffer);
            /* Clean up and rename */
            fclose(rfp);
            fclose(wfp);
            unlink(file_name);
            rename(tmp_name,file_name);
            return(1);

        }
        std::string strNoSpaces;
        strNoSpaces = std::string(buff);
        strNoSpaces.erase(0, strNoSpaces.find_first_not_of(" \t\r\n"));
        //if( strncasecmp(strNoSpaces.c_str(),entry,len)==0 || strNoSpaces[0] == '\0' )
        if( (strncasecmp(strNoSpaces.c_str(),entryNoSpaces.c_str(),len)==0 ) && (strNoSpaces[len]=='=' || strNoSpaces[len]==' ' || strNoSpaces[len]!='\t') || strNoSpaces[0] == '\0' )
            break;
        fprintf(wfp,"%s\n",buff);
    }

    if( buff[0] == '\0' )
    {   fprintf(wfp,"%s=%s\n",entry,buffer);
        do
        {
            fprintf(wfp,"%s\n",buff);
        } while( read_line(rfp,buff) );
    }
    else
    {   fprintf(wfp,"%s=%s\n",entry,buffer);
        while( read_line(rfp,buff) )
        {
             fprintf(wfp,"%s\n",buff);
        }
    }

    /* Clean up and rename */
    fclose(wfp);
    fclose(rfp);
    unlink(file_name);
    rename(tmp_name,file_name);
    return(1);
}



/**************************************************************************
* Function:     GetPrivateProfileString()
* Arguments:    <char *> section - the name of the section to search for
*               <char *> entry - the name of the entry to find the value of
*               <char *> def - default string in the event of a failed read
*               <char *> buffer - a pointer to the buffer to copy into
*               <int> buffer_len - the max number of characters to copy
*               <char *> file_name - the name of the .ini file to read from
* Returns:      the number of characters copied into the supplied buffer
***************************************************************************/
int GetPrivateProfileString(const char *section, const char *entry, const char *def, char *buffer, int buffer_len, const char *file_name)
{
  char buff[MAX_LINE_LENGTH];
  char *ep;
  char t_section[MAX_LINE_LENGTH];
  char *ptr;
  int tlen;
  int len = strlen(entry);

  FILE *fp = fopen(file_name,"r");
  if( !fp ) return(0);

  sprintf(t_section,"[%s]",section);    /* Format the section name */
  tlen=strlen(t_section);


  /*  Move through file 1 line at a time until a section is matched or EOF */
  do
    {   if( fgets(buff,MAX_LINE_LENGTH-1,fp) == NULL)
      {   fclose(fp);
      strncpy(buffer,def,buffer_len);
      return(strlen(buffer));
      }
    }
  while( strncasecmp(buff,t_section,tlen) );

  /* Now that the section has been found, find the entry.
   * Stop searching upon leaving the section's area. */
  std::string strNoSpaces;
  do
    {
  if ( fgets(buff,MAX_LINE_LENGTH-1,fp) == NULL || strncasecmp(buff,"[",1)==0)
        {   fclose(fp);
        strncpy(buffer,def,buffer_len);
        return(strlen(buffer));
        }
        strNoSpaces = std::string(buff);
        strNoSpaces.erase(0, strNoSpaces.find_first_not_of(" \t\r\n"));

    }  while( strncasecmp(strNoSpaces.c_str(),entry,len)!=0 || (strNoSpaces[len]!='=' && strNoSpaces[len]!=' ' && strNoSpaces[len]!='\t'));
  fclose(fp);

  ep = strchr(buff,'=');    /* Parse out the equal sign */
  if (ep == NULL)
    {
      strncpy(buffer,def,buffer_len);
      return(strlen(buffer));
    }
  ep++;

  /* remove leading spaces*/
  while (*ep && (isspace(*ep) || *ep == 10))
    ep++;
  if (ep == NULL)
    {
      strncpy(buffer,def,buffer_len);
      return(strlen(buffer));
    }

  /* remove trailing spaces*/
  ptr = ep;
  while(*ptr) // go to the end, point to a NULL
    ptr++;

  ptr--;
  while (ptr > ep)  // backup and put in nulls if there is a space
    {
      if (isspace(*ptr) || *ep == 10)
        {
          *ptr = 0;
          ptr--;
        }
      else
        break;
    }

  /* Copy up to buffer_len chars to buffer */
  strncpy(buffer,ep,buffer_len - 1);

  buffer[buffer_len-1] = '\0';
  return(strlen(buffer));
}
#endif

#include "IniFileIO.h"
#ifdef WIN32
    #include <Windows.h>
    #include <strsafe.h>
#endif
using std::max;
using std::min;
#include <ios>
#include <stdio.h>
#include <time.h>

// Constants
const char			c_valuesSeparator2			= ',';
const char			c_beginningOfCommentChar2	= '#';
const unsigned long c_bufferSize2				= 1048576;
const int			c_maxValues					= 5000;
//#pragma warning (disable:4995) //Deprecated warning

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	maxVarSize	Size of the maximum variable.
 * @param	maxValSize	Size of the maximum value.
 *****************************************************************************/
CIniFileIO::CIniFileIO(const int maxVarSize, const int maxValSize):
	m_maxVarSize(maxVarSize),
    m_maxValSize(maxValSize),
    m_buffer(NULL)
{
	// Init the buffer
	m_buffer = new char[c_bufferSize2];
}


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	IniFilename	Filename of the initialise file.
 * @param	maxVarSize 	Size of the maximum variable.
 * @param	maxValSize 	Size of the maximum value.
 *****************************************************************************/
CIniFileIO::CIniFileIO(const string& IniFilename, const int maxVarSize, const int maxValSize):
	m_maxVarSize(maxVarSize),
    m_maxValSize(maxValSize),
    m_buffer(NULL)
{
	// Init the buffer
	m_buffer = new char[c_bufferSize2];

	// Initialize
	Initialize(IniFilename);
}


CIniFileIO::~CIniFileIO()
{
	// Finish
	Finish();

	// Release the memory
	if (m_buffer!=NULL)
	{
		delete []m_buffer;
		m_buffer = NULL;
	}
}



///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Initializes this object.
 *
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	activeIniFilename	Filename of the active initialise file.
 *****************************************************************************/
void CIniFileIO::Initialize(string activeIniFilename)
{
    // Test that the file exist
    m_IniFilename = activeIniFilename;
    FILE* fp;
    fp = fopen(activeIniFilename.c_str(), "r");

	if (fp==NULL)
	{
		// Show error
		printf("ERROR: The parameters file doesn't exist. A new file is created and default values are going to be set there.\n");
		Pause();

		// Create the file
		fp = fopen(activeIniFilename.c_str(), "w");
		
		// Set the file Header
		if (fp != NULL)
		{
			// Get file name
			string path, file;
			RetrievePath(activeIniFilename, path, file);
		
			// Get the time
			time_t rawtime;
			time ( &rawtime );
			struct tm * timeinfo = localtime ( &rawtime );

			// Write the header
			fprintf(fp, "##################################################################################\n");
			fprintf(fp, "#   %s: Parameters file for the VisionSys\n", file.c_str());
			fprintf(fp, "# \n");
			fprintf(fp, "#   Authors     : David Vazquez (David.Vazquez@cvc.uab.es)\n");
			fprintf(fp, "#                 David Geronimo (dgeronimo@cvc.uab.es)\n");
			fprintf(fp, "#\n");
			fprintf(fp, "#   Created     : %s", asctime (timeinfo));
			fprintf(fp, "#\n");
			fprintf(fp, "#   Description : It is the default parameters for the INRIA - HOG\n");
			fprintf(fp, "#                 experiment. This file has been automatically created.\n");
			fprintf(fp, "#\n");
			fprintf(fp, "#   TODO        : Check that all the parameters are correct.\n");
			fprintf(fp, "#                 Change the parameters to fit your experiment.\n");
			fprintf(fp, "#                 Use the bellow separator to divide the comments (Optional).\n");
			fprintf(fp, "#\n");
			fprintf(fp, "##################################################################################\n");
			fprintf(fp, "\n");
			fprintf(fp, "#################################################\n");
			fprintf(fp, "#\n");
			fprintf(fp, "#          SECTION: TASKS\n");
			fprintf(fp, "#\n");
			fprintf(fp, "#################################################\n");
		}
	}
	if (fp==NULL)
		ErrorQuit(VSys_FileNotFound, activeIniFilename);
	else
		fclose(fp);
}


/**************************************************************************//**
 * @brief	Initializes for write.
 *
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	activeIniFilename	Filename of the active initialise file.
 *****************************************************************************/
void CIniFileIO::InitializeWrite(string activeIniFilename)
{
	// Save the data
	m_IniFilename = activeIniFilename;

	// Test that the file exist
	FILE* fp = fopen(activeIniFilename.c_str(), "w");
	if (fp==NULL)
		ErrorQuit(VSys_FileNotFound, activeIniFilename);
	else
		fclose(fp);
}


/**************************************************************************//**
 * @brief	Initializes for update.
 *
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	activeIniFilename	Filename of the active initialise file.
 *****************************************************************************/
void CIniFileIO::InitializeUpdate(string activeIniFilename)
{
	// Save the data
	m_IniFilename = activeIniFilename;

	// Test that the file exist
	FILE* fp = fopen(activeIniFilename.c_str(), "a");
	if (fp==NULL)
		ErrorQuit(VSys_FileNotFound, activeIniFilename);
	else
		fclose(fp);
}


/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CIniFileIO::Finish()
{
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Auxiliar
///////////////////////////////////////////////////////////////////////////////



/**************************************************************************//**
 * @brief	Error exit.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	lpszFunction	The function.
 *****************************************************************************/
//void ErrorExit(LPTSTR lpszFunction) 
//{ 
//    // Retrieve the system error message for the last-error code
//
//    LPVOID lpMsgBuf;
//    LPVOID lpDisplayBuf;
//    DWORD dw = GetLastError(); 
//
//    FormatMessage(
//        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//        FORMAT_MESSAGE_FROM_SYSTEM |
//        FORMAT_MESSAGE_IGNORE_INSERTS,
//        NULL,
//        dw,
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//        (LPTSTR) &lpMsgBuf,
//        0, NULL );
//
//    // Display the error message and exit the process
//
//    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
//        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
//    StringCchPrintf((LPTSTR)lpDisplayBuf, 
//        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
//        TEXT("%s failed with error %d: %s"), 
//        lpszFunction, dw, lpMsgBuf); 
//    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
//
//    LocalFree(lpMsgBuf);
//    LocalFree(lpDisplayBuf);
//    ExitProcess(dw); 
//}




/**************************************************************************//**
 * @brief	Vect 2 mat.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	value	 	The value.
 * @param	maxValues	The maximum values.
 *
 * @return	.
 *****************************************************************************/
template <class T> vector<vector<T>> Vect2Mat(const vector<T>& value, const int maxValues)
{
	// Convert the vector to a matrix
	vector<vector<T>> matValue;
	int nRows = (int)ceil((float)value.size()/(float)maxValues);
	matValue.resize(nRows);

	int k=0;
	for (int i=0; i<nRows; i++)
	{
		// Allocate space for this row's elements
		int nSamples = ((i<nRows-1)? maxValues : (int)value.size() - (nRows-1)*maxValues);
		matValue[i].resize(nSamples);
		
		// Copy the elements
		for (int j=0; j<nSamples; j++) matValue[i][j] = value[k++];
	}

	return matValue;
}


/**************************************************************************//**
 * @brief	Mat 2 vector.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	value				The value.
 * @param [in,out]	vecValue	The vector value.
 *****************************************************************************/
template <class T> void Mat2Vec(const vector<vector<T>>& value, vector<T>& vecValue)
{
	// Convert the vector to a matrix
	unsigned int size = 0;
	for (unsigned int i=0; i<value.size(); i++) size += (unsigned int)value[i].size();
	vecValue.resize(size);

	unsigned int k=0;
	for (unsigned int i=0; i<value.size(); i++){
	for (unsigned int j=0; j<value[i].size(); j++)
		vecValue[k++] = value[i][j];}
}



///////////////////////////////////////////////////////////////////////////////
// Functions: ReadSection
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, string& value, const string& defaultValue, const string& comment) const
{
	// Enhance the comment
	string strComment = "[String value] " + comment;

	// Get the value
	value = GetStringValue(section, variable, defaultValue, strComment);
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, unsigned int& value, const unsigned int& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
	sprintf (strDefaultValue, "%d", defaultValue);

	// Enhance the comment
	string strComment = "[uInt value]   " + comment;

	// Get the value
	value = atoi(GetStringValue(section, variable, strDefaultValue, strComment).c_str());
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, unsigned char& value, const unsigned char& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
	sprintf (strDefaultValue, "%c", defaultValue);

	// Enhance the comment
	string strComment = "[uChar value]  " + comment;

	// Get the value
	value = static_cast<unsigned char>(atoi(GetStringValue(section, variable, strDefaultValue, strComment).c_str()));
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, unsigned long& value, const unsigned long& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
    sprintf (strDefaultValue, "%lu", defaultValue);
	
	// Enhance the comment
	string strComment = "[uLong value]  " + comment;

	// Get the value
	char *endPtr;
	value = strtoul(GetStringValue(section, variable, strDefaultValue, strComment).c_str(), &endPtr, 10);
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, bool& value, const bool& defaultValue, const string& comment) const
{
	// Set the default value into string format
	string strDefaultValue = (defaultValue ? "true" : "false");

	// Enhance the comment
	string strComment = "[True | False] " + comment;

	// Get the value
	string stringValue = GetStringValue(section, variable, strDefaultValue, strComment).c_str();
	std::transform(stringValue.begin(), stringValue.end(), stringValue.begin(), tolower);
	if ((stringValue == "y") || (stringValue == "yes") || (stringValue == "t") || (stringValue == "true") || (stringValue== "1"))
	{
		value = true;
	}
	else if ((stringValue == "n") || (stringValue == "no") || (stringValue == "f") || (stringValue == "false") || (stringValue== "0"))
	{
		value = false;
	}
	else
	{
		value = false;
		ErrorQuit_(VSys_IncINIParam, ("Expected a boolean value\n\nFile: %s\nSection: %s\nVar: %s", m_IniFilename.c_str(), section.c_str(), variable.c_str()));
	}
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, int& value, const int& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
	sprintf (strDefaultValue, "%d", defaultValue);

	// Enhance the comment
	string strComment = "[Int value]    " + comment;

	// Get the value
	value = atoi(GetStringValue(section, variable, strDefaultValue, strComment).c_str());
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, long& value, const long& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
    sprintf (strDefaultValue, "%ld", defaultValue);

	// Enhance the comment
	string strComment = "[Long value]   " + comment;

	// Get the value
	char *endPtr;
	value = strtol(GetStringValue(section, variable, strDefaultValue, strComment).c_str(), &endPtr, 10);
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, float& value, const float& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
	sprintf (strDefaultValue, "%0.9f", defaultValue);

	// Enhance the comment
	string strComment = "[Float value]  " + comment;

	// Get the value
	value = (float)atof(GetStringValue(section, variable, strDefaultValue, strComment).c_str());
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, double& value, const double& defaultValue, const string& comment) const
{
	// Set the default value into string format
	char strDefaultValue[256];
	sprintf (strDefaultValue, "%0.9f", defaultValue);

	// Enhance the comment
	string strComment = "[Double value] " + comment;

	// Get the value
	value = strtod(GetStringValue(section, variable, strDefaultValue, strComment).c_str(), 0);
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, vector<int>& value, const vector<int>& defaultValue, const string& comment) const
{
	// Set the default value into string format
	std::stringstream strDefaultValue;
	if (defaultValue.size() > 0) strDefaultValue << defaultValue[0];
	for (unsigned int i=1; i<defaultValue.size(); i++) strDefaultValue << ',' << defaultValue[i];

	// Enhance the comment
	string strComment = "[Int vector]   " + comment;

	// Get the value
	value.clear();
	string stringValue = GetStringValue(section, variable, strDefaultValue.str(), strComment);
	string element("");
	if (stringValue.size()>0)
	{
		while(GetValueFromValueSequence(stringValue, element))
			value.push_back(atoi(element.c_str()));
		value.push_back(atoi(element.c_str()));
	}
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, vector<float>& value, const vector<float>& defaultValue, const string& comment) const
{
	// Set the default value into string format
	std::stringstream strDefaultValue;
	if (defaultValue.size() > 0) strDefaultValue << defaultValue[0];
	for (unsigned int i=1; i<defaultValue.size(); i++) strDefaultValue << ',' << defaultValue[i];

	// Enhance the comment
	string strComment = "[Float vector] " + comment;

	// Get the value
	value.clear();
	string stringValue = GetStringValue(section, variable, strDefaultValue.str(), strComment);
	string element("");
	if (stringValue.size()>0)
	{
		while(GetValueFromValueSequence(stringValue, element))
			value.push_back((float)atof(element.c_str()));
		value.push_back((float)atof(element.c_str()));
	}
}



/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, vector<double>& value, const vector<double>& defaultValue, const string& comment) const
{
	// Set the default value into string format
	std::stringstream strDefaultValue;
	if (defaultValue.size() > 0) strDefaultValue << defaultValue[0];
	for (unsigned int i=1; i<defaultValue.size(); i++) strDefaultValue << ',' << defaultValue[i];

	// Enhance the comment
	string strComment = "[Double vector] " + comment;

	// Get the value
	value.clear();
	string stringValue = GetStringValue(section, variable, strDefaultValue.str(), strComment);
	string element("");
	if (stringValue.size()>0)
	{
		while(GetValueFromValueSequence(stringValue, element))
			value.push_back((double)atof(element.c_str()));
		value.push_back((double)atof(element.c_str()));
	}
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, vector<vector<int>>& value, const string& comment) const
{
	// 1. Read number of rows
	char sect[1024];
	int nSamples;
	sprintf(sect, "numberOf_%s", variable.c_str());
	ReadSection(section, sect, nSamples, 0, comment);

	// 2. Resize the output vector
	value.resize(nSamples);

	// 3. Read each row
	for (int i=0; i<nSamples; i++)
	{
		sprintf(sect, "%s%d", variable.c_str(), i);
		ReadSection(section, sect, value[i], vector<int>(), comment);
	}
}


/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadSection(const string& section, const string& variable, vector<vector<float>>& value, const string& comment) const
{
	// 1. Read number of rows
	char sect[1024];
	int nSamples;
	sprintf(sect, "numberOf_%s", variable.c_str());
	ReadSection(section, sect, nSamples, 0, comment);

	// 2. Resize the output vector
	value.resize(nSamples);

	// 3. Read each row
	for (int i=0; i<nSamples; i++)
	{
		sprintf(sect, "%s%d", variable.c_str(), i);
		ReadSection(section, sect, value[i], vector<float>(), comment);
	}
}

/**************************************************************************//**
 * @brief	Reads a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
/*void CIniFileIO::ReadSection(const string& section, const string& variable, CColor& value, const CColor& defaultValue, const string& comment) const
{
    // Set the default value into a vector
    vector<float> vDefValues(4);
    vDefValues[0] = (float) defaultValue.r;
    vDefValues[1] = (float) defaultValue.g;
    vDefValues[2] = (float) defaultValue.b;
    vDefValues[3] = defaultValue.a;

    // Get the value
    vector<float> vValues;
    ReadSection(section, variable, vValues, vDefValues, comment);

    // Set the vector into a color format
    value.r = (int)vValues[0];
    value.g = (int)vValues[1];
    value.b = (int)vValues[2];
    value.a = vValues[3];
}*/

/**************************************************************************//**
 * @brief	Reads a long section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadLongSection(const string& section, const string& variable, vector<float>& value, const string& comment) const
{
	vector<vector<float>> matValue;
	ReadSection(section, variable, matValue, comment);
	Mat2Vec(matValue, value);
}


/**************************************************************************//**
 * @brief	Reads a long section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @author	David Geronimo (dgeronimo@cvc.uab.es) 
 * @author  Antonio M .López (antonio@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section		 	The section.
 * @param	variable	 	The variable.
 * @param [in,out]	value	The value.
 * @param	defaultValue 	The default value.
 * @param	comment		 	The comment.
 *****************************************************************************/
void CIniFileIO::ReadLongSection(const string& section, const string& variable, vector<int>& value, const string& comment) const
{
	vector<vector<int>> matValue;
	ReadSection(section, variable, matValue, comment);
	Mat2Vec(matValue, value);
}



///////////////////////////////////////////////////////////////////////////////
// Functions: WriteSection
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const string& value, const string& comment) const
{
	SetStringValue(section, variable, value, comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const unsigned int& value, const string& comment) const
{
	std::stringstream ss;
	ss << value;
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const unsigned char& value, const string& comment) const
{
	std::stringstream ss;
	ss << value;
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const unsigned long& value, const string& comment) const
{
	std::stringstream ss;
	ss << value;
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const bool& value, const string& comment) const
{
	std::stringstream ss;

	if (value)
		ss << "true";
	else
		ss << "false";

	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const int& value, const string& comment) const
{
	std::stringstream ss;
	ss << value;
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const long& value, const string& comment) const
{
	std::stringstream ss;
	ss << value;
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const float& value, const string& comment) const
{
	//std::stringstream ss;
	//ss.precision(9);
	//ss.width(9);
	////ss.setf(ios::fixed);
	//ss << value;
	//SetStringValue(section, variable, ss.str(), comment);

	char str[64];
	sprintf(str, "%0.9f", value);
	SetStringValue(section, variable, str, comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const double& value, const string& comment) const
{
	//std::stringstream ss;
	//SetStringValue(section, variable, ss.str(), comment);

	char str[64];
    sprintf(str, "%0.9f", value);
	SetStringValue(section, variable, str, comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const vector<int>& value, const string& comment) const
{
	std::stringstream ss;
	
	if (value.size() > 0)
		ss << value[0];

	for (unsigned int i=1; i<value.size(); i++)
		ss << ',' << value[i];
	
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const vector<float>& value, const string& comment) const
{
	std::stringstream ss;
	
	if (value.size() > 0)
		ss << value[0];

	for (unsigned int i=1; i<value.size(); i++)
		ss << ',' << value[i];
	
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const vector<double>& value, const string& comment) const
{
	std::stringstream ss;
	
	if (value.size() > 0)
		ss << value[0];

	for (unsigned int i=1; i<value.size(); i++)
		ss << ',' << value[i];
	
	SetStringValue(section, variable, ss.str(), comment);
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const vector<vector<int>>& value, const string& comment) const
{
	// 1. Write number of rows
	char sect[1024];
	sprintf(sect, "numberOf_%s", variable.c_str());
	WriteSection(section, sect, (int)value.size(), comment);

	// 2. Write each row
	for (unsigned int i=0; i<value.size(); i++)
	{
		sprintf(sect, "%s%d", variable.c_str(), i);
		WriteSection(section, sect, value[i]);
	}
}


/**************************************************************************//**
 * @brief	Writes a section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteSection(const string& section, const string& variable, const vector<vector<float>>& value, const string& comment) const
{
	// 1. Write number of rows
	char sect[1024];
	sprintf(sect, "numberOf_%s", variable.c_str());
	WriteSection(section, sect, (int)value.size(), comment);

	// 2. Write each row
	for (unsigned int i=0; i<value.size(); i++)
	{
		sprintf(sect, "%s%d", variable.c_str(), i);
		WriteSection(section, sect, value[i]);
	}
}


/**************************************************************************//**
 * @brief	Writes a long section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteLongSection(const string& section, const string& variable, const vector<int>& value, const string& comment) const
{
	vector<vector<int>> matValue = Vect2Mat(value, c_maxValues);
	WriteSection(section, variable, matValue, comment);
}


/**************************************************************************//**
 * @brief	Writes a long section.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::WriteLongSection(const string& section, const string& variable, const vector<float>& value, const string& comment) const
{
	vector<vector<float>> matValue = Vect2Mat(value, c_maxValues);
	WriteSection(section, variable, matValue, comment);
}





///////////////////////////////////////////////////////////////////////////////
// Functions: Auxiliar
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Gets a string value.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section			The section.
 * @param	variable		The variable.
 * @param	defaultValue	The default value.
 * @param	comment			The comment.
 *
 * @return	The string value.
 *****************************************************************************/
string CIniFileIO::GetStringValue(const string& section, const string& variable, const string& defaultValue, const string& comment) const
{
	// 1. Read the string value in the [SECTION] Variable = string
    long numberOfReadChars = GetPrivateProfileString(section.c_str(), variable.c_str(), "", m_buffer, c_bufferSize2, m_IniFilename.c_str());

	// 2. Copy from the buffer to the output string
	string stringValue = m_buffer;

	// 3. Verify that the tag exist
	if(numberOfReadChars==0)
	{
		// Write the default value into the INI file
		SetStringValue(section, variable, defaultValue, comment + " (" + defaultValue + ")");

		// Set the default value as output
		stringValue = defaultValue;

		// Show an error
		Print("\n > ERROR -8: Unknown INI parameter: Set to default value\n"
			   "  > File    : %s\n"
			   "  > Section : %s\n"
			   "  > Var     : %s\n"
			   "  > Default : %s  #%s\n"
			   "  > Please, check if the parameter is ok!\n",
			   m_IniFilename.c_str(), section.c_str(), variable.c_str(), defaultValue.c_str(), LTrim(comment, 40).c_str());
		//getchar();
	}
	
	// 4. Verify that the full string have been readed
    if((unsigned long)numberOfReadChars>=c_bufferSize2-1)
		ErrorQuit_(VSys_Error, ("Tag to long. Increase buffer size.\n\nFile: %s\nSection: %s\nVar: %s", m_IniFilename.c_str(), section.c_str(), variable.c_str()));
	
	// 5. Delete the comments, tabulators and blank spaces
	DeleteCommentsAndTabulatorsAndBlankSpaces(stringValue);

	// 6. If the value it is empty set the output as ""
	if (stringValue == "empty")	stringValue = "";

	// 7. return the string value
	return stringValue;
}


/**************************************************************************//**
 * @brief	Sets a string value.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	section 	The section.
 * @param	variable	The variable.
 * @param	value   	The value.
 * @param	comment 	The comment.
 *****************************************************************************/
void CIniFileIO::SetStringValue(const string& section, const string& variable, const string& value, const string& comment) const
{
	// Check if the variable exists into the file
    long numberOfReadChars = GetPrivateProfileString(section.c_str(), variable.c_str(), "", m_buffer, c_bufferSize2, m_IniFilename.c_str());

	// Compute needed spaces to add to the variable and value to have an idented INI file
	int nSpacesVar = max(m_maxVarSize - (int)variable.size(), 0);
	int nSpacesVal = max(m_maxValSize - (int)value.size(), 0);

	// Add the spaces to the variable and value
	string strVar =  (numberOfReadChars==0 ? "\t" + variable : variable);
	string strVal =  string(nSpacesVar, ' ') + (value.empty() ?  " empty" : " " + value) + string(nSpacesVal, ' ') + "  # " + comment;

	// Write the data into the INI file
	if (WritePrivateProfileString(section.c_str(), strVar.c_str(), strVal.c_str(), m_IniFilename.c_str())==0)
	{
		//ErrorExit("Writing");
		printf("Error writing\n");
		exit(0);
	}
		
}


/**************************************************************************//**
 * @brief	Deletes the comments and tabulators and blank spaces described by value.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	value	The value.
 *****************************************************************************/
void CIniFileIO::DeleteCommentsAndTabulatorsAndBlankSpaces(string& value) const
{
	// Find the comment into the value
	int beginningOfCommentPosition = (int)value.find(c_beginningOfCommentChar2, 0);

	// Remove the comment from the value
	if (beginningOfCommentPosition!=-1)
		value = value.substr(0, beginningOfCommentPosition-1);

	// Find the quotations into the value
	int beginningOfQuotations = (int)value.find('"', 0);
	int endOfQuotations = (int)value.find('"', beginningOfQuotations+1);

	// Remove spaces and quotations
	if (beginningOfQuotations==-1 || endOfQuotations ==-1)
		// Remain only what it is into the quotations
		value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
	else
		// Remove the spaces from the value
		value = value.substr(beginningOfQuotations+1, endOfQuotations-1);
}


/**************************************************************************//**
 * @brief	Gets a value from value sequence.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	valueSequence	The value sequence.
 * @param [in,out]	value		 	The value.
 *
 * @return	The value from value sequence.
 *****************************************************************************/
int CIniFileIO::GetValueFromValueSequence(string& valueSequence, string& value) const
{
	// Format: "value1,value2,value3,...."
	// i.e. values have to be separated by commas, without any other character between them

	int separatorPosition = (int)valueSequence.find(c_valuesSeparator2, 0);
	if(separatorPosition!=-1)
	{
		value = valueSequence.substr(0, separatorPosition);
		valueSequence.erase(0, separatorPosition+1);
		return 1;
	}
	else
	{
		value = valueSequence;
		valueSequence = "";
		return 0;
	}
}

// Test
	//CIniFileIO IniFileIO("./Parameters.ini");
	//string strValue;
	//unsigned int uIntValue;
	//unsigned char uCharValue;
	//unsigned long uLongValue;
	//bool boolValue;
	//int intValue;
	//long longValue;
	//float floatValue;
	//double doubleValue;
	//vector<int> vValueInt;
	//vector<float> vValueFloat;
	//vector<vector<int>> vvValueInt;
	//vector<vector<float>> vvValueFloat;
	//vector<int> vValueIntLong;
	//vector<float> vValueFloatLong;

	//IniFileIO.ReadSection("NEW_SECTION", "stringVariable", strValue, "Hello world", "new string value");

	//IniFileIO.ReadSection("NEW_SECTION", "unsignedIntVariable", uIntValue, 1, "new unsigned int value");
	//IniFileIO.ReadSection("NEW_SECTION", "unsignedCharVariable", uCharValue, 'a', "new unsigned char value");
	//IniFileIO.ReadSection("NEW_SECTION", "unsignedLongVariable", uLongValue, 2, "new unsigned long value");

	//IniFileIO.ReadSection("NEW_SECTION", "boolVariable", boolValue, true, "new bool value");
	//IniFileIO.ReadSection("NEW_SECTION", "intVariable", intValue, 3, "new int value");
	//IniFileIO.ReadSection("NEW_SECTION", "longVariable", longValue, 4, "new long value");
	//IniFileIO.ReadSection("NEW_SECTION", "floatVariable", floatValue, 5.6f, "new float value");
	//IniFileIO.ReadSection("NEW_SECTION", "doubleVariable", doubleValue, 7.7, "new double value");

	//IniFileIO.ReadSection("NEW_SECTION", "intVector", vValueInt, vector<int>(3,8), "new int vector");
	//IniFileIO.ReadSection("NEW_SECTION", "floatVector", vValueFloat, vector<float>(3,9.9f), "new float vector");
