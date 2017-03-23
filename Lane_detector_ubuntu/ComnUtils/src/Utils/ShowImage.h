/* ********************************* FILE ************************************/
/** @file		ShowImage.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CShowImage
 * 
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Feb 09, 2014
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _SHOW_IMAGE_
#define _SHOW_IMAGE_

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
#include "opencv2/opencv.hpp"
//#include <opencv2/viz.hpp>
using std::string;


/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
class CStereoCamera;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
enum EShowImageType { esit_unknown=-1, esit_image, esit_disp, esit_depth };


/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CShowImage
 *
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Feb 02, 2014
 *	\sa			-
 *****************************************************************************/	
class CShowImage  
{
// METHODS
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CShowImage(const string& windowName, EShowImageType imageType=esit_unknown):
        m_windowName(windowName),
		m_imageType(imageType),
        m_isInit(false),
		m_pStereoCamera(NULL)
    {
    }
    ~CShowImage()                   {}
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
	void ShowImage(const cv::Mat& image, const cv::Mat& disp = cv::Mat(), CStereoCamera* pStereoCamera=NULL);
	void PutText(const int x, const int y);
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	int	GetKey();
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CShowImage(const CShowImage& other);
	CShowImage& operator=(const CShowImage& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	void Thread				();
	///@}


// ATRIBUTES
private:

    // Variables
    std::thread                 m_thread;				///< Thread
    cv::Mat                     m_image;				///< Image to show
    string						m_windowName;			///< Window name
    EShowImageType				m_imageType;			///< Image type

    // State
    bool						m_isInit;				///< Is init

    // Member objects
    CStereoCamera*				m_pStereoCamera;		///< Pointer to the stereo camera
    cv::Mat						m_disparity;			///< Pointer to the Disparity map
};
#endif // _SHOW_IMAGE_
