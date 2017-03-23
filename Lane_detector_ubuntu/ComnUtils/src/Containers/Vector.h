/* ********************************* FILE ************************************/
/** @file		Vector.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CVector
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _VECTORS_
#define _VECTORS_

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
 *	@brief		It defines the class CVector3D
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CVector3D
{
public:
	CVector3D() { a=b=c=0; }
	CVector3D(float a_in, float b_in, float c_in) { a=a_in; b=b_in; c=c_in; }
	~CVector3D() {}
	void Set(float a_in, float b_in, float c_in) { a=a_in; b=b_in; c=c_in; }

public:
	float a, b, c;
};



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CVector5D
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CVector5D
{
public:
	CVector5D() { a=b=c=d=e=0; }
	CVector5D(float a_in, float b_in, float c_in, float d_in, float e_in) { a=a_in; b=b_in; c=c_in; d=d_in; e=e_in; }
	~CVector5D() {}
	void Set(float a_in, float b_in, float c_in, float d_in, float e_in) { a=a_in; b=b_in; c=c_in; d=d_in; e=e_in; }

public:
	float a, b, c, d, e;
};

#endif