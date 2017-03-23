#include <time.h>
#include "StraightLaneDetector.h"
#include "Tracking/src/Track.h"
#include "Tracking/src/Kalman/OpenCVKalman.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 *****************************************************************************/

CStraightLaneDetector::CStraightLaneDetector(const string& parametersFile):
    m_parametersFile(parametersFile)
{
    ReadParameters();
}

/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

CStraightLaneDetector::~CStraightLaneDetector()
{
}

///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Initializes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param   image   image input in Mat format
 *****************************************************************************/
void CStraightLaneDetector::Initialize(int height,int width)
{
    //clock_t tStart = clock();
    heightCut = height*cutHeightFactor;
    rows = (height-heightCut)/factor;
    cols = width/factor;
    lineHeight = height*0.06/factor;
    //Initialize start parameters
    InitializeHomography();    //Initialize object that calculates homography
    ridge.Initialize(cols,rows);
    leftCandidateSelected = Vec3f(0,0,0);
    rightCandidateSelected = Vec3f(0,0,0);
    lastLeftDistanceToCenter = 0;
    lastRightDistanceToCenter = 0;
    rightDisplacement = 0;
    leftDisplacement = 0;
    ///Kalman tracking initialization
    unsigned int id = 0;
    int order=1;
    float measurementNoise=5.0f;
    float processNoise=0.01f;
    float postError=0.001f;
    m_stateRepres = esr_PitchHeight;

    //Left side
    leftMeasurement.resize(2);
    leftMeasurement[0] = 246;
    leftMeasurement[1] = 0.715585f;

    m_pTrackingLeftLane = new COpenCVKalman(id,leftMeasurement , m_stateRepres, order,
                                            measurementNoise, processNoise, postError);
    //Right side
    rightMeasurement.resize(2);
    rightMeasurement[0] = -194;
    rightMeasurement[1] = 2.19911f;

    m_pTrackingRightLane = new COpenCVKalman(id,rightMeasurement , m_stateRepres, order,
                                             measurementNoise, processNoise, postError);

    leftKalmanOff = true;
    rightKalmanOff = true;

    meanRoadWidth = 0;
    roadWidthMeasurements = 0;
    vanishingPointMeasurements = 0;
    vanishingPointmean = (Mat_<float>(1,3) << 0, 0, 0);
    roadWidthMeanActive = false;
    vanishingPointmeanActive = false;
    noLinesDetectedCounter = 0;
    bothLinesDetectedCounter = 0;
    firstRightDetection = true;
    firstLeftDetection = true;
    rightLaneChange = false;
    leftLaneChange = false;
    /*vanishingPointsLearned = {(Mat_<float>(1,3) << 640/factor, 266/factor - heightCut/factor, 1)
                              ,(Mat_<float>(1,3) << 662/factor, 270/factor - heightCut/factor, 1)
                             ,(Mat_<float>(1,3) << 640/factor, 240/factor - heightCut/factor, 1)};*/
    vanishingPointsLearned = {(Mat_<float>(1,3) << 490/factor, 285/factor - heightCut/factor, 1),
                             (Mat_<float>(1,3) << 434/factor, 200/factor - heightCut/factor, 1)};
    //roadLineSizesLearned = {(int)(506/factor),(int)(710/factor),(int)(600/factor),(int)(850/factor)};
    roadLineSizesLearned = {(int)(650/factor),(int)(800/factor),(int)(1000/factor)};
    //printf("Time initialize: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

/**************************************************************************//**
 * @brief	Reads the parameters.
 *
 * @author	Jose Luis Gomez
 * @date	May 11, 2016
 *****************************************************************************/
void CStraightLaneDetector::ReadParameters()
{

    CIniFileIO IniFileIO(m_parametersFile);
    IniFileIO.ReadSection("LANE_DETECTION", "Factor", factor, 2.0, "Scale Factor to resize image input [Default 2.0: 1/2 original size");
    IniFileIO.ReadSection("LANE_DETECTION", "HoughThreshold", houghThreshold, 90, "Threshold value to detect lines in Hough space [Default 90] recomended values [0 - 150] ");
    IniFileIO.ReadSection("LANE_DETECTION", "HoughRho", houghRho, 1, "The resolution of the parameter r (distance to center in polar coordinates) in pixels [Default 1 pixel]");
    IniFileIO.ReadSection("LANE_DETECTION", "minCandidateAngleLeft", minCandidateAngleLeft, 0.2, "Angle in radians, minimum value of Rho in Hough space (polar coordinates) to filter Left Lines [Recomenden Values: 0 - Pi/2 and lower than maxCandidateAngleLeft] ");
    IniFileIO.ReadSection("LANE_DETECTION", "maxCandidateAngleLeft", maxCandidateAngleLeft, 1.1, "Angle in radians, maximum value of Rho in Hough space (polar coordinates) to filter Left Lines [Recomenden Values: 0 - Pi/2 and bigger than minCandidateAngleLeft] ");
    IniFileIO.ReadSection("LANE_DETECTION", "minCandidateAngleRight", minCandidateAngleRight, 2.0, "Angle in radians, minimum value of Rho in Hough space (polar coordinates) to filter Right Lines [Recomenden Values: Pi/2 - Pi and lower than maxCandidateAngleRight] ");
    IniFileIO.ReadSection("LANE_DETECTION", "maxCandidateAngleRight", maxCandidateAngleRight, 2.95, "Angle in radians, maximum value of Rho in Hough space (polar coordinates) to filter Right Lines [Recomenden Values: Pi/2 - Pi and bigger than minCandidateAngleRight] ");
    IniFileIO.ReadSection("LANE_DETECTION", "candidateQuantity", candidateQuantity, 8, "Maximum number of lines selected in Hough Space from right and left lines [Default: 8]");
    IniFileIO.ReadSection("LANE_DETECTION", "kalmanFitTolerance", kalmanFitTolerance, 50, "Maximum tolerance for r in Hough Space to determine if a line fit with the kalman predicted [Default: 50]");
    IniFileIO.ReadSection("LANE_DETECTION", "kalmanFitRhoTolerance", kalmanFitRhoTolerance, 0.065, "Maximum tolerance for Rho in Hough Space to determine if a line fit with the kalman predicted [Default: 0.065]");
    IniFileIO.ReadSection("LANE_DETECTION", "widthTolerance", widthTolerance, 50, "Maximum width tolerable in pixels to accept a Line [Default: 30 pixels");
    IniFileIO.ReadSection("LANE_DETECTION", "vanishingPointTolerance", vanishingPointTolerance, 100, "Maximum Euclidean distance to tolerate a new vanishing point determinated by new lines [Default: 50]");
    IniFileIO.ReadSection("LANE_DETECTION", "kalmanNumFramesDeactivation", kalmanNumFramesDeactivation, 10, "Number of frames without any detection until kalman tracking is deactivated [Default: 10 frames]");
    IniFileIO.ReadSection("LANE_DETECTION", "kalmanNumFramesActivation", kalmanNumFramesActivation, 5, "Number of frames with detection until kalman tracking is activated [Default: 5 frames]");
    IniFileIO.ReadSection("LANE_DETECTION", "meansNumFrameDeactivation", meansNumFrameDeactivation, 10, "Number of frames without any detection until Vanishing point and width means tracking is deactivated [Default: 10 frames]");
    IniFileIO.ReadSection("LANE_DETECTION", "meansNumFrameActivation", meansNumFrameActivation, 5, "Number of frames with detection until Vanishing point and width means tracking is Activated [Default: 5 frames]");
    IniFileIO.ReadSection("LANE_DETECTION", "cutHeightFactor", cutHeightFactor, 0.4, "% of height cutted from the top of the image input that represent background [Default: 0.4 (40% image height)]");

    kalmanFitTolerance = kalmanFitTolerance/factor;
    houghThreshold =  houghThreshold/factor;
    widthTolerance = widthTolerance/factor;
    vanishingPointTolerance = vanishingPointTolerance/factor;

}

/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
void CStraightLaneDetector::Finish()
{
    CLaneDetector::Finish();
}

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Computes.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param	Image		  	Mat format image.
 *****************************************************************************/

void CStraightLaneDetector::Compute(Mat image, bool laneDetect, bool kalman)
{
    imageCounter++;
    // ------------------------------------------------
    ///Image resize
    //clock_t tStart = clock();
    CStraightLaneDetector::image = image.clone();
    image = Mat(image, Rect(0,heightCut,image.size().width,
                            image.size().height-heightCut)).clone();

    resize(image,image,Size(0,0),1.0/factor,1.0/factor,INTER_NEAREST);
    //printf("Time resize: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    // ------------------------------------------------
    ///Convert Mat image to a vector
    //tStart = clock();
    vector<unsigned char> vectorizedImage;
    vectorizedImage = ConvertMatToVector(image);
    // ------------------------------------------------
    ///Calculate ridgeness of the image
    unsigned char  *  imageMem = &vectorizedImage[0];
    float  *  result;
    result = ridge.Process(imageMem);
    // ------------------------------------------------
    ///recover ridgeness calculation and put into a Mat image
    thresholdResult = ConvertVectorToMat(result, rows, cols);
    cvtColor(CStraightLaneDetector::image, CStraightLaneDetector::image, CV_GRAY2BGR);
    //printf("Time ridge: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    // ------------------------------------------------
    /// Calculate and select lines on the ridgeness image
    //tStart = clock();
    HoughLinesOnPerspectiveDetection();
    LaneChange();
    //printf("Time Hough: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    // ------------------------------------------------
    ///Apply Kalman filter for search a candidate
    if (kalman)
        ApplyKalman();
    // ------------------------------------------------
    /// Calculate Lane type if it required
    if (laneDetect)
        LaneTypeDetection();
    // ------------------------------------------------
    /// Print lines selected to the image input
    Scalar colourRight;
    Scalar colourLeft;

    if (rightLaneIsContinuous)
        colourRight = Scalar(0,0,255); //red continuous
    else
        colourRight = Scalar(0,255,0); //green discontinuous
    if (leftLaneIsContinuous)
        colourLeft = Scalar(0,0,255); //red continuous
    else
        colourLeft = Scalar(0,255,0); //green discontinuous

    if (rightCandidateSelected != Vec3f(0,0,0))
        PrintLinesOnimage(rightCandidateSelected,colourRight);
    if (leftCandidateSelected != Vec3f(0,0,0))
        PrintLinesOnimage(leftCandidateSelected,colourLeft);

    Scalar colourArrow = Scalar(255,0,0);
    if (leftLaneChange)
    {
        Point pt1 = Point(CStraightLaneDetector::image.cols/2+30,CStraightLaneDetector::image.rows/4);
        Point pt2 = Point(CStraightLaneDetector::image.cols/2-30,CStraightLaneDetector::image.rows/4);
        arrowedLine(CStraightLaneDetector::image, pt1, pt2, colourArrow, 2, 8, 0, 0.5);
    }
    if (rightLaneChange)
    {
        Point pt1 = Point(CStraightLaneDetector::image.cols/2-30,CStraightLaneDetector::image.rows/4);
        Point pt2 = Point(CStraightLaneDetector::image.cols/2+30,CStraightLaneDetector::image.rows/4);
        arrowedLine(CStraightLaneDetector::image, pt1, pt2, colourArrow, 2, 8, 0, 0.5);
    }
}


/**************************************************************************//**
 * @brief	This function compute the possible lines of an image making the hough
 *          line detection and select the two lines candidates that represent
 *          the road lanes.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

void CStraightLaneDetector::HoughLinesOnPerspectiveDetection()
{
    vector<Vec3f> leftLinesCandidate;
    vector<Vec3f> rightLinesCandidate;
    vector<Vec2f> lines;
    Vec3f auxiliar;
    Point linePoint;
    float theta;
    float rho;

    // ------------------------------------------------
    /*///Candidate angles in radians
    float minCandidateAngleLeft = 0.2; //theta 15 -> angle = 90 - theta = 75
    float maxCandidateAngleLeft = 1.1; //theta 45 -> angle = 90 - theta = 45
    float minCandidateAngleRight = 2; //theta 180 - 126 = 54 -> angle = 90 - theta = 36
    float maxCandidateAngleRight = 2.95; //theta 180 - 165 = 15 -> angle = 75*/
    // ------------------------------------------------
    //unsigned int candidateQuantity = 8; //Number of maximum candidates searched
    int rows = thresholdResult.size().height;
    int cols = thresholdResult.size().width;
    houghlineResult = CV8Uconvertor(thresholdResult);
    // ------------------------------------------------
    ///Calculate hough
    HoughLines(houghlineResult, lines, houghRho, (CV_PI/180)*1, houghThreshold , 0, 0 );
    // ------------------------------------------------
    ///Candidate clasification
    unsigned int pos = 0;
    while (!(leftLinesCandidate.size() == candidateQuantity && rightLinesCandidate.size() == candidateQuantity) && pos < lines.size())
    {
        // ------------------------------------------------
        /// Calculate X and Y coordinates from the Polar result given by hough
        rho = lines[pos][0];
        theta = lines[pos][1];
        auxiliar[0] = rho;
        auxiliar[1] = theta;
        auxiliar[2] = pos;
        linePoint.y = rows;
        double a = cos(theta), b = sin(theta);
        linePoint.x = cvRound(-(b*linePoint.y/a)+(rho/a));
        // ------------------------------------------------
        /// Separation between lines that start at the first half of the image and at the second half
        //left side
        if (linePoint.x < cols/2 )
        {
            /// Selection of candidate lane that accomplish the angle requisites
            if (leftLinesCandidate.size() < candidateQuantity && theta < maxCandidateAngleLeft && theta > minCandidateAngleLeft )
            {
                leftLinesCandidate.push_back(auxiliar);
            }
        } else {
        //right side
            if (rightLinesCandidate.size() < candidateQuantity && theta > minCandidateAngleRight && theta < maxCandidateAngleRight)
            {
                rightLinesCandidate.push_back(auxiliar);
            }
        }
        pos++;
    }
    // ------------------------------------------------
    ///Candidate selection after the division between righ and left
    // Left
    if (leftLinesCandidate.size() > 0)
    {
        leftCandidateSelected = SelectCandidate(leftLinesCandidate,2);
        //PrintLinesOnimage(rightLinesCandidate,Scalar(0,255,255));
    } else {
        leftCandidateSelected = Vec3f(0,0,0);
    }
    // Right
    if (rightLinesCandidate.size() > 0)
    {
        rightCandidateSelected = SelectCandidate(rightLinesCandidate,1);
        //PrintLinesOnimage(leftLinesCandidate,Scalar(0,255,255));
    } else {
        rightCandidateSelected = Vec3f(0,0,0);
    }
    // ------------------------------------------------
    /// Store results
    lanesDetected.clear();
    lanesDetected.push_back(PolarToCartesian(rightCandidateSelected));
    lanesDetected.push_back(PolarToCartesian(leftCandidateSelected));
    /*//Print candidates selected by Hough in green
    if (rightCandidateSelected != Vec3f(0,0,0))
        PrintLinesOnimage(rightCandidateSelected,Scalar(0,255,0));
    if (leftCandidateSelected != Vec3f(0,0,0))
        PrintLinesOnimage(leftCandidateSelected,Scalar(0,255,0));*/
}

