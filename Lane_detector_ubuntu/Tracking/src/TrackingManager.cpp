#include "TrackingManager.h"

/*#include "Utils/IniFileIO.h"
#include "Camera.h"
#include "Refinement.h"
#include "Containers/RoiIO.h"
#include "Maths/munkres.h"
#include "Maths/Maths.h"*/
#include "Kalman/OpenCVKalman.h"
//#include "UKF/QSLUnscentedKalman.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	parametersFile	The parameters file.
 *****************************************************************************/
CTrackingManager::CTrackingManager(const string& parametersFile):
	m_parametersFile( parametersFile	),
	m_id			( 0					)
{
	// Read parameters
	ReadParameters();
}


/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CTrackingManager::~CTrackingManager()
{
	Finish();
}



///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Initializes this object.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CTrackingManager::Initialize()
{
}


/**************************************************************************//**
 * @brief	Reads the parameters.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CTrackingManager::ReadParameters()
{
	// Init INI file reader
	CIniFileIO IniFileIO(m_parametersFile);

	// Algorithm
	IniFileIO.ReadSection("TRACKING", "algorithm", m_algorithmName, "none", "[none | kalman | UKF]");
	SetAlgorithm(m_algorithmName);
	string str;
	IniFileIO.ReadSection("TRACKING", "StateRepresentation", str, "xyh", "State representation [xy | xyh | XYZ | XYZH]");
	SetStateRepres(str);
	IniFileIO.ReadSection("TRACKING", "Order", m_order, 1, "Dynamic modeling order: 0=constant position, 1=Constant velocity, 2=Constant aceleration");

	// Tracking history
	IniFileIO.ReadSection("TRACKING", "MinNumMeassurement", m_minNumMeassurement, 2, "Minimum number of meassurements of a tracker. Until this minimum is not reached it is not considered ad a real tracked object");
	IniFileIO.ReadSection("TRACKING", "MaxNumSkips", m_maxNumSkips, 2, "Maximum number of updates of a tracker without a measurement. If there are more, the tracker is destroied");
	
	// Data association
	IniFileIO.ReadSection("TRACKING", "DistanceMeasure", str, "RelDist", "Type of ROI distance. [AbsDist | RelDist | Overlapping]");
	SetDistanceMeasure(str);
	IniFileIO.ReadSection("TRACKING", "OverlappingMeasure", str, "OverlappingPascal", "Type of ROI overlapping. [xys | OverlappingPascal | OverlappingTUD | overlappingPedro]");
	m_overlappingType = CRefinement::GetDistanceMeasure(str);
	
	IniFileIO.ReadSection("TRACKING", "MaxDistance", m_maxDistance, 500, "Maximum distance in the final mahalanobis distance");

	IniFileIO.ReadSection("TRACKING", "MaxSearchOverlapping", m_maxSearchOverlapping, 0.9f, "Maximum search range in the distance of overlapping of the image");
	IniFileIO.ReadSection("TRACKING", "MaxSearchX", m_maxSearchX, 10, "Maximum search range in pixel for the x-direction of the image");
	IniFileIO.ReadSection("TRACKING", "MaxSearchY", m_maxSearchY, 10, "Maximum search range in pixel for the y-direction of the image");
	IniFileIO.ReadSection("TRACKING", "MaxSearchH", m_maxSearchH, 50, "Maximum search range in pixel for the box height of the image");

	IniFileIO.ReadSection("TRACKING", "MaxSearchXworld", m_maxSearchXworld, 1, "Maximum search range in meters for the x-direction of the world");
	IniFileIO.ReadSection("TRACKING", "MaxSearchYworld", m_maxSearchYworld, 0.5, "Maximum search range in meters for the y-direction of the world");
	IniFileIO.ReadSection("TRACKING", "MaxSearchZworld", m_maxSearchZworld, 3, "Maximum search range in meters for the z-direction of the world");
	IniFileIO.ReadSection("TRACKING", "MaxSearchHworld", m_maxSearchHworld, 0.5, "Maximum search range in meters for the box height of the world");

	// Kalman and Unscented Kalman
	IniFileIO.ReadSection("KALMAN", "KalmanMeasurementNoise", m_KalmanMeasurementNoise, 0.1f, "Kalman Measurement Noise");
	IniFileIO.ReadSection("KALMAN", "KalmanProcessNoise", m_KalmanProcessNoise, 0.01f, "Kalman Process Noise");
	IniFileIO.ReadSection("KALMAN", "KalmanPostError", m_KalmanPostError, 0.1f, "Kalman Post Error");

	// Finish
	IniFileIO.Finish();
}


/**************************************************************************//**
 * @brief	Sets an algorithm.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	algorithm	The algorithm.
 *****************************************************************************/
