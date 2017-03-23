/* ********************************* FILE ************************************/
/** @file		ShowImage3D.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CShowImage3D
 * 
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Feb 09, 2014
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _SHOW_IMAGE_3D_
#define _SHOW_IMAGE_3D_

#if _MSC_VER > 1000
	#pragma once
#endif

/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <thread>				// std::thread
#include <mutex>				// std::mutex
#include <atomic>				// std::atomic
#include <condition_variable>	// std::condition_variable_any
#include <string>				// std::string
#include <vector>				// std::vector
using std::string;
using std::vector;

// OpenCV
#include "opencv2/opencv.hpp"
#if CV_MAJOR_VERSION>2
#include <opencv2/viz.hpp>


/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CShowImage3D
 *
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Feb 02, 2014
 *	\sa			-
 *****************************************************************************/	
class CShowImage3D  
{
// METHODS
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
    CShowImage3D			(const string& windowName):
        m_windowName		(windowName),
        m_isInit			(false),
        m_removeAllWidgets	(false),
        m_cameraPoseChanged	(false)
    {
    }
    ~CShowImage3D			()                   {}
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize();
	void Finish();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	void ShowWidget			(const string& name, const cv::viz::Widget& widget, const cv::Affine3<double>& pose = cv::Affine3d::Identity());
	void RemoveAllWidgets	();
	void RemoveWidget		(const string& name);
	void SaveView			(const string& fileName);
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	void SetCameraPose(const cv::Affine3f& camPose)		{ m_camPose = camPose; }
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	int	WasStoped();
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CShowImage3D(const CShowImage3D& other);
	CShowImage3D& operator=(const CShowImage3D& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	void Thread				();
	///@}


// ATRIBUTES
private:

	// Variables
	std::thread					m_thread;				///< Thread
	string						m_windowName;			///< Window name
    cv::viz::Viz3d				m_window;				///< Create a window
	vector<cv::viz::Widget>		m_vWidgets;				///< Vector of widgets to show
	vector<string>				m_vNames;				///< Vector of widget names
	vector<cv::Affine3<float>>	m_vPoses;				///< Vector of widget poses
	vector<string>				m_vNamesToRemove;		///< Vector of widget names
	cv::Affine3f				m_camPose;				///< Camera pose

	// State
	bool						m_isInit;				///< Is init
	bool						m_removeAllWidgets;		///< Remove all widgets
	bool						m_cameraPoseChanged;	///< Flag indicating that the camera pose has changes
	std::mutex					m_mutex;				///< Mutex
};
#endif
#endif // _SHOW_IMAGE_3D_
