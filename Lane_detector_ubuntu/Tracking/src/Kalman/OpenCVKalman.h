/* ********************************* FILE ************************************/
/** @file		TrackKalman.h
 *
 *	@ingroup	Tracking
 *
 *	@brief		This file describes the declaration of the class COpenCVKalman
 *
 *	@author		David V?zquez (dvazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 *
 *****************************************************************************/
#ifndef _TRACK_KALMAN_
#define _TRACK_KALMAN_

#if _MSC_VER > 1000
    #pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "../Track.h"
#include "opencv2/opencv.hpp"

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
 *	@brief		It defines the class COpenCVKalman
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class COpenCVKalman: public CTrack
{
// METHODS
public:
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Constructors and destructors
    ///@{
    COpenCVKalman(const unsigned int id, const vector<float>& measurement, const EStateRepres stateRepres=esr_HorizonPitchHeight, const int order=1, const float measurementNoise=0.1f, const float processNoise=0.01f, const float postError=0.1f);
    ~COpenCVKalman();
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Initializes and finalizes
    ///@{
    virtual void		Initialize		();
    void                Initialize      (const vector<float>& measurement);
    virtual void		Finish			();
    ///@}


    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Computational methods
    ///@{
    virtual void		Compute			(const vector<float>& measurement	);
    virtual void		Predict			(							);
    virtual void		Update			(const vector<float>& measurement	);
    virtual void		Update			(							);
    //virtual void		Simulate		(							)	const;
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Functions:
    ///@{
    virtual void		ShowDebugInfo	(const char* name			)	const;
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Set methods
    ///@{
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Get methods
    ///@{
    cv::Mat GetErrorCovPre();
    cv::Mat GetErrorCovPost();
    cv::Mat GetMeasurementNoiseCov();
    cv::Mat GetProcessNoiseCov();
    ///@}

private:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private constructors
    ///@{
    COpenCVKalman(const COpenCVKalman& other);
    COpenCVKalman& operator=(const COpenCVKalman& other);
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private methods
    ///@{
    void RoadToMat(const vector<float>& road, cv::Mat mat) const;
    void MatToRoad(const cv::Mat mat, vector<float>& road) const;
    ///@}


// ATRIBUTES
private:
    int					m_dynamParams;			///< Size of the dynamic matrix
    int					m_measureParams;		///< Size of the measurement vector
    int					m_controlParams;		///< Size of the control parameters vector

    float				m_measurementNoise;		///< Noise at the measurement process. We assume it to equal in all dimensions and no correlated.
    float				m_processNoise;			///< Noise of the dynamic process. We assume it to equal in all dimensions and no correlated.
    float				m_postError;			///< N...

    cv::KalmanFilter	m_kf;					///< OpenCV kalman filter
    cv::Mat				m_prediction;			///< OpenCV prediction
    cv::Mat				m_measurement;			///< OpenCV measurent
    cv::Mat				m_state;				///< OpenCV state
    cv::Mat				m_invCov;				///< OpenCV inverse of the covariance matrix
};
#endif // _TRACK_KALMAN_
