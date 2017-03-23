#include "QSLUnscentedKalman.h"

#include "qvmatrixalgebra.h"
#include "qvukf.h"
using namespace std;
#include <iostream>

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
CQSLUnscentedKalman::CQSLUnscentedKalman(const unsigned int id, const CRoi& measurement, const EStateRepres stateRepres, const int order, const float measurementNoise, const float processNoise):
	CTrack(id, measurement, stateRepres, order),
	m_measurementNoise(measurementNoise),
    m_processNoise(processNoise),
    m_evolutionFunction(stateRepres, order)
{
	switch(m_stateRepres)
	{
		case esr_xy:
			m_measureParams	= 2;
		break;
		case esr_xyh:
			m_measureParams	= 3;
		break;
		case esr_XYZ:
			m_measureParams	= 3;
		break;
		case esr_XYZH:
			m_measureParams	= 4;
		break;
		default:
			exit(0);
	};
	m_dynamParams		= m_measureParams*(m_order+1);
	m_ukf_filter = new QVUKF();
	Initialize();
}


/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CQSLUnscentedKalman::~CQSLUnscentedKalman()
{
	Finish();
}



///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


void CQSLUnscentedKalman::Initialize()
{
	// Set the measurement noise
	m_R_t	= m_measurementNoise*(QVMatrix::identity(m_measureParams));

	// Set the process noise
	m_Q_t	= m_processNoise*(QVMatrix::identity(m_dynamParams));

	// Set in the history as it has a measurement
	m_vEvidenceHistory.push_back(true);

	// Set the state as predicted
	SetTrackerState(estt_init);

	// Transform the ROI into Kalman measurement model
	m_measurement =  QVVector(m_measureParams);
	RoiToVect(m_roiMeasurement, m_measurement);
	m_observationFunction.setObservation(m_measurement);
	m_state = m_measurement;

	// Set initial state
	SetTrackerInfo(m_roiState);

	// Update the tracker
	// m_ukf_filter->update(m_measurement, m_evolutionFunction, m_observationFunction, m_R_t, m_Q_t);
}


void CQSLUnscentedKalman::Finish()
{
	if(m_ukf_filter != NULL)
	{
		delete m_ukf_filter;
		m_ukf_filter = NULL;
	}

}

void CQSLUnscentedKalman::RoiToVect(const CRoi& roi, QVVector& mat) const
{
	switch(m_stateRepres)
	{
		case esr_xy:			
			mat(0) = roi.x();
			mat(1) = roi.y();
			
		break;
		case esr_xyh:
			mat(0) = roi.x();
			mat(1) = roi.y();
			mat(2) = roi.h();
		break;
		case esr_XYZ:
			mat(0) = roi.xw();
			mat(1) = roi.yw();
			mat(2) = roi.hw();
		break;
		case esr_XYZH:
			mat(0) = roi.xw();
			mat(1) = roi.yw();
			mat(2) = roi.zw();
			mat(3) = roi.hw();
		break;
		default:
			exit(0);
	};
}

void CQSLUnscentedKalman::VectToRoi(const QVVector& mat, CRoi& roi) const
{
	switch(m_stateRepres)
	{
		case esr_xy:
			roi.set_x(mat(0));
			roi.set_y(mat(1));
			roi.set_velocityX(mat(2));
			roi.set_velocityY(mat(3));
		break;
		case esr_xyh:
			roi.set_x(mat(0));
			roi.set_y(mat(1));
			roi.set_h(mat(2));
			roi.set_w(mat(2)*m_roiMeasurement.w()/m_roiMeasurement.h());
			roi.set_velocityX(mat(3));
			roi.set_velocityY(mat(4));
			//roi.set_velocityH(mat.at<float>(5));
		break;
		case esr_XYZ:
			roi.set_xw(mat(0));
			roi.set_yw(mat(1));
			roi.set_hw(mat(2));
			roi.set_velocityX(mat(3));
			roi.set_velocityY(mat(4));
			//roi.set_velocityZ(mat.at<float>(5));
		break;
		case esr_XYZH:
			roi.set_xw(mat(0));
			roi.set_yw(mat(1));
			roi.set_zw(mat(2));
			roi.set_hw(mat(3));
			roi.set_ww(mat(3)*m_roiMeasurement.ww()/m_roiMeasurement.hw());
			roi.set_velocityX(mat(3));
			roi.set_velocityY(mat(4));
			//roi.set_velocityZ(mat.at<float>(6));
			//roi.set_velocityH(mat.at<float>(7));
		break;
		default:
			exit(0);
	};

	SetTrackerInfo(roi);
}


