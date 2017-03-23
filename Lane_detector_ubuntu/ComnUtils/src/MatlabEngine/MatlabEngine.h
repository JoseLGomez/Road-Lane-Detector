/* ********************************* FILE ************************************/
/** @file		MatlabEngine.h
*
*	@ingroup	ComnUtils
* 
*	@brief		Call matlab software from framework
*				Reference: edit([matlabroot '/extern/examples/eng_mat/engdemo.c']);
* 
*	@author		David Geronimo (dgeronimo@cvc.uab.es)
*	@author		David Vazquez (David.Vazquez@cvc.uab.es)
*	@author		Jiaolong Xu (jiaolong@cvc.uab.es)
*
*	@date		May 4, 2012
*	@note		(C) Copyright CVC-UAB, ADAS
* 
*****************************************************************************/
#ifndef _MATLABENGINE_
#define _MATLABENGINE_


#if _MSC_VER > 1000
#pragma once
#endif

/*****************************************************************************
* INCLUDE FILES
*****************************************************************************/
#include "../Utils/Utils.h"
#define NOT_USEMATLAB
#ifndef NOT_USEMATLAB
#include "engine.h"
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
*	@brief		It defines the class CMatlabEngine: call matlab from framework.
*
*	@author		David Geronimo (dgeronimo@cvc.uab.es)
*	@author		David Vazquez (David.Vazquez@cvc.uab.es)
*
*	@date		May 4, 2012
*	\sa			-
*****************************************************************************/
class CMatlabEngine
{
	// METHODS
public:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
    CMatlabEngine();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
    ~CMatlabEngine();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	void Initialize();
	void ExecuteCommand(const string strCmd);
	void RunMFile(const string path, const string fileName);
	void Finish();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	///@}

protected:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Protected constructors
	///@{
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
#ifndef NOT_USEMATLAB
	Engine *ep;
#endif
};
#endif // _MATLABENGINE_
