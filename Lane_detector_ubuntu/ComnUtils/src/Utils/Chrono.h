/* ********************************* FILE ************************************/
/** @file		Chrono.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CChrono
 * 
 *  @author		Antonio M. López (antonio@cvc.uab.es)
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _VSYS_CHRONO_
#define _VSYS_CHRONO_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <stdio.h>
#ifndef __unix__
    #include <windows.h>
#endif


/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CChrono
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es) (from Antonio Lopez ADAS chrono)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/	
class CChrono  
{
// METHODS
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CChrono();
	virtual ~CChrono();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	void StartChrono();
	void TIC();
	void TOC();
	void Tic();
	void Toc();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	static void	ConvertFromSeconds(double totalTime, int& hours, int& minutes, float& seconds);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	double	GetTotalTime		();
	double	GetTimeFromKeypoint	();
	void	GetTotalTime		(int& hours, int& minutes, float& seconds);
	void	GetTimeFromKeypoint	(int& hours, int& minutes, float& seconds);
	void	SetTimeKeypoint		();
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CChrono(const CChrono& other);
	CChrono& operator=(const CChrono& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	///@}


// ATRIBUTES
private:
	// Variables
#ifndef __unix__
	LARGE_INTEGER	m_timerFrequency;	///< 
	LARGE_INTEGER	m_startCounter;		///< 
	LARGE_INTEGER	m_keypoint;			///< 
#else
    long	m_timerFrequency;	///<
    long    m_startCounter;		///<
    long	m_keypoint;			///<
#endif
};
#endif
