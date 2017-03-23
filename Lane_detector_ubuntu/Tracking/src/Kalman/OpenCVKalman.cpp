#include "OpenCVKalman.h"

using namespace std;
#include <iostream>
using namespace cv;


template<class T> void PrintMatrix(const cv::Mat& m, const char* name=NULL)
{
    if (name) cout << "[" << name << "] = " << endl;
    for(int r=0; r<m.rows; r++)
    {
        for(int c=0; c<m.cols; c++) cout << "\t" << m.at<T>(r, c);
        cout << endl;
    }
    cout << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	numEvidences			Number of evidences.
 * @param	maxDistanceThreshold	The maximum distance threshold.
 *****************************************************************************/
COpenCVKalman::COpenCVKalman(const unsigned int id, const vector<float>& measurement, const EStateRepres stateRepres, const int order, const float measurementNoise, const float processNoise, const float postError):
    CTrack(id, measurement, stateRepres, order),
    m_measurementNoise(measurementNoise),
    m_processNoise(processNoise),
    m_postError(postError)
{

    switch(m_stateRepres)
    {
        case esr_HorizonPitchHeight:
            m_measureParams	= 3;
        break;
        case esr_Horizon:
            m_measureParams	= 1;
        break;
        case esr_PitchHeight:
            m_measureParams	= 2;
        break;
        default:
            exit(0);
    };
    m_dynamParams		= m_measureParams*(m_order+1);
    m_controlParams		= 0;



    Initialize();
}

/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
COpenCVKalman::~COpenCVKalman()
{
	Finish();
}



///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////

//template<class T> void PrintMatrix(Mat m, char* name=NULL)
//{
//	if (name) cout << "[" << name << "] = " << endl;
//	for(int r=0; r<m.rows; r++)
//	{
//		for(int c=0; c<m.cols; c++) cout << "\t" << m.at<T>(r, c);
//		cout << endl;
//	}
//	cout << endl;
//}

void COpenCVKalman::Initialize()
{
	// Set in the history as it has a measurement
	m_vEvidenceHistory.push_back(true);

	// Set the state as predicted
	SetTrackerState(estt_init);

	// Intialize the kalman filter sizes
	m_kf.init(m_dynamParams, m_measureParams, m_controlParams, CV_32F);

	// Set the transition matrix
	if (m_order==0)
	{
		setIdentity(m_kf.transitionMatrix);
	}
	else if (m_order==1)
	{
		switch(m_stateRepres)
		{
			case esr_xy:
				// x y Vx Vy
				m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
			break;
			case esr_xyh:
				// x y h Vx Vy Vh
				//m_kf.transitionMatrix = *(Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
				m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,0,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1); // Vh=0
			break;
			case esr_XYZ:
				// X Y Z Vx Vy Vz
				m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
			break;
			case esr_XYZH:
				// X Y Z H Vx Vy Vz Vh
				//m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
			break;
            case esr_HorizonPitchHeight:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
            break;
            case esr_Horizon:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,  0,1);
            break;
            case esr_PitchHeight:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,1,0,   0,1,0,1,  0,0,1,0, 0,0,0,1);
            break;
			default:
				exit(0);
		};
	}
	else if (m_order==2)
	{
				switch(m_stateRepres)
		{
			case esr_xy:
				// x y Vx Vy ax ay
				m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,1,0,1,0,	0,1,0,1,0,1,	0,0,1,0,1,0,	0,0,0,1,0,1,	0,0,0,0,1,0,	0,0,0,0,0,1);
			break;
			case esr_xyh:
				// x y h Vx Vy Vh ax ay ah
				m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,1,0,0,   0,1,0,0,1,0,0,1,0,  0,0,1,0,0,0,0,0,0,  0,0,0,1,0,0,1,0,0,	0,0,0,0,1,0,0,1,0,  0,0,0,0,0,1,0,0,1, 0,0,0,0,0,0,0,0,1,	0,0,0,0,0,0,0,1,0,	0,0,0,0,0,0,0,0,1); // Vh=0, ah=0
			break;
			case esr_XYZ:
				// X Y Z Vx Vy Vz ax ay az
				m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,1,0,0,   0,1,0,0,1,0,0,1,0,  0,0,1,0,0,1,0,0,1,  0,0,0,1,0,0,1,0,0,	0,0,0,0,1,0,0,1,0,  0,0,0,0,0,1,0,0,1, 0,0,0,0,0,0,0,0,1,	0,0,0,0,0,0,0,1,0,	0,0,0,0,0,0,0,0,1);
			break;
			case esr_XYZH:
				// X Y Z H Vx Vy Vz Vh
				//
			break;
            case esr_HorizonPitchHeight:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera aHorizon aPitchCamera aHeightCamera
                //m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
            break;
			default:
				exit(0);
		};
	}

    // Transform the ROI into Kalman measurement model
    m_measurement =  Mat(m_measureParams, 1, CV_32F);
    /*if (m_stateRepres != esr_HorizonPitchHeight)
        RoiToMat(m_roiMeasurement, m_measurement);
    else*/
        RoadToMat(m_roadMeasurement, m_measurement);
 
	// Set initial state
	for (int i=0; i<m_measureParams; i++)
		m_kf.statePre.at<float>(i) = m_measurement.at<float>(i);
	m_kf.statePost = m_kf.statePre;
	m_state = m_measurement;

	// Set measurement matrix as identity
	setIdentity(m_kf.measurementMatrix);

	// Set the measurement noise
	setIdentity(m_kf.measurementNoiseCov, Scalar::all(m_measurementNoise));

	// Set the process noise
	setIdentity(m_kf.processNoiseCov, Scalar::all(m_processNoise));

	// Set the...
	setIdentity(m_kf.errorCovPost, Scalar::all(m_postError));

	// Set the inverse of the covariance to compute distance (inv of identity is identity).
	m_invCov = m_invCov.eye(m_measureParams, m_measureParams, CV_32F);

	// Show the kalman matrices
	//ShowDebugInfo("Initialization");
}

