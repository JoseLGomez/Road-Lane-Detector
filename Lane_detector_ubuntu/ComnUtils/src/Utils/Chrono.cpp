#include "MyChrono.h"

/**************************************************************************//**
 * @brief	Default constructor.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CChrono::CChrono()
{
#ifdef WIN32
	QueryPerformanceFrequency(&m_timerFrequency);
#else
    //exit(0);
#endif
}


/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CChrono::~CChrono()
{
}



///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Starts a chrono.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CChrono::StartChrono()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_startCounter);
#else
    m_startCounter = high_resolution_clock::now();
#endif
}



///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Gets the total time.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @return	The total time.
 *****************************************************************************/
double CChrono::GetTotalTime()
{
#ifdef WIN32
	LARGE_INTEGER currentCounter;
	QueryPerformanceCounter(&currentCounter);
	return (static_cast<double>(currentCounter.QuadPart - m_startCounter.QuadPart)*1000 / m_timerFrequency.QuadPart)*0.001f;
#else
    high_resolution_clock::time_point   currentCounter = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(currentCounter - m_startCounter);
    return time_span.count();
#endif
}


/**************************************************************************//**
 * @brief	Gets the time from keypoint.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @return	The time from keypoint.
 *****************************************************************************/
double CChrono::GetTimeFromKeypoint()
{
#ifdef WIN32
	LARGE_INTEGER currentCounter;
	QueryPerformanceCounter(&currentCounter);
	return (static_cast<double>(currentCounter.QuadPart - m_keypoint.QuadPart)*1000 / m_timerFrequency.QuadPart)*0.001f;
#else
    high_resolution_clock::time_point   currentCounter = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(currentCounter - m_keypoint);
    return time_span.count();
#endif
}


/**************************************************************************//**
 * @brief	Gets a total time.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	hours  	The hours.
 * @param [in,out]	minutes	The minutes.
 * @param [in,out]	seconds	The seconds.
 *****************************************************************************/
void CChrono::GetTotalTime(int& hours, int& minutes, float& seconds)
{
	// Get the time and convert to hours, minutes, seconds
	ConvertFromSeconds(GetTotalTime(), hours, minutes, seconds);
}


/**************************************************************************//**
 * @brief	Gets a time from keypoint.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	hours  	The hours.
 * @param [in,out]	minutes	The minutes.
 * @param [in,out]	seconds	The seconds.
 *****************************************************************************/
void CChrono::GetTimeFromKeypoint(int& hours, int& minutes, float& seconds)
{
	// Get the time and convert to hours, minutes, seconds
	ConvertFromSeconds(GetTimeFromKeypoint(), hours, minutes, seconds);
}


/**************************************************************************//**
 * @brief	Convert from seconds.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	totalTime	   	Time of the total.
 * @param [in,out]	hours  	The hours.
 * @param [in,out]	minutes	The minutes.
 * @param [in,out]	seconds	The seconds.
 *****************************************************************************/
void CChrono::ConvertFromSeconds(const double totalTime, int& hours, int& minutes, float& seconds) 
{
	double remainingTime = totalTime;
		// Hours
	hours = int(remainingTime/3600);
	remainingTime = remainingTime - hours*3600;
		// Minutes
	minutes = int(remainingTime/60);
	remainingTime = remainingTime - minutes*60;
		// Seconds
	seconds = (float)remainingTime;
}


/**************************************************************************//**
 * @brief	Sets the time keypoint.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CChrono::SetTimeKeypoint()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_keypoint); 
#else
    m_keypoint = high_resolution_clock::now();
#endif
}



///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Tic: starts the chrono.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CChrono::TIC()
{
	StartChrono();
}


/**************************************************************************//**
 * @brief	Toc: stops the chrono.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CChrono::TOC()
{
	int hours, minutes;
	float seconds;
	GetTotalTime(hours, minutes, seconds);
	printf("Total Elapsed time: %dh %dm %.2fs\n", hours, minutes, seconds);
}


/**************************************************************************//**
 * @brief	Tic: starts a new key point in the chrono
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CChrono::Tic()
{
	SetTimeKeypoint();
}


/**************************************************************************//**
 * @brief	Toc: stops a key point in the chrono
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CChrono::Toc()
{
	int hours, minutes;
	float seconds;
	GetTimeFromKeypoint(hours, minutes, seconds);
	printf("Elapsed time: %dh %dm %.2fs\n", hours, minutes, seconds);
}
