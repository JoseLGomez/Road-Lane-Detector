/* ********************************* FILE ************************************/
/** @file		UniqueIdentifier.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CUniqueIdentifier
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _UNIQUEIDENTIFIER_
#define _UNIQUEIDENTIFIER_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/

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
 *	@brief		It defines the class CUniqueIdentifier: implements a singleton.
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CUniqueIdentifier
{
// METHODS
public:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	static CUniqueIdentifier* Instance();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	void SetID		(int value) { id = value;  };
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	long GetNewID	()			{ return id++; }
	///@}
	
protected:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Protected constructors
	///@{
	CUniqueIdentifier(long v=0) { id=v; }
	///@}

private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	///@}

// ATRIBUTES
private:
	static CUniqueIdentifier*	pinstance;	///< 
	long						id;			///< 
};
#endif // _UNIQUEIDENTIFIER_
