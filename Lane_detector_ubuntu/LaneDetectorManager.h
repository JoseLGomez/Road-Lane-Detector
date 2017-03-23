#ifndef LANEDETECTORMANAGER_H
#define LANEDETECTORMANAGER_H

#include "StraightLaneDetector.h"
#include "CurveLaneDetector.h"

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
class CTrack;

class CLaneDetectorManager
{
public:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Constructors and destructors
    CLaneDetectorManager(const string& parametersFile);
    ~CLaneDetectorManager();
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Initializes and finalizes
    ///@{
    void Initialize		(int height, int width);
    void Finish			();
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Computational methods
    ///@{
    void Compute		(Mat image, bool laneDetect, bool kalman);
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Get methods
    ///@{
    Mat getThresholdResult();
    Mat getCurveThresholdResult();
    Mat getHoughlineResult();
    Mat getImage();
    Mat getCurveImage();
    vector<vector<Point>> getLanesDetected();
    CStraightLaneDetector* getStraightLaneDetector();
    ///@}

private:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private constructors
    ///@{
    CLaneDetectorManager(const CLaneDetectorManager& other);
    CLaneDetectorManager& operator=(const CLaneDetectorManager& other);
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private methods
    ///@{
    void ReadParameters();
    ///@}

// ATRIBUTES
    CStraightLaneDetector*				m_pStraightLaneDetector;			///< Pointer to the Lane Detector algorithm
    CCurveLaneDetector*                 m_pCurveLaneDetector;
    // Parameters from constructor
    string                              m_parametersFile;                   ///< Parameters file

};

#endif // LANEDETECTORMANAGER_H
