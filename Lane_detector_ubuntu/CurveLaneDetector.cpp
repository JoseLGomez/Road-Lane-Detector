#include <time.h>
#include "CurveLaneDetector.h"

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

CCurveLaneDetector::CCurveLaneDetector(const string& parametersFile):
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

CCurveLaneDetector::~CCurveLaneDetector()
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
void CCurveLaneDetector::Initialize(int height, int width)
{
    heightCut = height*cutHeightFactor;
    rows = (height-heightCut)/factor;
    cols = width/factor;
    lineHeight = height*0.06/factor;
    //Initialize start parameters
    ridge.Initialize(cols,rows);
    leftCandidateSelected = Vec3f(0,0,0);
    rightCandidateSelected = Vec3f(0,0,0);
}

/**************************************************************************//**
 * @brief	Reads the parameters.
 *
 * @author	Jose Luis Gomez
 * @date	May 11, 2016
 *****************************************************************************/
void CCurveLaneDetector::ReadParameters()
{

    CIniFileIO IniFileIO(m_parametersFile);
    IniFileIO.ReadSection("LANE_DETECTION", "Factor", factor, 2.0, "Scale Factor to resize image input [Default 2.0: 1/2 original size");
    IniFileIO.ReadSection("LANE_DETECTION", "widthTolerance", widthTolerance, 50, "Maximum width tolerable in pixels to accept a Line [Default: 30 pixels");
    IniFileIO.ReadSection("LANE_DETECTION", "vanishingPointTolerance", vanishingPointTolerance, 100, "Maximum Euclidean distance to tolerate a new vanishing point determinated by new lines [Default: 50]");
    IniFileIO.ReadSection("LANE_DETECTION", "cutHeightFactor", cutHeightFactor, 0.4, "% of height cutted from the top of the image input that represent background [Default: 0.4 (40% image height)]");
    IniFileIO.ReadSection("LANE_DETECTION", "focalLenghtV", focalLenghtV, 779.884594, " Focal lenght along vertical axis of the camera in pixels [Default: 779.884594]");
    IniFileIO.ReadSection("LANE_DETECTION", "focalLenghtH", focalLenghtH, 779.884594, " Focal lenght along hortizontal axis of the camera in pixels [Default: 779.884594]");
    IniFileIO.ReadSection("LANE_DETECTION", "cameraHeight", cameraHeight, 1.5, "Camera height from the ground [Default: 1.5]");
    IniFileIO.ReadSection("LANE_DETECTION", "pitch", pitch, 2.0, "Angle with the road plane on the Y axis (pitch angle) [Default: 2.0 (40% image height)]");

    widthTolerance = widthTolerance/factor;
    vanishingPointTolerance = vanishingPointTolerance/factor;
}

/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
void CCurveLaneDetector::Finish()
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

void CCurveLaneDetector::Compute(Mat image)
{
    imageCounter++;
    // ------------------------------------------------
    ///Image resize
    //clock_t tStart = clock();
    CCurveLaneDetector::image = image.clone();
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
    cvtColor(CCurveLaneDetector::image, CCurveLaneDetector::image, CV_GRAY2BGR);
    //printf("Time ridge: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    // ------------------------------------------------
    /// Calculate and select lines on the ridgeness image
}
