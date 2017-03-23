/* ********************************* FILE ************************************/
/** @file		Track.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CTrack
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _TRACK_
#define _TRACK_

#if _MSC_VER > 1000
	#pragma once
#endif



/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Homography.h>
#include <procesamientodenivelbajo.h>

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
using namespace cv;
using namespace std;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
enum EStateTrackerType { estt_unknown=-1, estt_init, estt_predicted, estt_updated, estt_updatedAuto, estt_active };
enum EStateRepres { esr_unknown=-1, esr_xy, esr_xyh, esr_XYZ, esr_XYZH, esr_HorizonPitchHeight, esr_Horizon, esr_PitchHeight };

/* ******************************** CLASS ************************************/
/**
 *	@ingroup	Tracking
 *	@brief		It defines the class CTrack
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CTrack
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
    //CTrack	(const unsigned int id, const CRoi& measurement, const EStateRepres stateRepres, const int order);
    CTrack	(const unsigned int id, const vector<float>& measurement, const EStateRepres stateRepres, const int order);
    virtual ~CTrack	();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	virtual void	Initialize			() = 0;
    virtual void    Initialize          (const vector<float>& measurement) = 0;
	virtual void	Finish				() = 0;
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
    virtual void	Compute				(const vector<float>& measurement	)	= 0;
	virtual void	Predict				(							)	= 0;
    virtual void	Update				(const vector<float>& measurement	)	= 0;
	virtual void	Update				(							)	= 0;
	//virtual void	Simulate			(							)	const	= 0;
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	virtual void	ShowDebugInfo		(const char* name			)	const	= 0;
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	virtual void	SetCurrentEvidence	(const bool value				)	{ m_vEvidenceHistory.push_back(value);	}
	void			SetTrackerState		(const EStateTrackerType state	)	{ m_trackerState = state;				}
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	//virtual vector<vector<float> >	GetErrorCovariance	()	const = 0;
	unsigned int		GetTrackerId		()	const						{ return m_trackerId;					}
	EStateTrackerType	GetTrackerState		()	const						{ return m_trackerState;				}
    vector<float>		GetRoadMeasurement	()	const						{ return m_roadMeasurement;				}
    vector<float>		GetRoadState		()	const						{ return m_roadState;					}
    vector<float>		GetRoadPrediction	()	const						{ return m_roadPrediction;				}
	unsigned short		GetAge				()	const						{ return (unsigned short)m_vEvidenceHistory.size();		}
	unsigned char		GetNumSkips			()	const						{ return (unsigned char)m_numSkips;					}
	unsigned int		GetNumMeasurements	()	const						{ return m_numMeasurements;				}
    virtual cv::Mat     GetErrorCovPre      () = 0;
    virtual cv::Mat     GetErrorCovPost     () = 0;
    virtual cv::Mat     GetMeasurementNoiseCov  () = 0;
    virtual cv::Mat     GetProcessNoiseCov  () = 0;
	///@}

protected:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Protected methods
	///@{
	///@}

private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CTrack(const CTrack& other);
	CTrack& operator=(const CTrack& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	///@}


public:	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Parameters from constructor
    ///@{
	unsigned int		m_trackerId;				///< Id number
	EStateRepres		m_stateRepres;				///< Tracking state representation
	int					m_order;					///< Tracking dynamic model order
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Parameters from INI
    ///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Variables
    ///@{
	EStateTrackerType	m_trackerState;				///< State of the tracker
	vector<bool>		m_vEvidenceHistory;			///< Evidence history

    vector<float>		m_roadMeasurement;			///< ROAD measurement
    vector<float>		m_roadState;				///< ROAD state
    vector<float>		m_roadPrediction;			///< ROAD prediction

	unsigned int		m_numSkips;					///< Number of measurements skips
	unsigned int		m_numMeasurements;			///< Number of measurements
	///@}
};
#endif // _TRACK_
