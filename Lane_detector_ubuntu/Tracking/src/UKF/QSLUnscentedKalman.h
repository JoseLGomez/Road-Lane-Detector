/* ********************************* FILE ************************************/
/** @file		TrackKalman.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CQSLUnscentedKalman
 * 
 *	@author		David Vázquez (dvazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _TRACK_QSL_UNSCENTED_KALMAN_
#define _TRACK_QSL_UNSCENTED_KALMAN_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "../Track.h"
#include "qvmatrixalgebra.h"
#include "qvukf.h"
#include "evolutionFunction.h"
#include "observationFunction.h"


/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
class QVVector;
class QVMatrix;
class QVUKF;

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
 *	@brief		It defines the class CQSLUnscentedKalman
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CQSLUnscentedKalman: public CTrack
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CQSLUnscentedKalman(const unsigned int id, const CRoi& measurement, const EStateRepres stateRepres, const int order, const float measurementNoise, const float processNoise);
	~CQSLUnscentedKalman();
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	virtual void		Initialize		();
	virtual void		Finish			();
	///@}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	virtual void		Compute			(const CRoi& measurement	);
	virtual void		Predict			(							);
	virtual void		Update			(const CRoi& measurement	);
	virtual void		Update			(							);
	//virtual void		Simulate		(							)	const;
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	virtual float		DistanceToRoi	(const CRoi& roi			)	const;
	virtual void		ShowDebugInfo	(const char* name			)	const;
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
    CQSLUnscentedKalman(const CQSLUnscentedKalman& other);
    CQSLUnscentedKalman& operator=(const CQSLUnscentedKalman& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	void RoiToVect(const CRoi& roi, QVVector& mat) const;
	void VectToRoi(const QVVector& mat, CRoi& roi) const;
	///@}


// ATRIBUTES
private:
	int					m_dynamParams;			///< Size of the dynamic matrix
	int					m_measureParams;		///< Size of the measurement vector
	
	float				m_measurementNoise;		///< Noise at the measurement process. We assume it to equal in all dimensions and no correlated.
	float				m_processNoise;			///< Noise of the dynamic process. We assume it to equal in all dimensions and no correlated.

	QVUKF*				m_ukf_filter;			///< Unscendent Kalman Filter
	EvolutionFunction	m_evolutionFunction;	///< Evolution function
	ObservationFunction	m_observationFunction;	///< Observation funtion
	QVMatrix			m_R_t;					///< QVMatrix Noise
	QVMatrix			m_Q_t;					///< QVMatrix Noise
	QVVector			m_prediction;			///< QVMatrix prediction
	QVVector			m_measurement;			///< QVMatrix measurent
	QVVector			m_state;				///< QVMatrix state
};
#endif // _TRACK_QSL_UNSCENTED_KALMAN_