/**************************************************************************//**
 * @brief	This function compute the possible lines of an image making the hough
 *          line detection and select the two lines candidates that represent
 *          the road lanes.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param   lines       Vector of float vector with 3 positions that represents
 *                      a line in polar coordinate.
 * @param   option      numeric value for choose between left and right candidate
 *                      ad no candidate lines
 *****************************************************************************/

Vec3f CStraightLaneDetector::SelectCandidate(vector<Vec3f> lines, int option)
{
    Point linePt1;
    Point linePt2;
    Vec3f bestCandidate;
    int bestCandidateX;
    int difRho;
    int maxDif = 0;
    int MaxX = 0;
    int MinX = 0;
    int teoricOptim = 0;
    int bestCandidateP2X = 0;
    int centerDistance = 0;
    float theta;
    float rho;
    bool firstChoose = true;

    switch (option) {
    case 1:
        //right candidate
        maxDif = 20/factor;
        MaxX = cols+200;
        MinX = (cols/2)+centerDistance/factor;
        teoricOptim = MinX;
        break;
    case 2:
        //left candidate
        maxDif = 20/factor;
        MaxX = (cols/2)-centerDistance/factor;
        MinX = -200;
        teoricOptim = MaxX;
        break;
    }

    for( size_t i = 0; i < lines.size(); i++ )
    {
        rho = lines[i][0];
        theta = lines[i][1];
        linePt1.y = 0;
        linePt2.y = rows;
        double a = cos(theta), b = sin(theta);
        linePt1.x = cvRound(-(b*linePt1.y/a)+(rho/a));
        linePt2.x = cvRound(-(b*linePt2.y/a)+(rho/a));
        if ((option == 1 && linePt2.x > MinX && linePt2.x < MaxX)
                || (option == 2 && linePt2.x < MaxX && linePt2.x > MinX))
        {
            if (firstChoose)
            {
                bestCandidate = lines[0];
                bestCandidateX = abs(linePt2.x-teoricOptim);
                bestCandidateP2X = linePt2.x;
                firstChoose = false;
            } else {
                int aux = abs(linePt2.x-teoricOptim);
                if (bestCandidateX > aux)
                {
                    bestCandidate = lines[i];
                    bestCandidateP2X = linePt2.x;
                    bestCandidateX = aux;
                }
            }
        }
    }

    for( size_t i = 0; i < lines.size(); i++ )
    {
        rho = lines[i][0];
        theta = lines[i][1];
        linePt2.y = rows;
        double a = cos(theta), b = sin(theta);
        linePt2.x = cvRound(-(b*linePt2.y/a)+(rho/a));
        difRho = abs(linePt2.x - bestCandidateP2X);
        if (difRho < maxDif+1)
        {
             return lines[i];
        }
    }
    return bestCandidate;
}