void COpenCVKalman::Initialize(const vector<float>& measurement)
{
    m_roadMeasurement = measurement;
    m_roadState	= measurement;
    m_roadPrediction = measurement;
    // Set in the history as it has a measurement
    m_vEvidenceHistory.push_back(true);

    // Set the state as predicted
    SetTrackerState(estt_init);

    // Intialize the kalman filter sizes
    m_kf.init(m_dynamParams, m_measureParams, m_controlParams, CV_32F);

    // Set the transition matrix
    if (m_order==0)
    {
        setIdentity(m_kf.transitionMatrix);
    }
    else if (m_order==1)
    {
        switch(m_stateRepres)
        {
            case esr_xy:
                // x y Vx Vy
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
            break;
            case esr_xyh:
                // x y h Vx Vy Vh
                //m_kf.transitionMatrix = *(Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,0,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1); // Vh=0
            break;
            case esr_XYZ:
                // X Y Z Vx Vy Vz
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
            break;
            case esr_XYZH:
                // X Y Z H Vx Vy Vz Vh
                //m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
            break;
            case esr_HorizonPitchHeight:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,   0,1,0,0,1,0,  0,0,1,0,0,1,  0,0,0,1,0,0, 0,0,0,0,1,0,  0,0,0,0,0,1);
            break;
            case esr_Horizon:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,  0,1);
            break;
            case esr_PitchHeight:
                // Horizon PitchCamera HeightCamera VHorizon VPitchCamera VHeightCamera
                m_kf.transitionMatrix = (Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,1,0,   0,1,0,1,  0,0,1,0, 0,0,0,1);
            break;
            default:
                exit(0);
        };
    }

    // Transform the ROI into Kalman measurement model
    m_measurement =  Mat(m_measureParams, 1, CV_32F);
    /*if (m_stateRepres != esr_HorizonPitchHeight)
        RoiToMat(m_roiMeasurement, m_measurement);
    else*/
        RoadToMat(m_roadMeasurement, m_measurement);

    // Set initial state
    for (int i=0; i<m_measureParams; i++)
        m_kf.statePre.at<float>(i) = m_measurement.at<float>(i);
    m_kf.statePost = m_kf.statePre;
    m_state = m_measurement;

    // Set measurement matrix as identity
    setIdentity(m_kf.measurementMatrix);

    // Set the measurement noise
    setIdentity(m_kf.measurementNoiseCov, Scalar::all(m_measurementNoise));

    // Set the process noise
    setIdentity(m_kf.processNoiseCov, Scalar::all(m_processNoise));

    // Set the...
    setIdentity(m_kf.errorCovPost, Scalar::all(m_postError));

    // Set the inverse of the covariance to compute distance (inv of identity is identity).
    m_invCov = m_invCov.eye(m_measureParams, m_measureParams, CV_32F);

    // Show the kalman matrices
    //ShowDebugInfo("Initialization");
}


void COpenCVKalman::Finish()
{
}

void COpenCVKalman::RoadToMat(const vector<float>& road, Mat mat) const
{
    switch(m_stateRepres)
    {
        case esr_HorizonPitchHeight:
            mat.at<float>(0) = (float)road[0];
            mat.at<float>(1) = (float)road[1];
            mat.at<float>(2) = (float)road[2];
        break;
        case esr_Horizon:
            mat.at<float>(0) = (float)road[0];
        break;
        case esr_PitchHeight:
            mat.at<float>(0) = (float)road[0];
            mat.at<float>(1) = (float)road[1];
        break;
        default:
            exit(0);
    };
}

