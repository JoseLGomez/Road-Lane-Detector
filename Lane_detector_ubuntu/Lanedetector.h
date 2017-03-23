#ifndef LANEDETECTOR_H
#define LANEDETECTOR_H

#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Homography.h>
#include <procesamientodenivelbajo.h>
#include "ComnUtils/src/Utils/IniFileIO.h"

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

using namespace cv;
using namespace std;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/// Global Variables
//const float factor = 2;
//const int houghThreshold = 90/factor;
//const int houghRho = 1;
const float ADD90_RAD = 1.5708;

class CLaneDetector
{
public:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Constructors and destructors
    CLaneDetector();
    virtual ~CLaneDetector();
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Initializes and finalizes
    ///@{
    virtual void Initialize();
    virtual void Finish();
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Computational methods
    ///@{
    virtual void Compute();
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Functions:
    ///@{
    virtual void LaneTypeDetection();
    void PrintImage(Mat src, String nombre);
    void PrintMat(Mat src, String name);
    void PrintLinesOnimage(vector<Vec3f> lines, Scalar colour);
    void PrintLinesOnimage(Vec3f lineInput, Scalar colour);
    Mat CV8Uconvertor(Mat input);
    vector<unsigned char> ConvertMatToVector(Mat image);
    Mat ConvertVectorToMat(float * image, int rows, int cols);
    vector<Point> PolarToCartesian(Vec3f input);
    int PolarToCartesian(Vec3f input, int y);
    int PolarToCartesian(vector<float> input, int y);

    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Get methods
    ///@{
    Mat getThresholdResult();
    Mat getHoughlineResult();
    Mat getImage();
    vector<vector<Point>> getLanesDetected();
    ///@}

private:

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private constructors
    ///@{
    CLaneDetector(const CLaneDetector& other);
    CLaneDetector& operator=(const CLaneDetector& other);
    ///@}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ///@name Private methods
    ///@{
    void ReadParameters();
    ///@}
protected:
    ///@name Protected methods
    ///@{
    void InitializeHomography();
    void CalculateRoadWidthMean();
    void CalculateVanishingPointMean();
    Mat CalculateVanishingPoint(Vec3f right, Vec3f left);
    bool VanishingPointCompararison(Vec3f line);
    bool LaneCompararison(Vec3f left, Vec3f right);
    int DoubleLaneCompararison(Vec3f leftHough, Vec3f leftKalman, Vec3f rightHough, Vec3f rightKalman);
    int CalculateFitWithRoadWidth(Vec3f left, Vec3f right);
    float CalculateFitWithVanishingPoint(Vec3f left, Vec3f right);
    float CalculateFitWithVanishingPoint(Vec3f line);
    ///@}

    // Parameters from constructor
    string                              m_parametersFile;                   ///< Parameters file

    // Lane detector results
    Mat                         thresholdResult;
    Mat                         houghlineResult;
    Mat                         image;

    // Parameters
    int                         rows;
    int                         cols;
    Vec3f                       leftCandidateSelected;
    Vec3f                       rightCandidateSelected;
    int                         lastRightDistanceToCenter;
    int                         lastLeftDistanceToCenter;
    int                         rightDisplacement;
    int                         leftDisplacement;
    bool                        rightLaneChange;
    bool                        leftLaneChange;
    vector<vector<Point>>       lanesDetected;
    bool                        rightLaneIsContinuous = true;
    bool                        leftLaneIsContinuous = true;
    vector<int>                 leftLaneHistorial;
    vector<int>                 rightLaneHistorial;
    int                         imageCounter = 0;
    int                         heightCut;
    int                         lineHeight;
    float                       meanRoadWidth;
    int                         roadWidthMeasurements;
    int                         vanishingPointMeasurements;
    Mat                         vanishingPointmean;
    bool                        roadWidthMeanActive;
    bool                        vanishingPointmeanActive;
    int                         noLinesDetectedCounter;
    int                         bothLinesDetectedCounter;
    vector<Mat>                 vanishingPointsLearned;
    vector<int>                 roadLineSizesLearned;
    int                         widthTolerance;
    int                         vanishingPointTolerance;
    float                       factor;
    float                       cutHeightFactor;


    //Clases
    Homography                  homography;
    CProcesamientoDeNivelBajo   ridge;

// ATRIBUTES

};

#endif // LANEDETECTOR_H