/**************************************************************************//**
 * @brief	Apply a Kalman filter for make a better estimation of what line are
 *          a better candidate and suppress noise.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

void CStraightLaneDetector::ApplyKalman()
{
    int houghCase;
    int rightKalmanCase;
    int leftKalmanCase;
    int kalmanFitCase;
    int codeCases;
    int finalselectionCase;
    Vec3f rightKalman;
    Vec3f leftKalman;

    rightMeasurement = m_pTrackingRightLane->GetRoadPrediction();
    rightKalman = Vec3f(rightMeasurement[0],rightMeasurement[1],1);
    leftMeasurement = m_pTrackingLeftLane->GetRoadPrediction();
    leftKalman = Vec3f(leftMeasurement[0],leftMeasurement[1],1);

    /*//Print Kalman lines in blue
    Vec3f aux = Vec3f(0,0,0);
    aux[0] = rightMeasurement[0];
    aux[1] = rightMeasurement[1];

    if (aux != Vec3f(0,0,0) && !rightKalmanOff)
        PrintLinesOnimage(aux,Scalar(255,0,0));
    aux[0] = leftMeasurement[0];
    aux[1] = leftMeasurement[1];
    if (aux != Vec3f(0,0,0) && !leftKalmanOff)
        PrintLinesOnimage(aux,Scalar(255,0,0));*/

    if (rightKalmanOff || rightLaneChange || leftLaneChange)
        rightKalmanCase = 1; //right Kalman deactivated
    else
        rightKalmanCase = 2; //right Kalman activated

    if (leftKalmanOff || rightLaneChange || leftLaneChange)
        leftKalmanCase = 1; //left Kalman deactivated
    else
        leftKalmanCase = 2; //left Kalman activated

    if (rightCandidateSelected != Vec3f(0,0,0))
    {
        if (leftCandidateSelected != Vec3f(0,0,0))
        {
            //Both lines detected
            houghCase = 1;
        } else {
            //Right line detected
            houghCase = 3;
        }
    } else {
        if (leftCandidateSelected != Vec3f(0,0,0))
        {
            //Left line detected
            houghCase = 2;
        } else {
            //No lines detected
            houghCase = 4;
        }
    }
    //create codification number in decimal base with the 3 variables
    codeCases = leftKalmanCase*100 + rightKalmanCase*10 + houghCase;
    //Switch that calculate what case of kalman fit is
    switch (codeCases)
    {
    case 121: case 123: case 223:
        //Cases only right Kalman can fit
        kalmanFitCase = CalculateKalmanFit(false, true);
        break;
    case 211: case 212: case 222:
        //Cases only left Kalman can fit
        kalmanFitCase = CalculateKalmanFit(true, false);
        break;
    case 221:
        //Case both Kalman can fit
        kalmanFitCase = CalculateKalmanFit(true, true);
        break;
    case 111:
        if (CalculateFitWithRoadWidth(leftCandidateSelected,rightCandidateSelected) < 25
                && CalculateFitWithVanishingPoint(leftCandidateSelected,rightCandidateSelected) < 50)
            finalselectionCase = 11;
        else if (CalculateFitWithVanishingPoint(leftCandidateSelected) < 50)
            finalselectionCase = 13;
        else if (CalculateFitWithVanishingPoint(rightCandidateSelected) < 50)
            finalselectionCase = 31;
        else
            finalselectionCase = 33;
        kalmanFitCase = 0;
        break;
    case 112:
        if (CalculateFitWithVanishingPoint(leftCandidateSelected) < 75)
            finalselectionCase = 13;
        else
            finalselectionCase = 33;
        kalmanFitCase = 0;
        break;
    case 113:
        if (CalculateFitWithVanishingPoint(rightCandidateSelected) < 75)
            finalselectionCase = 31;
        else
            finalselectionCase = 33;
        kalmanFitCase = 0;
        break;
    case 114:
        finalselectionCase = 33;
        kalmanFitCase = 0;
        break;
    default:
        //Cases that can't fit with kalman
        kalmanFitCase = 4;
        break;
    }
    //Add kalman fit case to codification number
    codeCases = codeCases*10 + kalmanFitCase;
    //Switch that calculates what parameters and what comparison function will select
    switch (codeCases)
    {
    case 2124:
        //Only left kalman active, left line detected, kalman don't fit
        if (VanishingPointCompararison(leftCandidateSelected))
        {
            finalselectionCase = 13;
        } else {
            finalselectionCase = 23;
        }
        break;
    case 1234:
        //Only right kalman active, right line detected, kalman don't fit
        if (VanishingPointCompararison(rightCandidateSelected))
        {
            finalselectionCase = 31;
        } else {
            finalselectionCase = 32;
        }
        break;
    case 1213:
        //Only right Kalman active, both lines detected, right kalman fit on line
        if (LaneCompararison(leftKalman, rightCandidateSelected))
        {
            finalselectionCase = 11;
        } else {
            finalselectionCase = 31;
        }
        break;
    case 1224:
        //Only right Kalman active, left line detected, kalman don't fit
        if (LaneCompararison(leftCandidateSelected,rightKalman))
        {
            finalselectionCase = 12;
        } else {
            finalselectionCase = 32;
        }
        break;
    case 2112:
        //Only left Kalman active, both lines detected, left kalman fit
        if (LaneCompararison(leftCandidateSelected,rightCandidateSelected))
        {
            finalselectionCase = 11;
        } else {
            finalselectionCase = 13;
        }
        break;
    case 2134:
        //Only left Kalman active, right line detected, kalman don't fit
        if (LaneCompararison(leftKalman,rightCandidateSelected))
        {
            finalselectionCase = 21;
        } else {
            finalselectionCase = 23;
        }
        break;
    case 2212:
        //Both kalman active, both lines detected, left kalman fit
        if (LaneCompararison(leftCandidateSelected,rightCandidateSelected))
        {
            finalselectionCase = 11;
        } else {
            finalselectionCase = 12;
        }
        break;
    case 2213:
        //Both kalman active, both lines detected, right kalman fit
        if (LaneCompararison(leftCandidateSelected,rightCandidateSelected))
        {
            finalselectionCase = 11;
        } else {
            finalselectionCase = 21;
        }
        break;
    case 2224:
        //Both kalman active, left line detected, kalman don't fit
        if (LaneCompararison(leftCandidateSelected,rightKalman))
        {
            finalselectionCase = 12;
        } else {
            finalselectionCase = 22;
        }
        break;
    case 2234:
        //Both kalman active, right line detected, kalman don't fit
        if (LaneCompararison(leftKalman,rightCandidateSelected))
        {
            finalselectionCase = 21;
        } else {
            finalselectionCase = 22;
        }
        break;
    case 1214:
        finalselectionCase = DoubleLaneCompararison(leftCandidateSelected,leftKalman
                                                    ,rightCandidateSelected,Vec3f(0,0,0));
        break;
    case 2114:
        finalselectionCase = DoubleLaneCompararison(leftCandidateSelected,Vec3f(0,0,0)
                                                    ,rightCandidateSelected,rightKalman);
        break;
    case 2214:
        finalselectionCase = DoubleLaneCompararison(leftCandidateSelected,leftKalman
                                                    ,rightCandidateSelected,rightKalman);
        break;
    case 1233:
        finalselectionCase = 31;
        break;
    case 1244:
        finalselectionCase = 32;
        break;
    case 2122:
        finalselectionCase = 13;
        break;
    case 2144:
        finalselectionCase = 23;
        break;
    case 2211:
        finalselectionCase = 11;
        break;
    case 2222:
        finalselectionCase = 12;
        break;
    case 2233:
        finalselectionCase = 21;
        break;
    case 2244:
        finalselectionCase = 22;
        break;
    }

    int leftcode = finalselectionCase/10;
    int rightcode = finalselectionCase%10;

    if (rightcode == 1)
    {
        ///Right side
        rightLaneIsContinuous = LineAnalisys(rightCandidateSelected[0], rightCandidateSelected[1]);
        if (rightLaneIsContinuous)
            rightLaneHistorial.push_back(1);
        else
            rightLaneHistorial.push_back(0);
        if (rightLaneHistorial.size() > 10)
            rightLaneHistorial.erase(rightLaneHistorial.begin());
        if  (!leftLaneChange && !rightLaneChange)
        {
            //Right candidate fit with kalman
            rightMeasurement[0] = rightCandidateSelected[0];
            rightMeasurement[1] = rightCandidateSelected[1];

            if (firstRightDetection)
            {
                firstRightDetection = false;
                m_pTrackingRightLane->Initialize(rightMeasurement);
            } else if (!(rightMeasurement[0] == 0 && rightMeasurement[1] == 0))
                m_pTrackingRightLane->Compute(rightMeasurement);
            //Right line detected, kalman activation counter
            rightKalmanOnCounter++;
            rightKalmanOffCounter = 0;
        } else {
            rightMeasurement[0] = rightCandidateSelected[0];
            rightMeasurement[1] = rightCandidateSelected[1];
            m_pTrackingRightLane->Initialize(rightMeasurement);
        }
    } else if ( rightcode == 2)
    {
        rightCandidateSelected[0] = rightMeasurement[0];
        rightCandidateSelected[1] = rightMeasurement[1];
        //Right line not detected, kalman desactivation counter
        rightKalmanOffCounter++;
    } else {
        rightCandidateSelected = Vec3f(0,0,0);
    }
    if (leftcode == 1)
    {
        if (!leftLaneChange && !rightLaneChange)
        {
            ///Left side
            leftLaneIsContinuous = LineAnalisys(leftCandidateSelected[0],leftCandidateSelected[1]);
            if (leftLaneIsContinuous)
                leftLaneHistorial.push_back(1);
            else
                leftLaneHistorial.push_back(0);
            if (leftLaneHistorial.size() > 10)
                leftLaneHistorial.erase(leftLaneHistorial.begin());

            leftMeasurement[0] = leftCandidateSelected[0];
            leftMeasurement[1] = leftCandidateSelected[1];
            if (firstLeftDetection)
            {
                firstLeftDetection = false;
                m_pTrackingLeftLane->Initialize(leftMeasurement);
            } else if (!(leftMeasurement[0] == 0 && leftMeasurement[1] == 0))
                m_pTrackingLeftLane->Compute(leftMeasurement);
            //left line detected, kalman activation counter
            leftKalmanOnCounter++;
            leftKalmanOffCounter = 0;
        } else {
            leftMeasurement[0] = leftCandidateSelected[0];
            leftMeasurement[1] = leftCandidateSelected[1];
            m_pTrackingLeftLane->Initialize(leftMeasurement);
        }
    } else if ( leftcode == 2)
    {
        leftCandidateSelected[0] = leftMeasurement[0];
        leftCandidateSelected[1] = leftMeasurement[1];
        //Left line not detected, kalman desactivation counter
        leftKalmanOffCounter++;
    } else {
        leftCandidateSelected = Vec3f(0,0,0);
    }
    if (finalselectionCase == 11)
    {
        bothLinesDetectedCounter++;
        CalculateRoadWidthMean();
        CalculateVanishingPointMean();
    } else if (finalselectionCase == 33)
    {
        // No lines detected
        noLinesDetectedCounter++;
        //Left line not detected, kalman desactivation counter
        leftKalmanOffCounter++;
        //Right line not detected, kalman desactivation counter
        rightKalmanOffCounter++;  
    } else {
        noLinesDetectedCounter++;
    }

    // Check counters for active or desactivate Kalman filters, Road width mean and
    // Vanishing point mean
    if (leftKalmanOffCounter > kalmanNumFramesDeactivation - 1)
    {
        leftKalmanOff = true;
        leftKalmanOnCounter = 0;
        firstLeftDetection = true;
    } else if (leftKalmanOnCounter > kalmanNumFramesActivation - 1)
    {
        leftKalmanOff = false;
        leftKalmanOnCounter = 0;
    }
    if (rightKalmanOffCounter > kalmanNumFramesDeactivation - 1)
    {
        rightKalmanOff = true;
        rightKalmanOnCounter = 0;
        firstRightDetection = true;
    } else if (rightKalmanOnCounter > kalmanNumFramesActivation -1)
    {
        rightKalmanOff = false;
        rightKalmanOnCounter = 0;
    }

    if (noLinesDetectedCounter > meansNumFrameDeactivation)
    {
        //Deactivate means and reinitialize to 0
        noLinesDetectedCounter = 0;
        bothLinesDetectedCounter = 0;
        roadWidthMeanActive = false;
        vanishingPointmeanActive = false;
        //if (noLinesDetectedCounter > meansNumFrameDeactivation*2)
        //{
            //noLinesDetectedCounter = 0;
            roadWidthMeasurements = 0;
            vanishingPointMeasurements = 0;
            meanRoadWidth = 0;
            vanishingPointmean = (Mat_<float>(1,3) << 0, 0, 0);
        //}
    } else if (bothLinesDetectedCounter > meansNumFrameActivation)
    {
        noLinesDetectedCounter = 0;
        bothLinesDetectedCounter = 0;
        roadWidthMeanActive = true;
        vanishingPointmeanActive = true;
        leftLaneChange = false;
        rightLaneChange = false;
    }

    /*Mat errorCovPre = m_pTrackingRightLane->GetErrorCovPre();
    PrintMat(errorCovPre,"errorCovPre");
    Mat errorCovPost = m_pTrackingRightLane->GetErrorCovPost();
    PrintMat(errorCovPost,"errorCovPost");
    Mat measurementNoiseCov = m_pTrackingRightLane->GetMeasurementNoiseCov();
    PrintMat(measurementNoiseCov,"measurementNoiseCov");
    Mat processNoiseCov = m_pTrackingRightLane->GetProcessNoiseCov();
    PrintMat(processNoiseCov,"processNoiseCov");*/
}

