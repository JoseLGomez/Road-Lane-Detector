/* ********************************* MODULE **********************************/
/** @defgroup	Tracking Tracking
 *
 *	@ingroup	CoreLayer
 *				
 *	@brief		This is the module Tracking
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/


/* ********************************* FILE ************************************/
/** @file		TrackingManager.h
 *
 *	@ingroup	Tracking
 * 
 *	@brief		This file describes the declaration of the class CTrackingManager
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _TRACKING_MANAGER_
#define _TRACKING_MANAGER_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <list>
#include <string>
#include <vector>
#include "Track.h"
//#include "Refinement.h"
using std::list;
using std::string;
using std::vector;

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
class CRoi;
class CTrack;
class CMonoCamera;
class CMatrix2D;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
enum ERoiTrackingAlgorithm	{ erta_none = -1, erta_alphabeta, erta_kalman, erta_unscented_kalman };
enum ERoiDistanceType		{ erdt_unknown = -1, erdt_absDist, erdt_relDist, erdt_overlapping };


/* ******************************** CLASS ************************************/
/**
 *	@ingroup	Tracking
 *	@brief		It defines the class CTrackingManager
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CTrackingManager
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	explicit CTrackingManager	(const string& parametersFile);
	~CTrackingManager			();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize				();
	void Finish					();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	void Compute				(const vector<CRoi*>& vRois, vector<CRoi*>& vTracks, CMonoCamera& camera, float speed=1, float yawRate=0, float cycleTime=1);
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
	void GetTracks();
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CTrackingManager(const CTrackingManager& other);
	CTrackingManager& operator=(const CTrackingManager& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	void					ReadParameters			();
	void					SetAlgorithm			(string& algorithm);
	void					SetStateRepres			(string& algorithm);
	void					SetDistanceMeasure		(string& distanceMeasure);

	void					SetInertialSensorData	(const CMonoCamera& camera, const float speed, const float yawRate, const float cycleTime);
	void					PredictTrackers			();
	//CMatrix2D				ComputeDistanceMatrix2	(const vector<CRoi*>& vRois) const;
	vector<vector<float> >	ComputeDistanceMatrix	(const vector<CRoi*>& vRois) const;
	//vector<vector<float> >	ComputeDataAssociation2	(const CMatrix2D& distanceMatrix) const;
	vector<vector<float> >	ComputeDataAssociation	(const vector<vector<float> >& distanceMatrix) const;
	vector<bool>			UpdateTrackers			(const vector<vector<float> >& vAssociations, const vector<CRoi*>& vRois);
	void					SkipUpdates				();
	void					CreateNewTrackers		(const vector<CRoi*>& vRois, vector<bool> vAssignedRois);
	void					SetTrackedRois			(vector<CRoi*>& vTracks) const;
	
	//void					SearchForTrack			(CRoi* roi, list<CTrack*>& vTracks, vector<int>& vTrackMatches);
	CTrack*					CreateNewTrack			(CRoi& roi);
	float					Distance				(CRoi& roi, CRoi& track, CTrack& tracker) const;

	///@}

private:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Parameters from constructor
    ///@{
	string					m_parametersFile;					///< Parameters file
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Parameters from INI
    ///@{
	string					m_algorithmName;					///< Tracking algorithm name
	ERoiTrackingAlgorithm	m_algorithmType;					///< Tracking algorithm type
	EStateRepres			m_stateRepres;						///< Tracking state representation
	int						m_order;							///< Tracking dynamic model order
	int						m_minNumMeassurement;				///< Minimum number of meassurements of a tracker. Until this minimum is not reached it is not considered ad a real tracked object
	int						m_maxNumSkips;						///< Maximum number of updates of a tracker without a measurement. If there are more, the tracker is destroied
	
	float					m_maxDistance;						///< Maximum mahalanobis distance
	ERefDistMeasure			m_overlappingType;					///< Type of ROI overlapping
	ERoiDistanceType		m_distanceType;						///< Type of ROI distance

	float					m_maxSearchOverlapping;				///< Maximum search range in the distance of overlapping of the image
	float					m_maxSearchX;						///< Maximum search range in pixel for the x-direction of the image
	float					m_maxSearchY;						///< Maximum search range in pixel for the y-direction of the image
	float					m_maxSearchH;						///< Maximum search range in pixel for the box height of the image

	float					m_maxSearchXworld;					///< Maximum search range in pixel for the x-direction of the world
	float					m_maxSearchYworld;					///< Maximum search range in pixel for the y-direction of the world
	float					m_maxSearchZworld;					///< Maximum search range in pixel for the z-direction of the world
	float					m_maxSearchHworld;					///< Maximum search range in pixel for the box height of the world

	float					m_KalmanMeasurementNoise;			///< Kalman Measurement Noise
	float					m_KalmanProcessNoise;				///< Kalman Process Noise
	float					m_KalmanPostError;					///< Kalman Post Error
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Variables
    ///@{
	vector<CTrack*>			m_vTracks;							///< Vector of trackers
	const CMonoCamera*		m_pCamera;							///< Pointer to the camera
	float					m_speed;							///< Vehicle current speed
	float					m_yawRate;							///< Vehicle current yawRate
	float					m_cycleTime;						///< Current cycle time (Elapsed time since previous frame)
	unsigned int			m_id;								///< "Singleton" id assignation
	///@}
};

#endif // _TRACKING_MANAGER_
