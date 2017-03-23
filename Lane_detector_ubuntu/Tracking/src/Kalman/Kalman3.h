/* ********************************* FILE ************************************/
/** @file		Kalman3.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CKalman3
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/	
#ifndef _KALMAN3_
#define _KALMAN3_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "ComnUtils/src/Maths/matrixlapack.h"

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	Tracking
 *	@brief		It defines the class CKalman3
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CKalman3
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CKalman3();
	CKalman3(float a_in, float b_in, float c_in, float maxDistanceThreshold);
	~CKalman3();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize(float a_in, float b_in, float c_in, float maxDistanceThreshold);
	void Finish();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	void Predict();
	void Correct(float a_in, float b_in, float c_in);
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CKalman3(const CKalman3& other);
	CKalman3& operator=(const CKalman3& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	void ProjectState();
	void ProjectErrorCovariance();
	void KalmanGain();
	void UpdateEstimateFromMeasurement();
	void UpdateErrorCovariance();
	///@}

// ATRIBUTES
public:
	float			a;						///<
	float			b;						///<
	float			c;						///<
	float			m_maxDistanceThreshold; ///< Im not sure if this, together with SearchNearestEvidence, should be here

private:
	CMatrix<6,1>	X;						///< State
	CMatrix<6,6>	P;						///< Error Covariance
	CMatrix<3,1>	Z;						///< Position
	CMatrix<6,3>	K;						///< Kalman Gain

	CMatrix<6,6>	A;						///< Relates state from k-1 to k
	CMatrix<3,6>	H;						///< Relates state to measurement
	CMatrix<6,6>	Q;						///< Process noise covariance
	CMatrix<3,3>	R;						///< Measurement Noise Covariance

	CMatrix<6,6>	I;						///< Identity

	CMatrix<3,1>	tmp31_1;				///< 
	CMatrix<3,1>	tmp31_2;				///< 
	CMatrix<3,3>	tmp33_1;				///< 
	CMatrix<3,3>	tmp33_2;				///< 
	CMatrix<3,3>	tmp33_3;				///< 
	CMatrix<3,6>	tmp36_1;				///< 
	CMatrix<6,1>	tmp61_1; 				///< 
	CMatrix<6,1>	tmp61_2; 				///< 
	CMatrix<6,3>	tmp63_1;				///< 
	CMatrix<6,6>	tmp66_1; 				///< 
	CMatrix<6,6>	tmp66_2; 				///< 
	CMatrix<6,6>	tmp66_3;				///< 
};
#endif // _KALMAN3_