void CTrackingManager::SetAlgorithm(string& algorithm)
{
	transform(algorithm.begin(), algorithm.end(), algorithm.begin(), tolower);
	if (algorithm == "none")
		m_algorithmType = erta_none;
	else if (algorithm == "kalman")
		m_algorithmType = erta_kalman;
	else if (algorithm == "ukf")
		m_algorithmType = erta_unscented_kalman;
	else
		ErrorQuit(VSys_UnkINIParam, algorithm);
}

void CTrackingManager::SetStateRepres(string& algorithm)
{
	transform(algorithm.begin(), algorithm.end(), algorithm.begin(), tolower);
	if		(algorithm == "xy")
		m_stateRepres = esr_xy;
	else if (algorithm == "xyh")
		m_stateRepres = esr_xyh;
	else if (algorithm == "xyz")
		m_stateRepres = esr_XYZ;
	else if (algorithm == "xyzh")
		m_stateRepres = esr_XYZH;
	else
		ErrorQuit(VSys_UnkINIParam, algorithm);
}

void CTrackingManager::SetDistanceMeasure(string& str)
{
	transform(str.begin(), str.end(), str.begin(), tolower);
	if		(str == "absdist")
		m_distanceType = erdt_absDist;
	else if (str == "reldist")
		m_distanceType = erdt_relDist;
	else if (str == "overlapping")
		m_distanceType = erdt_overlapping;
	else
		ErrorQuit(VSys_UnkINIParam, str);
}


/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CTrackingManager::Finish()
{
	for (unsigned int i=0; i<m_vTracks.size(); i++)
		delete m_vTracks[i];
	m_vTracks.clear();
}



///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Computes.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	vRois		   	The rois.
 * @param [in,out]	vTracks	[in,out] If non-null, the tracks.
 *****************************************************************************/
void CTrackingManager::Compute(const vector<CRoi*>& vRois, vector<CRoi*>& vTracks, CMonoCamera& camera, float speed, float yawRate, float cycleTime)
{
	// If tracker disabled stop
	if	(m_algorithmType==erta_none) 
	{
		//vTracks = vRois;
		//vTracks.resize(vRois.size());
		//for (unsigned int i=0; i<vRois.size(); i++)
		//{
		//	vTracks[i] = new CRoi(*vRois[i]);
		//}
		return;
	}

	// Set vehicle inertial sensers data and camera
	SetInertialSensorData	(camera, speed, yawRate, cycleTime);

	// Predict trackers
	PredictTrackers();

	// Compute the distance matrix from predictions and measurements: Optimium pairs by Hungarian method
	//CMatrix2D distanceMatrix2 = ComputeDistanceMatrix2(vRois);
	vector<vector<float> > distanceMatrix = ComputeDistanceMatrix(vRois);

	// Compute the data association given the distance matrix
	vector<vector<float> > vAssociations =	ComputeDataAssociation	(distanceMatrix);
	//vector<vector<float> > vAssociations2 =	ComputeDataAssociation2	(distanceMatrix2);

	// Correct tracks with matched ROI meassurements
	vector<bool> vAsignedRois = UpdateTrackers(vAssociations, vRois);

	// Update or delete the old trackers
	SkipUpdates();

	// Create new tracks for ROI meassurements not assigned to any tracker
	CreateNewTrackers(vRois, vAsignedRois);

	// Set the tracked rois as output
	SetTrackedRois(vTracks);
}

void CTrackingManager::SetInertialSensorData(const CMonoCamera& camera, const float speed, const float yawRate, const float cycleTime)
{
	m_pCamera	= &camera;
	m_speed		= speed;
	m_yawRate	= yawRate;
	m_cycleTime	= cycleTime;
}

void CTrackingManager::PredictTrackers()
{
	// Predict trackers
	for (unsigned int i=0; i<m_vTracks.size(); i++) 
		m_vTracks[i]->Predict();
}