///////////////////////////////////////////////////////////////////////////////
// Functions: Public
///////////////////////////////////////////////////////////////////////////////

void CQSLUnscentedKalman::Compute(const CRoi& measurement)
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
void CQSLUnscentedKalman::Predict()
{
	// Call father class method
	CTrack::Predict();

	// First predict, to update the internal statePre variable
	m_prediction = m_ukf_filter->getState();
	
	// To invert covariance to apply Mahalanobis
	// m_invCov = m_kf.errorCovPre(Range(0, m_measureParams), Range(0, m_measureParams)).inv();

	// Convert from kalman prediction to Roi prediction
	VectToRoi(m_prediction, m_roiPrediction);

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
void CQSLUnscentedKalman::Update(const CRoi& measurement)
{	
	// Call father class method
	CTrack::Update(measurement);

	// Transform the ROI into Kalman measurement model
	RoiToVect(m_roiMeasurement, m_measurement);

	// The "correct" phase that is going to use the predicted value and our measurement
	m_ukf_filter->update(m_measurement, m_evolutionFunction, m_observationFunction, m_R_t, m_Q_t);
	m_state = m_ukf_filter->getState();

	// To invert covariance to apply Mahalanobis
	// m_invCov = m_kf.errorCovPre(Range(0, m_measureParams), Range(0, m_measureParams)).inv();

	// Convert from kalman state to Roi state
	m_roiState = m_roiMeasurement;
	VectToRoi(m_state, m_roiState);

	// Show the kalman matrices
	//PrintMatrix<float>(m_state, "Updated State");
	//ShowDebugInfo("Updated");
	
}

void CQSLUnscentedKalman::Update()
{
	// Call father class method
	CTrack::Update();

	// The "correct" phase that is going to use only the predicted value
	// m_state = m_kf.correct(m_prediction(Range(0, m_measureParams), Range(0, 1)));
	QVVector mini_prediction(m_measureParams);
	for(int i=0; i<m_measureParams;i++)
		mini_prediction(i) = m_prediction(i);
	
	m_ukf_filter->update(mini_prediction, m_evolutionFunction, m_observationFunction, m_R_t, m_Q_t);
	m_state = m_ukf_filter->getState();

	// To invert covariance to apply Mahalanobis
	// m_invCov = m_kf.errorCovPre(Range(0, m_measureParams), Range(0, m_measureParams)).inv();

	// Convert from kalman state to Roi state
	VectToRoi(m_state, m_roiState);

	// Show the kalman matrices
	//ShowDebugInfo("Updated with the prediction");
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Distance to Roi
///////////////////////////////////////////////////////////////////////////////

void CQSLUnscentedKalman::ShowDebugInfo(const char* name) const
{
	/*
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
	*/
}

/**************************************************************************//**
 * @brief	Distance to roi.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	pRoi	If non-null, the roi.
 *
 * @return	.
 *****************************************************************************/
float CQSLUnscentedKalman::DistanceToRoi(const CRoi& roi) const
{
	QVVector aux(m_measureParams);
	RoiToVect(roi, aux);

	QVVector mini_state(m_measureParams);
	for(int i=0; i<m_measureParams;i++)
		mini_state(i) = m_prediction(i);

	//float distance = (float)Mahalonobis(aux, mini_state, m_invCov);
	float distance = (aux - mini_state).norm2();

	return distance;
}
	
