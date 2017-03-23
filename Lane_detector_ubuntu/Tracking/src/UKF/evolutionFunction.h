/* ********************************* FILE ************************************/
/** @file		EvolutionFunction.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CEvolutionFunction
 * 
 *	@author		David Vázquez (dvazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _EVOLUTION_FUNCTION_
#define _EVOLUTION_FUNCTION_

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



class EvolutionFunction: public QVFunction<QVVector, QVVector>
{
	public:

		EvolutionFunction(const EStateRepres stateRepres, const int order):
		m_stateRepres(stateRepres), 
		m_order(order)
		{
		}

		// Gets as input the current state and returns the next state
		QVVector evaluate(const QVVector &value);

private:
	EStateRepres	m_stateRepres;
	int				m_order;
};

#endif //_EVOLUTION_FUNCTION_