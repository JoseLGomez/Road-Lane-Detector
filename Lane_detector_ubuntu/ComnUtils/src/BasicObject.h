/* ********************************* FILE ************************************/
/** \file        BasicObject.h
 *
 *    \ingroup   ComnUtils
 *
 *    \brief     This file describes the declaration of the class CBasicObject
 *
 *    \author    David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *    \date      Apr 15, 2012
 *    \note      (C) Copyright CVC-UAB, ADAS
 *
 *****************************************************************************/
#ifndef _BASIC_OBJECT_
#define _BASIC_OBJECT_

#if _MSC_VER > 1000
        #pragma once
#endif

/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "ParameterHandler.h"

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
 *    \ingroup    ComnUtils
 *    \brief      It defines the class CBasicObject
 *
 *    \author     David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *    \date       Apr 15, 2012
 *    \sa         -
 *****************************************************************************/
class CBasicObject
{
// METHODS
//------------------------------------------------------------------
public:
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Constructors and destructors
    ///@{
	explicit CBasicObject(const string& name):
		m_name			( name	 ),
		m_init			( false	 ),
		m_enabled		( true	 ),
		m_parameterSet	( m_name )
	{
	}

	~CBasicObject() {}
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Initializes and finalizes
    ///@{
    virtual void Initialize() = 0;
	virtual void Finish() = 0;
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Computational methods
    ///@{
    virtual void Compute() = 0;
    ///@}

     //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Functions:
    ///@{
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Set methods
    ///@{
    virtual void SetData() = 0;
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Get methods
    ///@{
    CParameterSet*	GetParameterSet	()			{ return &m_parameterSet;	}
	const string&	GetName			() const	{ return m_name;			}
	bool			IsInit			() const	{ return m_init;			}
	bool			IsEnabled		() const	{ return m_enabled;			}
    ///@}

//------------------------------------------------------------------
protected:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private constructors
    ///@{
	//CBasicObject(const CBasicObject& other) {}
	//CBasicObject& operator=(const CBasicObject& other) {}
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private methods
    ///@{
    virtual void ReadParameters() = 0;
    ///@}

// ATTRIBUTES
//-----------------------------------------------------------------
protected:
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Parameters from constructor
    ///@{
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Parameters from INI
    ///@{
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name State variables
    ///@{
    string					m_name;					///< Name of the class
    bool					m_init;					///< Flag indicating if it is initialized
	bool					m_enabled;				///< Flag indication if it is enabled
    CParameterSet			m_parameterSet;			///< Parameter set
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Auxiliar variables
    ///@{
    ///@}
};
#endif //_BASIC_OBJECT_