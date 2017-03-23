/* ********************************* FILE ************************************/
/** @file		ObservationFunction.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CObservationFunction
 * 
 *	@author		David Vázquez (dvazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _OBSERVATION_FUNCTION_
#define _OBSERVATION_FUNCTION_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "../Track.h"
#include "qvmatrixalgebra.h"
#include "qvukf.h"


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

class ObservationFunction: public QVFunction<QVVector, QVVector>
{
	private:
		//Define here whatever you need
		QVVector m_observation;

	public:
		ObservationFunction()
		{
		}

		void setObservation(const QVVector &value)	{ m_observation = value; }

		// Get the state as value and returns the distance from the state to the observation
		QVVector evaluate(const QVVector &value)
		{
			// 
			//QVVector minivalue = value.
			return QVVector(1, (m_observation - value).norm2());
		}
};

#endif //_OBSERVATION_FUNCTION_