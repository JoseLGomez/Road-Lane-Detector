/* ********************************* FILE ************************************/
/** @file		StraightLaneDetector.h
 *
 *	@ingroup	LaneDetector
 *
 *	@brief		This file describes the declaration of the class CStraightLaneDetector
 *
 *  @author     Jose Luis Gomez
 *  @date       Apr 4, 2016
 *	@note		(C) Copyright CVC-UAB, ADAS
 *
 *****************************************************************************/

#ifndef STRAIGHT_LANEDETECTOR_H
#define STRAIGHT_LANEDETECTOR_H

/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/

#include <Lanedetector.h>
#include "Tracking/src/Track.h"


/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

using namespace cv;
using namespace std;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/


/* ******************************** CLASS ************************************/
/**
 *	@ingroup	LaneDetector
 *	@brief		It defines the class CLaneDetector
 *
 *  @author     Jose Luis Gomez
 *  @date       Apr 4, 2016
 *	\sa			-
 *****************************************************************************/

class CStraightLaneDetector : public CLaneDetector
{
// METHODS
public:
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Constructors and destructors
    ///@{
    CStraightLaneDetector(const string& parametersFile);
    ~CStraightLaneDetector();
    ///@}
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Initializes and finalizes
    ///@{
    void Initialize(int height, int width);
    void Finish();
    ///@}
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Computational methods
    ///@{
    void Compute(Mat image, bool laneDetect, bool kalman);
    ///@}
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Functions:
    ///@{
    void LaneTypeDetection();
    ///@}
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Get methods
    ///@{
    ///@}

private:
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private constructors
    ///@{
    CStraightLaneDetector(const CStraightLaneDetector& other);
    CStraightLaneDetector& operator=(const CStraightLaneDetector& other);
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private methods
    ///@{
    void ReadParameters();
    void HomographyHoughLinesDetection(Mat input);
    void HoughLinesOnPerspectiveDetection();
    Vec3f SelectCandidate(vector<Vec3f> lines, int option);
    bool LineAnalisys(float rho, float theta);
    void ApplyKalman();
    int CalculateKalmanFit(bool left, bool right);
    void LaneChange();
    ///@}

// ATRIBUTES
    // Parameters from constructor
    string                      m_parametersFile;                   ///< Parameters file
    //Road Tracking
    CTrack                      *m_pTrackingLeftLane;
    CTrack                      *m_pTrackingRightLane;
    EStateRepres                m_stateRepres;
    vector<float>               leftMeasurement;
    vector<float>               rightMeasurement;
    int                         KalmanStarterCounter = 0;
    int                         leftKalmanOnCounter = 0;
    int                         leftKalmanOffCounter = 0;
    int                         rightKalmanOnCounter = 0;
    int                         rightKalmanOffCounter = 0;
    bool                        leftKalmanOff;
    bool                        rightKalmanOff;

    int                         houghThreshold;
    int                         houghRho;
    float                       minCandidateAngleLeft;
    float                       maxCandidateAngleLeft;
    float                       minCandidateAngleRight;
    float                       maxCandidateAngleRight;
    uint                        candidateQuantity;
    int                         kalmanFitTolerance;
    float                       kalmanFitRhoTolerance;
    int                         kalmanNumFramesDeactivation;
    int                         kalmanNumFramesActivation;
    int                         meansNumFrameDeactivation;
    int                         meansNumFrameActivation;
    bool                        firstRightDetection;
    bool                        firstLeftDetection;
};

#endif // STRAIGHT_LANEDETECTOR_H