/**************************************************************************//**
 * @brief	Calculate if Kalman lines prediction fit with the actual Lines detected
 *          by Hough and return a code of the case. 1: both lines fits with kalman,
 *          2: Left line fit with Kalman, 3: right lane fit with Kalman, 4: Any line
 *          fit with Kalman.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

int CStraightLaneDetector::CalculateKalmanFit(bool left, bool right)
{

    int outputCode;
    if (left && abs(leftMeasurement[0] - leftCandidateSelected[0]) < kalmanFitTolerance
             && abs(leftMeasurement[1] - leftCandidateSelected[1]) < kalmanFitRhoTolerance)
    {
        //Left line detected fit with left kalman
        outputCode = 1;
    } else {
        //Left line detected DON'T fit with left kalman
        outputCode = 2;
    }
    if (right && abs(rightMeasurement[0] - rightCandidateSelected[0]) < kalmanFitTolerance
              && abs(rightMeasurement[1] - rightCandidateSelected[1]) < kalmanFitRhoTolerance)
    {
        //Right line detected fit with left kalman
        if (outputCode > 1)
            outputCode = 3;
    } else {
        //Right line detected DON'T fit with left kalman
        if (outputCode > 1)
            outputCode = 4;
        else
            outputCode = 2;
    }
    //outputCode: 1: both fits with kalman, 2: Left fit, 3: right fit, 4: no fit
    return outputCode;
}

/**************************************************************************//**
 * @brief	Detect the type of the line: continuous or discontinuous from the
 *          threshold image
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

void CStraightLaneDetector::LaneTypeDetection()
{
    int auxcounter = 0;
    ///Right side
    if (rightLaneHistorial.size() > 0)
    {
        for ( int i = 0; i < rightLaneHistorial.size(); i++)
        {
            auxcounter = auxcounter + rightLaneHistorial[i];
        }
        if ( auxcounter > rightLaneHistorial.size()/2)
            rightLaneIsContinuous = true;
        else
            rightLaneIsContinuous = false;
    }
    ///Left side
    auxcounter = 0;
    if (leftLaneHistorial.size() > 0)
    {
        for ( int i = 0; i < leftLaneHistorial.size() +1; i++)
        {
            auxcounter = auxcounter + leftLaneHistorial[i];
        }
        if ( auxcounter > leftLaneHistorial.size()/2)
            leftLaneIsContinuous = true;
        else
            leftLaneIsContinuous = false;
    }
}

/**************************************************************************//**
 * @brief	Analize the line input on the ridgeness image for parametrize
 *          continuity or discontinuity
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

bool CStraightLaneDetector::LineAnalisys(float rho, float theta)
{
    vector<int> lanesegments;
    int x, y;
    int lineCounter = 0;
    int noLineCounter = 0;
    int totalPoints = 0;
    double a = cos(theta), b = sin(theta);
    for (int i = 0; i < rows; i++)
    {
        y = i;
        x = cvRound(-(b*y/a)+(rho/a));
        int aux = houghlineResult.at<uchar>(y,x);
        if (aux > 0 || houghlineResult.at<uchar>(y,x-1) > 0
                || houghlineResult.at<uchar>(y,x+1) > 0
                || houghlineResult.at<uchar>(y,x-2) > 0
                || houghlineResult.at<uchar>(y,x+2) > 0
                || houghlineResult.at<uchar>(y,x+3) > 0
                || houghlineResult.at<uchar>(y,x-3) > 0
                || houghlineResult.at<uchar>(y+1,x-1) > 0
                || houghlineResult.at<uchar>(y+1,x+1) > 0
                || houghlineResult.at<uchar>(y+1,x-2) > 0
                || houghlineResult.at<uchar>(y+1,x+2) > 0
                || houghlineResult.at<uchar>(y+1,x+3) > 0
                || houghlineResult.at<uchar>(y+1,x-3) > 0)
        {
            lineCounter++;
            totalPoints++;
            noLineCounter = 0;
        } else if (lineCounter > 5) {
            noLineCounter++;
            lanesegments.push_back(lineCounter);
            lineCounter = 0;
        } else {
            noLineCounter++;
            if (noLineCounter > 7) {
                lineCounter = 0;
            }
        }
    }
    if (totalPoints > 0.6*rows) {
        return true;
    } else if (lanesegments.size() > 0) {
        return false;
    } else {
        return true;
    }
}

/**************************************************************************//**
 * @brief	Analize if a new dection is moving to the left or right for detect
 *          if there is a left or right lane change
 *
 * @author	Jose Luis Gomez
 * @date	May 20, 2016
 *****************************************************************************/
