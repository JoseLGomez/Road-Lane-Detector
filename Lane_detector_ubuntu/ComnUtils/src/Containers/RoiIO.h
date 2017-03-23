/* ********************************* FILE ************************************/
/** @file		RoiIO.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CRoiIO
 * 
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _ROI_IO_
#define _ROI_IO_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "Roi.h"
#include <string>
using std::string;

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/
class CIniFileIO;
class CStereoCamera;
namespace cv
{
	class Mat;
};


/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
enum ERoiIOType { eriot_unknown=-1, eriot_rois, eriot_confidences, eriot_evaluationCaltech, eriot_evaluationCaltechViewpoint, eriot_disparity, eriot_evaluationKitti };



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CRoiIO
 *
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CRoiIO
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CRoiIO();
	~CRoiIO();
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	static RoiDisplayParam ReadRoiDisplayParam	(CIniFileIO* pIniFileIO, const string& section);
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	static void Show							(cv::Mat& image, const CRoi& roi, const CColor& color=CColor(255,255,255,1), const int width=1, const bool showConfidence=true, const bool showDistance=false, const bool showTrackingInfo=false);
	static void Show							(cv::Mat& image, const vector<CRoi*>& vRois, RoiDisplayParam dispParam);
	static void ShowAsPoint						(cv::Mat& image, const CRoi& roi, const CColor& color=CColor(255,255,255,1), const bool showConfidence=true, const bool showDistance=false);
	static void ShowAsPoint						(cv::Mat& image, const vector<CRoi*>& vRois, RoiDisplayParam dispParam);

	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	// Functions: Read/Write
	static void Read							(const string& filename, const ERoiIOType type, vector<CRoi*>& vRois);
	static void Write							(const string& filename, const ERoiIOType type, const vector<CRoi*>& vRois, const string& object = "Pedestrian");
	static void ReadConfidences					(const string& filename, vector<float>& vConfidences);
	static void WriteConfidences				(const string& filename, const vector<float>& vConfidences);

	// Write Functions: Disparities
	static void ReadDisparities					(const string& filename, vector<CRoi*>& vRois);
	static void WriteDisparities				(const string& filename, const vector<CRoi*>& vRois);

	///@}

 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Delete Filters
	///@{
	static void ClearRoisConfLabel				(vector<CRoi*>& vRois);
	static void RemoveRoisByConfidence			(const float minConfidence, vector<CRoi*>& vRois);
	static void RemoveRoisByLabel				(const float labelToRemove, vector<CRoi*>& vRois);
	static void RemoveRoisByClass				(const classType classToRemove, vector<CRoi*>& vRois);
	static void RemoveRoisByClass2				(const classType classToRetain, vector<CRoi*>& vRois);
	static void RemoveRoisByMandatory			(vector<CRoi*>& vRois);
	static void RemoveRoisByHeight				(const float minHeight, const float maxHeight, vector<CRoi*>& vRois);
	static void RemoveRoisFromTop				(vector<CRoi*> &vRois, const int minY);
	static void RemoveRoisFromBottom			(vector<CRoi*> &vRois, const int minY);
	static void RemoveRoisFromRange				(vector<CRoi*> &vRois, const int minX, const int maxX, const int minY, const int maxY);
	static void RemoveRoisFromRange2			(vector<CRoi*> &vRois, const int minX, const int maxX, const int minY, const int maxY);
	static void RemoveRoisFromRangeAdaptative	(vector<CRoi*> &vRois, const float A, const float B, const float C, const int marginY, const float percMargin);
	static void RemoveRoisFromRangeAdaptative	(vector<CRoi*> &vRois, const float A, const float B, const float D1, const float D2, const int marginY, const float percMargin);
	static void NormalizeRoiAR					(const float AR, vector<CRoi*>& vRois);
	static void RemoveRoisDuplicated			(vector<CRoi*>& vRois);
	///@}

 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Remove Filters
	///@{
	static void FilterRoisByConfidence			(const float minConfidence, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByConfidence			(const float minConfidence, const float maxConfidence,const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByClass				(const classType classToRemove, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByClass2				(const classType classToRetain, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
    static void FilterRoisByLabel				(const classType labelToRemove, const vector<vector<CRoi*> >& vvRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByLabel				(const float labelToRemove, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
    static void FilterRoisByLabel				(const float labelToRemove, const vector<vector<CRoi*> >& vvRois, vector<CRoi*>& vRoisOut);

	static void FilterRoisByMandatory			(const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisForTraining			(const classType objClass, const float minHeight, const visibilityType visibilityType, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByVisibility			(const visibilityType typeToRetain, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByHeight				(const float minHeight, const float maxHeight, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByContactToBorder		(const int imgSizeX, const int imgSizeY, const float maxPercX, const float maxPercY, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut, const float percMargin);
	static void FilterRoisRandom				(const int nRois, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisRandomInRange			(const int nRois,const float minConfidence,const float maxConfidence, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	static void FilterRoisByScaleFactor			(const float minScalefactor, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut);
	///@}

 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	static void DivideRoisByScale				(const vector<CRoi*>& vRois, vector<vector<CRoi*> >& vvRois, const int nScales);
	static void UndivideRoisByScale				(const vector<vector<CRoi*> >& vvRois, vector<CRoi*>& vRois);
	static void SetLabelsByConfidence			(const float minConfidence, vector<CRoi*>& vRois);
	static void SetObligatoriesByHeight			(const float minHeight, const float maxHeight, vector<CRoi*>& vRois);
	///@}

	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Sort functions
	///@{
	static void Sort							(vector<CRoi*>& vRois)		{ Quicksort(vRois,0,(int)vRois.size()-1); }
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Disparity functions
	///@{
	static void saveDisparityMean				(vector<CRoi*>& vRois, const cv::Mat& disp, const float minDisparityPixels, const CStereoCamera& pStereoCamera, bool recompute3Drois);
	static void saveDisparityMedian				(vector<CRoi*>& vRois, const cv::Mat& disp, const float minDisparityPixels, const CStereoCamera& pStereoCamera, bool recompute3Drois);
	static void saveDisparityFoot				(vector<CRoi*>& vRois, const cv::Mat& disp, const CStereoCamera& pStereoCamera, bool recompute3Drois);
	///@}

 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	// Free vector of rois
	static void FreeRois (vector<CRoi*>& vRois)
	{
		for (unsigned int i=0; i<vRois.size(); i++) 
			delete vRois[i];
		vRois.clear();
	}

    static void FreeRois (vector<vector<CRoi*> >& vvRois)
	{
		for( unsigned int j=0; j<vvRois.size(); j++)
		{
			for (unsigned int i=0; i<vvRois[j].size(); i++)
				delete vvRois[j][i];
			vvRois[j].clear();
		}
		vvRois.clear();
	}
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	///@}
	
private:
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CRoiIO(const CRoiIO& other);
	CRoiIO& operator=(const CRoiIO& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Read/Write Functions
	///@{
	// Read/Write Functions: RoisExtended
	static void ReadRois(const string& filename, vector<CRoi*>& vRois);
	static void WriteRois(const string& filename, const vector<CRoi*>& vRois);

	// Read/Write Functions: Detections extended
	static void ReadDetections(const string& filename, vector<CRoi*>& vRois);
	static void WriteDetections(const string& filename, const vector<CRoi*>& vRois);

	// Read/Write Functions: Confidences
	static void ReadConfidences(const string& filename, vector<CRoi*>& vRois);
	static void WriteConfidences(const string& filename, const vector<CRoi*>& vRois);

	// Read/Write Functions: Annotations extended
	static void ReadAnnotations(const string& filename, vector<CRoi*>& vRois);
	static void WriteAnnotations(const string& filename, const vector<CRoi*>& vRois);

	// Write Functions: detections for Caltech or Kitti evaluation
	static void WriteEvaluationCaltech(const string& filename, const vector<CRoi*>& vRois, const bool saveViewpoint = false);
	static void WriteEvaluationKitti(const string& filename, const vector<CRoi*>& vRois, const string& object = "Pedestrian");
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Sort functions
	///@{
	static int Partition(vector<CRoi*>& vRois, int left, int right);
	static void Quicksort(vector<CRoi*>& vRois, int lo, int hi);
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions: Compute median
	///@{
	static float MedianBinsApproximation(const int NBins, const float mean, const float desvStd, const vector<int>& arrayData);
	///@}


// ATRIBUTES
};
#endif // _ROI_IO_
