#include "Track.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	sizeHistory				The size history.
 * @param	maxDistanceThreshold	The maximum distance threshold.
 *****************************************************************************/
CTrack::CTrack(const unsigned int id, const vector<float>& measurement, const EStateRepres stateRepres, const int order):
    m_trackerId			( id			),
    m_stateRepres		( stateRepres	),
    m_order				( order			),
    m_trackerState		( estt_init		),
    m_roadMeasurement	( measurement	),
    m_roadState			( measurement	),
    m_roadPrediction	( measurement	),
    m_numSkips			( 0				),
    m_numMeasurements	( 1				)
{
}

/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CTrack::~CTrack()
{
}

void CTrack::Predict()
{
	// Set the state as predicted
	SetTrackerState(estt_predicted);
}

void CTrack::Update(const vector<float>& measurement)
{
    // Save ROI measurement
    m_roadMeasurement = measurement;

    // Set in the history as it has a measurement
    m_vEvidenceHistory.push_back(true);
    m_numMeasurements++;

    // Set the state as predicted
    //SetTrackerState(estt_updated);
}

void CTrack::Update()
{
	// Set in the history as it has no measurement
	m_vEvidenceHistory.push_back(false);
	m_numSkips++;

	// Set the state as predicted
    if (m_stateRepres!=esr_HorizonPitchHeight)
        SetTrackerState(estt_updatedAuto);

}