float CTrackingManager::Distance(CRoi& roi, CRoi& track, CTrack& tracker) const
{
	float dist, overlappingDist, diffX, diffY, diffH;

	switch (m_distanceType)
	{
		case erdt_absDist:
			diffX = float(track.x()-roi.x());
			diffY = float(track.y()-roi.y());
			diffH = float(track.h()-roi.h());
			if (( fabsf( diffX ) > m_maxSearchX ) ||
				( fabsf( diffY ) > m_maxSearchY ) ||
				( fabsf( diffH ) > m_maxSearchH ) )
				dist = 99999;
			else
				dist = tracker.DistanceToRoi(roi);
		break;
		case erdt_relDist:
			diffX = (track.x()-roi.x())/(float)track.w();
			diffY = (track.y()-roi.y())/(float)track.y();
			diffH = (track.h()-roi.h())/(float)track.h();
			if (( fabsf( diffX ) > m_maxSearchX ) ||
				( fabsf( diffY ) > m_maxSearchY ) ||
				( fabsf( diffH ) > m_maxSearchH ) )
				dist = 99999;
			else
				dist = tracker.DistanceToRoi(roi);
		break;
		case erdt_overlapping:
			overlappingDist = CRefinement::GetRoiDistance(track, roi, m_overlappingType);
			if (overlappingDist > m_maxSearchOverlapping )
				dist = 99999;
			else
				dist = tracker.DistanceToRoi(roi);
		default:
			ErrorQuit(VSys_IncINIParam, "");
	};

	return dist;
}

//CMatrix2D CTrackingManager::ComputeDistanceMatrix2(const vector<CRoi*>& vRois) const
//{
//	// Create the distance matrix
//	CMatrix2D distanceMatrix((int)m_vTracks.size(), (int)vRois.size());
//
//	// Compute the distance matrix
//	for (unsigned int i=0; i<m_vTracks.size(); i++)
//	{
//		CRoi track = m_vTracks[i]->GetRoiPrediction();
//		for (unsigned int j=0; j<vRois.size(); j++)
//		{
//			float distance = Distance(*vRois[j], track, *m_vTracks[i]);
//			distanceMatrix.Set(i, j, distance);
//		}
//	}
//
//	// Show debug info
//	printf("\n  > %2d detections and %2d trackers\n", (int)vRois.size(), (int)m_vTracks.size());
//	distanceMatrix.Show("Distance matrix");
//
//	// Return the matrix
//	return distanceMatrix;
//}

vector<vector<float> > CTrackingManager::ComputeDistanceMatrix(const vector<CRoi*>& vRois) const
{
	// Create the distance matrix
	vector<vector<float> > distanceMatrix((int)m_vTracks.size(), vector<float>((int)vRois.size()));

	// Compute the distance matrix
	for (unsigned int i=0; i<m_vTracks.size(); i++)
	{
		CRoi track = m_vTracks[i]->GetRoiPrediction();
		for (unsigned int j=0; j<vRois.size(); j++)
		{
			float distance = Distance(*vRois[j], track, *m_vTracks[i]);
			distanceMatrix[i][j] = distance;
		}
	}

	// Show debug info
	printf("\n  > %2d detections and %2d trackers\n", (int)vRois.size(), (int)m_vTracks.size());
	//ShowMatrix<float>(distanceMatrix, "Distance matrix");
	
	// Return the matrix
	return distanceMatrix;
}


//vector<vector<float> > CTrackingManager::ComputeDataAssociation2(const CMatrix2D& distanceMatrix) const
//{
//	vector<vector<float> > vAssociations;
//	HungarianAlgorithm(distanceMatrix, vAssociations);
//	ShowMatrix<float>(vAssociations, "AssociationMatrix matrix");
//	return vAssociations;
//}

vector<vector<float> > CTrackingManager::ComputeDataAssociation(const vector<vector<float> >& distanceMatrix) const
{
	// Initialize input matrix
	int ncols = (int)distanceMatrix.size();
	int nrows = (ncols>0) ? (int)distanceMatrix[0].size() : 0;
	Matrix<double> matrix(nrows, ncols);
	for (int row=0; row<nrows; row++) {
		for (int col=0; col<ncols; col++) {
			matrix(row,col) = (double)distanceMatrix[col][row];
		}
	}

	//// Display begin matrix state.
	//for ( int row = 0 ; row < nrows ; row++ ) {
	//	for ( int col = 0 ; col < ncols ; col++ ) {
	//		std::cout.width(2);
	//		std::cout << matrix(row,col) << ",";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;

	// Apply Munkres algorithm to matrix.
	Munkres m;
	m.solve(matrix);

	//// Display solved matrix.
	//for ( int row = 0 ; row < nrows ; row++ ) {
	//	for ( int col = 0 ; col < ncols ; col++ ) {
	//		std::cout.width(2);
	//		std::cout << matrix(row,col) << ",";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;

	// Create asignation matrix
	vector<vector<float> > vAssociations;
	for (int row=0; row<nrows; row++)
	for (int col=0; col<ncols; col++)
	{
		if (matrix(row,col)==0)
		{
			vector<float> association(3);
			association[0] = (float)col;
			association[1] = (float)row;
			association[2] = distanceMatrix[col][row];
			vAssociations.push_back(association);
		}
	}
	//ShowMatrix(vAssociations, "Association matrix");

	return vAssociations;
}

