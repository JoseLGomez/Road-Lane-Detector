/* ********************************* FILE ************************************/
/** @file		IniFileIO.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CIniFileIO
 * 
 *  @author     Antonio M. L�pez (antonio@cvc.uab.es)
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _INI_FILE_READER_
#define _INI_FILE_READER_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "Utils.h"
#include "Color.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::string;
using std::vector;

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/
#ifdef __unix__
#define PATHPREFIX string(getenv("HOME")) + string("/")
#else
#define PATHPREFIX string("d:/")
#endif

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CIniFileIO
 *
 *  @author     Antonio M. L�pez (antonio@cvc.uab.es)
 *	@author		David Geronimo (dgeronimo@cvc.uab.es) (Original by Antonio Lopez)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CIniFileIO
{
// METHODS
public:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CIniFileIO(const int maxVarSize=40, const int maxValSize=60);	
	CIniFileIO(const string& IniFilename, const int maxVarSize=40, const int maxValSize=60);
	virtual ~CIniFileIO();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize(string activeIniFilename);
	void InitializeWrite(string activeIniFilename);
	void InitializeUpdate(string activeIniFilename);
	void Finish();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Read section
	///@{
	void ReadSection(const string& section, const string& variable, string& value, const string& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, unsigned int& value, const unsigned int& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, unsigned char& value, const unsigned char& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, unsigned long& value, const unsigned long& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, bool& value, const bool& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, int& value, const int& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, long& value, const long& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, float& value, const float& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, double& value, const double& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, vector<int>& value, const vector<int>& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, vector<float>& value, const vector<float>& defaultValue, const string& comment) const;
	void ReadSection(const string& section, const string& variable, vector<double>& value, const vector<double>& defaultValue, const string& comment) const;
    void ReadSection(const string& section, const string& variable, vector<vector<int> >& value, const string& comment) const;
    void ReadSection(const string& section, const string& variable, vector<vector<float> >& value, const string& comment) const;
    void ReadSection(const string& section, const string& variable, CColor& value, const CColor& defaultValue, const string& comment) const;
	void ReadLongSection(const string& section, const string& variable, vector<int>& value, const string& comment) const;
	void ReadLongSection(const string& section, const string& variable, vector<float>& value, const string& comment) const;
	
	///@}

 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Write section
	///@{
	void WriteSection(const string& section, const string& variable, const string& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const unsigned int& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const unsigned char& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const unsigned long& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const bool& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const int& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const long& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const float& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const double& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const vector<int>& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const vector<float>& value, const string& comment="") const;
	void WriteSection(const string& section, const string& variable, const vector<double>& value, const string& comment="") const;
    void WriteSection(const string& section, const string& variable, const vector<vector<int> >& value, const string& comment="") const;
    void WriteSection(const string& section, const string& variable, const vector<vector<float> >& value, const string& comment="") const;
    void WriteSection(const string& section, const string& variable, const CColor& value, const string& comment="") const;
	void WriteLongSection(const string& section, const string& variable, const vector<float>& value, const string& comment="") const;
	void WriteLongSection(const string& section, const string& variable, const vector<int>& value, const string& comment="") const;
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	void SetParametersFile(string parametersFile) { m_IniFilename = parametersFile; }
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CIniFileIO(const CIniFileIO& other);
	CIniFileIO& operator=(const CIniFileIO& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	string GetStringValue(const string& section, const string& variable, const string& defaultValue, const string& comment) const;
	void SetStringValue(const string& section, const string& variable, const string& stringValue, const string& comment) const;
	int  GetValueFromValueSequence(string& valueSequence, string& value) const;
	void DeleteCommentsAndTabulatorsAndBlankSpaces(string& value) const;
	///@}

//ATRIBUTES
private:
	// Parameters from constructor
	string			m_IniFilename;			///< Parameters file name
	int				m_maxVarSize;			///< 
	int				m_maxValSize;			///<
	
	// Variables
	char*			m_buffer;				///<
};
#endif // _INI_FILE_READER_
