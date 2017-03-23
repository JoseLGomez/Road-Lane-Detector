#include "LaneDetectorManager.h"
#include "opencv2/opencv.hpp"


using namespace cv;


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

CLaneDetectorManager::CLaneDetectorManager(const string& parametersFile):
    m_parametersFile(parametersFile)
{
    m_pStraightLaneDetector = new CStraightLaneDetector(parametersFile);
    m_pCurveLaneDetector = new CCurveLaneDetector(parametersFile);
}

/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
CLaneDetectorManager::~CLaneDetectorManager()
{
    Finish();
}

///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Initializes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
void CLaneDetectorManager::Initialize(int height, int width)
{
    m_pStraightLaneDetector->Initialize(height,width);
    m_pCurveLaneDetector->Initialize(height,width);
}

/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
void CLaneDetectorManager::Finish()
{
    if (m_pStraightLaneDetector!= NULL)
        m_pStraightLaneDetector->Finish();
    if (m_pStraightLaneDetector != NULL)
        delete m_pStraightLaneDetector;
    if (m_pCurveLaneDetector!= NULL)
        m_pCurveLaneDetector->Finish();
    if (m_pCurveLaneDetector != NULL)
        delete m_pCurveLaneDetector;
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
void CLaneDetectorManager::Compute(Mat image, bool laneDetect, bool kalman)
{
    m_pStraightLaneDetector->Compute(image,laneDetect,kalman);
    m_pCurveLaneDetector->Compute(image);
}

///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////

Mat CLaneDetectorManager::getThresholdResult()
{
    return m_pStraightLaneDetector->getThresholdResult();
}
Mat CLaneDetectorManager::getCurveThresholdResult()
{
    return m_pCurveLaneDetector->getThresholdResult();
}
Mat CLaneDetectorManager::getHoughlineResult()
{
    return m_pStraightLaneDetector->getHoughlineResult();
}
Mat CLaneDetectorManager::getImage()
{
    return m_pStraightLaneDetector->getImage();
}
Mat CLaneDetectorManager::getCurveImage()
{
    return m_pCurveLaneDetector->getImage();
}
vector<vector<Point>> CLaneDetectorManager::getLanesDetected()
{
    return m_pStraightLaneDetector->getLanesDetected();
}
CStraightLaneDetector* CLaneDetectorManager::getStraightLaneDetector()
{
    return m_pStraightLaneDetector;
}