/*void CStraightLaneDetector::LaneChange()
{
    int rightX;
    int leftX;
    int rightDistanceToCenter;
    int leftDistanceToCenter;
    bool leftLine = false;
    bool rightLine = false;
    if (leftCandidateSelected != Vec3f(0,0,0))
    {
        leftX = PolarToCartesian(leftCandidateSelected, rows);
        leftDistanceToCenter = (cols/2) - leftX;
        leftLine = true;
    }
    if (rightCandidateSelected != Vec3f(0,0,0))
    {
        rightX = PolarToCartesian(rightCandidateSelected, rows);
        rightDistanceToCenter = rightX - (cols/2);
        rightLine = true;
    }
    if (lastLeftDistanceToCenter != 0 && lastRightDistanceToCenter != 0)
    {
        if ((leftLine && leftDistanceToCenter < lastLeftDistanceToCenter) ||
                (rightLine && rightDistanceToCenter > lastRightDistanceToCenter))
        {
            leftDisplacement++;
            rightDisplacement = 0;
        } else if ((leftLine && leftDistanceToCenter > lastLeftDistanceToCenter) ||
                 (rightLine && rightDistanceToCenter < lastRightDistanceToCenter))
        {
            rightDisplacement++;
            leftDisplacement = 0;
        }
    }
    if (rightLine)
        lastRightDistanceToCenter = rightDistanceToCenter;
    if (leftLine)
        lastLeftDistanceToCenter = leftDistanceToCenter;

    if (leftDisplacement > 6)
    {
        leftLaneChange = true;
        rightLaneChange = false;
        leftDisplacement = 0;
    } else if (rightDisplacement > 6)
    {
        rightLaneChange = true;
        leftLaneChange = false;
        rightDisplacement = 0;
    }
}*/