void COpenCVKalman::MatToRoad(const Mat mat, vector<float>& road) const
{
    switch(m_stateRepres)
    {
        case esr_HorizonPitchHeight:
            road[0] = (mat.at<float>(0));
            road[1] = (mat.at<float>(1));
            road[2] = (mat.at<float>(2));
        break;
        case esr_Horizon:
            road[0] = (mat.at<float>(0));
            road[1] = (mat.at<float>(1));
            road[2] = (mat.at<float>(2));
        break;
        case esr_PitchHeight:
            road[0] = (mat.at<float>(0));
            road[1] = (mat.at<float>(1));
            road[2] = (mat.at<float>(2));
        break;
        default:
            exit(0);
    };

    //SetTrackerInfo(road);
}


///////////////////////////////////////////////////////////////////////////////
// Functions: Public
///////////////////////////////////////////////////////////////////////////////

void COpenCVKalman::Compute(const vector<float>& measurement)
{
    Predict();
    Update(measurement);

    // Show the kalman matrices
    //ShowDebugInfo("Predicted and Updated");
}

/**************************************************************************//**
 * @brief	Predicts this object.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void COpenCVKalman::Predict()
{
	// Call father class method
	CTrack::Predict();

	// First predict, to update the internal statePre variable
	m_prediction = m_kf.predict();
	
	// To invert covariance to apply Mahalanobis
	m_invCov = m_kf.errorCovPre(Range(0, m_measureParams), Range(0, m_measureParams)).inv();

    MatToRoad(m_prediction, m_roadPrediction);
	// Show the kalman matrices
	//ShowDebugInfo("Predicted");
}


/**************************************************************************//**
 * @brief	Corrects.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	roi	The roi.
 *****************************************************************************/
void COpenCVKalman::Update(const vector<float>& measurement)
{
    // Call father class method
    CTrack::Update(measurement);

    // Transform the ROI into Kalman measurement model
    RoadToMat(m_roadMeasurement, m_measurement);

    // The "correct" phase that is going to use the predicted value and our measurement
    m_state = m_kf.correct(m_measurement);

    // To invert covariance to apply Mahalanobis
    m_invCov = m_kf.errorCovPre(Range(0, m_measureParams), Range(0, m_measureParams)).inv();

    // Convert from kalman state to Roi state
    m_roadState = m_roadMeasurement;

    MatToRoad(m_state, m_roadState);


    // Show the kalman matrices
    //PrintMatrix<float>(m_state, "Updated State");
    //ShowDebugInfo("Updated");
}

void COpenCVKalman::Update()
{
	// Call father class method
	CTrack::Update();

	// The "correct" phase that is going to use only the predicted value
	m_state = m_kf.correct(m_prediction(Range(0, m_measureParams), Range(0, 1)));

	// To invert covariance to apply Mahalanobis
	m_invCov = m_kf.errorCovPre(Range(0, m_measureParams), Range(0, m_measureParams)).inv();

	// Show the kalman matrices
	//ShowDebugInfo("Updated with the prediction");
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Distance to Roi
///////////////////////////////////////////////////////////////////////////////

void COpenCVKalman::ShowDebugInfo(const char* name) const
{
	if (name) cout << "\n\t\t\t[\t" << name << "\t]" << endl;
	cout << "\t\t\t === KALMAN MATRIX ===" << endl;
	PrintMatrix<float>(m_kf.statePre, "statePre");
	cout << endl;
	PrintMatrix<float>(m_kf.statePost, "statePost");
	cout << endl;
	PrintMatrix<float>(m_kf.transitionMatrix, "transitionMatrix");
	cout << endl;
	PrintMatrix<float>(m_kf.controlMatrix, "controlMatrix");
	cout << endl;
	PrintMatrix<float>(m_kf.measurementMatrix, "measurementMatrix");
	cout << endl;
	PrintMatrix<float>(m_kf.processNoiseCov, "processNoiseCov");
	cout << endl;
    PrintMatrix<float>(m_kf.measurementNoiseCov, "measurementNoiseCov");
	cout << endl;
	PrintMatrix<float>(m_kf.errorCovPre, "errorCovPre");
	cout << endl;
	PrintMatrix<float>(m_kf.gain, "gain");
	cout << endl;
	PrintMatrix<float>(m_kf.errorCovPost, "errorCovPost");
	cout << endl;
	PrintMatrix<float>(m_prediction, "m_prediction");
	cout << endl;
	PrintMatrix<float>(m_state, "m_state");
	cout << endl;
	PrintMatrix<float>(m_measurement, "m_measurement");
	cout << endl;
	PrintMatrix<float>(m_invCov, "m_invCov");
	cout << endl;
	cout << "\t\t\t =====================" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////

cv::Mat COpenCVKalman::GetErrorCovPre()
{
    return m_kf.errorCovPre;
}

cv::Mat COpenCVKalman::GetErrorCovPost()
{
    return m_kf.errorCovPost;
}

cv::Mat COpenCVKalman::GetMeasurementNoiseCov()
{
    return m_kf.measurementNoiseCov;
}
cv::Mat COpenCVKalman::GetProcessNoiseCov()
{
    return m_kf.processNoiseCov;
}