vector<bool> CTrackingManager::UpdateTrackers(const vector<vector<float> >& vAssociations, const vector<CRoi*>& vRois)
{
	vector<bool> vAssignedRois(vRois.size(), false);

	// Correct tracks with matched ROI meassurements
	//printf("\n == Associated trackers == \n");
	for (unsigned int i=0; i<vAssociations.size(); i++)
	{
		// Get pair
		int iTrack = (int)vAssociations[i][0];
		int iRoi   = (int)vAssociations[i][1];
		float dist = vAssociations[i][2];

		// Threshold far detections
		if (dist<m_maxDistance)
		{
			m_vTracks[iTrack]->Update(*vRois[iRoi]);
			vAssignedRois[iRoi] = true;
			// Show debug info
			printf("  > Tracker: %3u updated with detection: %3d, Distance: %6.2f\n", m_vTracks[iTrack]->GetTrackerId(), iRoi, dist);
		}
		else
		{
			printf("  > Tracker: %3u, Detection: %3d, Distance: %6.2f, No updated\n", m_vTracks[iTrack]->GetTrackerId(), iRoi, dist);
		}
	}

	return vAssignedRois;
}

void CTrackingManager::SkipUpdates()
{
	// Auxiliar vector
	vector<CTrack*> vTracksFiltered;

	// Show debug info
	//printf("\n == Updated and deleted old trackers == \n");

	// Update or delete old tracks
	for (unsigned int i=0; i<m_vTracks.size(); i++)
	{
		// If it has been updated do nothing
		if (m_vTracks[i]->GetTrackerState() == estt_updated)
		{
			vTracksFiltered.push_back(m_vTracks[i]);
			continue;
		}

		// Check if it has some evidences in the last m_maxNumSkips frames
		bool bSomeEvidence = false;
		for (int j=(int)m_vTracks[i]->m_vEvidenceHistory.size()-1, k=0; j>=0 && k<m_maxNumSkips; j--, k++)
		{
			if (m_vTracks[i]->m_vEvidenceHistory[j])
			{
				bSomeEvidence = true;
				break;
			}
		}

		// Remove old tracker
		if (!bSomeEvidence)
		{
			// Show debug info
			printf("  > Tracker: %3u deleted after %2d frames without detections\n ", m_vTracks[i]->GetTrackerId(), m_maxNumSkips);
			//for (unsigned int j=0; j<m_vTracks[i]->m_vEvidenceHistory.size(); j++)
			//	printf("%s, ", (m_vTracks[i]->m_vEvidenceHistory[j]) ? "true" : "false");
			//printf("\n");
			delete m_vTracks[i];
		}

		// Update traker with its own prediction
		else
		{
			m_vTracks[i]->Update();
			vTracksFiltered.push_back(m_vTracks[i]);
			printf("  > Tracker: %3u updated with its own prediction\n", m_vTracks[i]->GetTrackerId());
		}
	}

	m_vTracks = vTracksFiltered;
}

void CTrackingManager::CreateNewTrackers(const vector<CRoi*>& vRois, vector<bool> vAssignedRois)
{
	//printf("\n == New trackers Created == \n");
	for (unsigned int i=0; i<vRois.size(); i++)
		if (!vAssignedRois[i])
		{
			m_vTracks.push_back(CreateNewTrack(*vRois[i]));
			printf("  > Tracker %3u created\n", m_vTracks[m_vTracks.size()-1]->GetTrackerId());
		}
}

void CTrackingManager::SetTrackedRois(vector<CRoi*>& vTracks) const
{
	vTracks.resize(m_vTracks.size(), NULL);
	for (unsigned int i=0; i<m_vTracks.size(); i++)
	{
		vTracks[i] = new CRoi(m_vTracks[i]->GetRoiState());
		vTracks[i]->set_trackerActive((int)m_vTracks[i]->m_numMeasurements >= m_minNumMeassurement);
	}
}


/**************************************************************************//**
 * @brief	Creates a new track.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	roi	The roi.
 *
 * @return	The new new track.
 *****************************************************************************/
CTrack* CTrackingManager::CreateNewTrack(CRoi& roi)
{
	switch(m_algorithmType)
	{
		case erta_none:
			return NULL;

		case erta_kalman:
			return new COpenCVKalman(m_id++, roi, m_stateRepres, m_order, m_KalmanMeasurementNoise, m_KalmanProcessNoise, m_KalmanPostError);

	//	case erta_unscented_kalman:
	//		return new CQSLUnscentedKalman(m_id++, roi, m_stateRepres, m_order, m_KalmanMeasurementNoise, m_KalmanProcessNoise);

		default:
			ErrorQuit(VSys_NotImplemented, "");
			return NULL;
	};
}
