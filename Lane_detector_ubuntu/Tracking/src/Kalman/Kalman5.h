/* ********************************* FILE ************************************/
/** @file		Kalman5.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CKalman5
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _KALMAN5_
#define _KALMAN5_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "Maths/matrixlapack.h"

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
 *	@brief		It defines the class CKalman5
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CKalman5
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CKalman5();
	CKalman5(float a_in, float b_in, float c_in, float d_in, float e_in, float maxDistanceThreshold);
	~CKalman5();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize(float a_in, float b_in, float c_in, float d_in, float e_in, float maxDistanceThreshold);
	void Finish();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	void Predict();
	void Correct(float a_in, float b_in, float c_in, float d_in, float e_in);
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
	CKalman5(const CKalman5& other);
	CKalman5& operator=(const CKalman5& other);
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
	float				a;						///<
	float				b;						///<
	float				c;						///<
	float				d;						///<
	float				e;						///<
	float				m_maxDistanceThreshold; ///< Im not sure if this, together with SearchNearestEvidence, should be here

private:
	CMatrix<10, 1>		X;						///< State
	CMatrix<10,10>		P;						///< Error Covariance
	CMatrix< 5, 1>		Z;						///< Position
	CMatrix<10, 5>		K;						///< Kalman Gain

	CMatrix<10,10>		A;						///< Relates state from k-1 to k
	CMatrix< 5,10>		H;						///< Relates state to measurement
	CMatrix<10,10>		Q;						///< Process noise covariance
	CMatrix< 5, 5>		R;						///< Measurement Noise Covariance

	CMatrix<10,10>		I;						///< Identity

	CMatrix< 5, 1>		tmp51_1;				///< 
	CMatrix< 5, 1>		tmp51_2;				///< 
	CMatrix< 5, 5>		tmp55_1;				///< 
	CMatrix< 5, 5>		tmp55_2;				///< 
	CMatrix< 5, 5>		tmp55_3;				///< 
	CMatrix< 5,10>		tmp510_1;				///< 
	CMatrix<10, 1>		tmp101_1; 				///< 
	CMatrix<10, 1>		tmp101_2; 				///< 
	CMatrix<10, 5>		tmp105_1;				///< 
	CMatrix<10,10>		tmp1010_1;				///< 
	CMatrix<10,10>		tmp1010_2;				///< 
	CMatrix<10,10>		tmp1010_3;				///< 
};

#endif // _KALMAN5_
