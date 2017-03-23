/* ********************************* FILE ************************************/
/** @file		CurveLaneDetector.h
 *
 *	@ingroup	LaneDetector
 *
 *	@brief		This file describes the declaration of the class CCurveLaneDetector
 *
 *  @author     Jose Luis Gomez
 *  @date       Jun 1, 2016
 *	@note		(C) Copyright CVC-UAB, ADAS
 *
 *****************************************************************************/

#ifndef CURVELANEDETECTOR_H
#define CURVELANEDETECTOR_H

/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/

#include <Lanedetector.h>


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

class CCurveLaneDetector : public CLaneDetector
{
// METHODS
public:
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Constructors and destructors
    ///@{
    CCurveLaneDetector(const string& parametersFile);
    ~CCurveLaneDetector();
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
    void Compute(Mat image);
    ///@}
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Functions:
    ///@{
    //void LaneTypeDetection();
    ///@}
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Get methods
    ///@{
    ///@}

private:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private constructors
    ///@{
    CCurveLaneDetector(const CCurveLaneDetector& other);
    CCurveLaneDetector& operator=(const CCurveLaneDetector& other);
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private methods
    ///@{
    void ReadParameters();
    ///@}
// ATRIBUTES
    // Parameters from constructor
    string                      m_parametersFile;                   ///< Parameters file
    //Camera values
    float                       focalLenghtV;
    float                       focalLenghtH;
    float                       cameraHeight;
    float                       pitch;
    //Internal values
    float                       yaw;
    float                       C;
    float                       x;
    float                       L;
};

#endif // CURVELANEDETECTOR_H