void CStraightLaneDetector::LaneChange()
{
    int rightX;
    int leftX;
    int rightDistanceToCenter;
    int leftDistanceToCenter;
    bool leftLine = false;
    bool rightLine = false;
    if (leftCandidateSelected != Vec3f(0,0,0))
    {
        leftX = PolarToCartesian(leftCandidateSelected, rows);
        leftDistanceToCenter = (cols/2) - leftX;
        leftLine = true;
    }
    if (rightCandidateSelected != Vec3f(0,0,0))
    {
        rightX = PolarToCartesian(rightCandidateSelected, rows);
        rightDistanceToCenter = rightX - (cols/2);
        rightLine = true;
    }
    if (lastLeftDistanceToCenter != 0 && lastRightDistanceToCenter != 0)
    {
        if ((leftLine && leftDistanceToCenter < lastLeftDistanceToCenter) ||
                (rightLine && rightDistanceToCenter > lastRightDistanceToCenter))
        {
            leftDisplacement = leftDisplacement + (lastLeftDistanceToCenter - leftDistanceToCenter);
            rightDisplacement = rightDisplacement + (rightDistanceToCenter - lastRightDistanceToCenter);
        } else if ((leftLine && leftDistanceToCenter > lastLeftDistanceToCenter) ||
                 (rightLine && rightDistanceToCenter < lastRightDistanceToCenter))
        {
            rightDisplacement = rightDisplacement + (lastRightDistanceToCenter - rightDistanceToCenter);
            leftDisplacement = leftDisplacement + (leftDistanceToCenter - lastLeftDistanceToCenter);
        }
    }
    if (rightLine)
        lastRightDistanceToCenter = rightDistanceToCenter;
    if (leftLine)
        lastLeftDistanceToCenter = leftDistanceToCenter;

    if (leftDisplacement > 6)
    {
        leftLaneChange = true;
        rightLaneChange = false;
        rightDisplacement = 0;
        leftDisplacement = 0;
    } else if (rightDisplacement > 6)
    {
        rightLaneChange = true;
        leftLaneChange = false;
        rightDisplacement = 0;
        leftDisplacement = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////


