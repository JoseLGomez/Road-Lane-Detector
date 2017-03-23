/* ********************************* FILE ************************************/
/** @file		Roi.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CRoi
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _ROI_
#define _ROI_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "../Utils/Color.h"
#include <vector>
#include <math.h>  
using std::vector;


/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/
#define Round_(x) int(x + ((x>=0)? 0.5 : -0.5))

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
enum classType {CT_Unknown=-1, CT_Pedestrian, CT_Car, CT_Van, CT_Truck, CT_Bike, CT_Group, CT_Background};
enum viewType {VT_Unknown=-1, VT_Front, VT_Left, VT_Right, VT_Back};
enum visibilityType {VBT_Unknown=-1, VBT_FullyVisible, VBT_PartOccluded, VBT_Occluded};
enum sceneType {ST_Unknown=-1, ST_Street, ST_Road, ST_Highway, ST_Forest};
enum poseType {PT_Unknown=-1, PT_Stand, PT_Walk, PT_Sit, PT_Other};
enum gtMatchType {GTT_Unknown, GTT_TP, GTT_FP, GTT_TN, GTT_FN};
struct RoiDisplayParam
{
	bool	m_isEnabled;		///< To show or not to show
	bool	m_showConfidence;	///< Show the window confidence
	bool	m_showDistance;		///< Show the distance to the groundtruth
	bool	m_showTrackingInfo;	///< Show the trackingInfo
	CColor	m_color;			///< Bounding box color
	CColor	m_colorOptional;	///< Bounding box color optional (For annotations optional)
	int		m_lineWidth;		///< Bounding box line width
	int		m_lineWidthOptional;///< Bounding box line width optional (For annotations optional)
	float	m_threshold;		///< Threshold to be shown
};


/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CRoi
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CRoi
{
// METHODS
public:
	///////////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	///////////////////////////////////////////////////////////////////////////////
	CRoi():
		m_x(0), m_y(0), m_w(0), m_h(0),									// Image coordinates
		m_xs(0), m_ys(0), m_ws(0), m_hs(0), 							// ImagePiramyd slice coordinates
		m_scaleIndex(0), m_s(1), 										// ImagePiramid slice index and scale
		m_timeIndex(0),													// ImagePiramid time index
		m_xw(0), m_yw(0), m_zw(0), m_ww(0), m_hw(0), m_dw(0),			// World coordinates (Stereo)
		m_label(0), m_confidence(0),									// Label and confidence
		m_obligatory(true),												// Indicates if it is mandatory to detect (For annotations)
		m_stdDev(-1.0f),												// StdDev of the ROI
		m_class(CT_Unknown), m_view(VT_Unknown), 
		m_visibility(VBT_Unknown), m_scene(ST_Unknown), m_pose(PT_Unknown),
		m_selected(false),
		m_trackerId(0), m_trackerActive(false), m_trackerAge(0), m_trackerNumSkips(0), m_velocityX(0), m_velocityY(0),
		m_dispR(0), m_dispT(0)
	{}

	CRoi(const int x, const int y, const int w, const int h, const float conf=0.0f):
		m_x(x), m_y(y), m_w(w), m_h(h),									// Image coordinates
		m_xs(x), m_ys(y), m_ws(w), m_hs(h), 							// ImagePiramyd slice coordinates
		m_scaleIndex(0), m_s(1), 										// ImagePiramid slice index and scale
		m_timeIndex(0),													// ImagePiramid time index
		m_xw(0), m_yw(0), m_zw(0), m_ww(0), m_hw(0), m_dw(0),			// World coordinates (Stereo)
		m_label(0), m_confidence(conf),									// Label and confidence
		m_obligatory(true),												// Indicates if it is mandatory to detect (For annotations)
		m_stdDev(-1.0f),												// StdDev of the ROI
		m_class(CT_Unknown), m_view(VT_Unknown), 
		m_visibility(VBT_Unknown), m_scene(ST_Unknown), m_pose(PT_Unknown),
		m_selected(false),
		m_trackerId(0), m_trackerActive(false), m_trackerAge(0), m_trackerNumSkips(0), m_velocityX(0), m_velocityY(0),
		m_dispR(0), m_dispT(0)
	{}

	CRoi(const int xs, const int ys, const int ws, const int hs, const int scaleIndex, const float scaleFactor, const int x, const int y, const int w, const int h):
		m_x(x), m_y(y), m_w(w), m_h(h),									// Image coordinates
		m_xs(xs), m_ys(ys), m_ws(ws), m_hs(hs),							// ImagePiramyd slice coordinates
		m_scaleIndex(scaleIndex), m_s(scaleFactor), 					// ImagePiramid slice index and scale
		m_timeIndex(0),													// ImagePiramid time index
		m_xw(0), m_yw(0), m_zw(0), m_ww(0), m_hw(0), m_dw(0),			// World coordinates (Stereo)
		m_label(0), m_confidence(0),									// Label and confidence
		m_obligatory(true),												// Indicates if it is mandatory to detect (For annotations)
		m_stdDev(-1.0f),												// StdDev of the ROI
		m_class(CT_Unknown), m_view(VT_Unknown), 
		m_visibility(VBT_Unknown), m_scene(ST_Unknown), m_pose(PT_Unknown),
		m_selected(false),
		m_trackerId(0), m_trackerActive(false), m_trackerAge(0), m_trackerNumSkips(0), m_velocityX(0), m_velocityY(0),
		m_dispR(0), m_dispT(0)
	{}

	~CRoi()
		{}


	///////////////////////////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////////////////////////
	bool operator > (const CRoi roi) const	
	{ 
		return ((m_x > roi.x()) || ((m_x == roi.x()) && ((m_y > roi.y()) || ((m_y == roi.y()) && ((m_w > roi.w()) || ((m_w == roi.w()) && (m_h > roi.h())))))));
	}

	bool operator<(const CRoi roi) const	
	{ 
		return ((m_x < roi.x()) || ((m_x == roi.x()) && ((m_y < roi.y()) || ((m_y == roi.y()) && ((m_w < roi.w()) || ((m_w == roi.w()) && (m_h < roi.h())))))));
	}

	bool operator<=(const CRoi roi) const	
	{ 
		return ((m_x < roi.x()) || ((m_x == roi.x()) && ((m_y < roi.y()) || ((m_y == roi.y()) && ((m_w < roi.w()) || ((m_w == roi.w()) && (m_h <= roi.h())))))));
	}
	
	bool operator>=(const CRoi roi) const	
	{ 
		return ((m_x > roi.x()) || ((m_x == roi.x()) && ((m_y > roi.y()) || ((m_y == roi.y()) && ((m_w > roi.w()) || ((m_w == roi.w()) && (m_h >= roi.h())))))));
	}
	
	bool operator==(const CRoi roi) const	
	{ 
		return (m_x == roi.x()) && (m_y == roi.y()) && (m_w == roi.w()) && (m_h == roi.h());
	}

	///////////////////////////////////////////////////////////////////////////////
	// Initialization/Finalization
	///////////////////////////////////////////////////////////////////////////////
	void Reset()
	{
		m_x = m_y = m_w = m_h = 0;						// Image coordinates
		m_xs= m_ys= m_ws= m_hs= 0;						// ImagePiramyd slice coordinates
		m_scaleIndex=0;	m_s=1;							// ImagePiramid slice index and scale
		m_timeIndex=0;									// ImagePiramid time index 
		m_xw= m_yw= m_zw= m_ww= m_hw= m_dw = 0;			// World coordinates (Stereo)
		m_label=0;	m_confidence=0.0f;					// Label and confidence
		m_obligatory = true;							// Indicates if it is mandatory to detect (For annotations)
		m_stdDev = -1.0f;								// StdDev of the ROI
		m_class=CT_Unknown; m_view=VT_Unknown; 
		m_visibility=VBT_Unknown; m_scene=ST_Unknown; m_pose=PT_Unknown;
		m_selected = false;
		m_trackerId=0; m_trackerActive=false; m_trackerAge=0; m_trackerNumSkips=0; m_velocityX=0; m_velocityY=0;
	}

	void Copy(const CRoi& roi)
	{
		m_x=roi.x(); m_y=roi.y(); m_w=roi.w(); m_h=roi.h(); m_timeIndex=roi.timeIndex();
		m_xs=roi.xs(); m_ys=roi.ys(); m_ws=roi.ws(); m_hs=roi.hs(); m_scaleIndex=roi.scaleIndex(); m_s=roi.s();
		m_xw=roi.xw(); m_yw=roi.yw(); m_zw=roi.zw(); m_ww=roi.ww(); m_hw=roi.hw(); m_dw=roi.dw();
		m_confidence=roi.confidence(); m_label = roi.label();	m_stdDev=roi.stdDev(); m_obligatory = roi.obligatory();
		m_class=roi.Class(); m_view=roi.view(); m_visibility=roi.visibility(); m_scene=roi.scene(); m_pose=roi.pose();
		m_selected= roi.selected();
		m_trackerId=roi.trackerId(); m_trackerActive=roi.trackerActive(); m_trackerAge=roi.trackerAge(); m_trackerNumSkips=roi.trackerNumSkips(); m_velocityX=roi.velocityX(); m_velocityY=roi.velocityY();
		m_dispR = roi.dispR(); m_dispT = roi.dispT();
	}

	void SetImageCoordinates(const int x, const int y, const int w, const int h)
	{ 
		m_x = x;	m_y = y;	m_w = w;	m_h = h;
		m_xs = x;	m_ys = y;	m_ws = w;	m_hs = h;
		m_s = 1;	m_scaleIndex = 0;	m_timeIndex = 0;
	}

	void SetImageCoordinates(const int xs, const int ys, const int ws, const int hs, const int scaleIndex,
								   const float scaleFactor, const int x, const int y, const int w, const int h)
	{
		m_x = x;	m_y = y;	m_w = w;	m_h = h;
		m_xs = xs;	m_ys = ys;	m_ws = ws;	m_hs = hs;
		m_s = scaleFactor;	m_scaleIndex = scaleIndex;	m_timeIndex = 0;
	}

	void SetWorldCoordinates(const float xw, const float yw, const float zw, const float ww, const float hw, const float dw)
	{
		m_xw = xw; m_yw = yw; m_zw = zw; m_ww = ww; m_hw = hw; m_dw = dw;
	}



	///////////////////////////////////////////////////////////////////////////////
	// GETS
	///////////////////////////////////////////////////////////////////////////////
	// Corners coordenates in original image
	int			x1				() const	{ return int(Round_(m_x-m_w*0.5f));		}
	int			x2				() const	{ return int(Round_(m_x+m_w*0.5f-1.0f));	}
	int			y1				() const	{ return int(Round_(m_y-m_h*0.5f));		}
	int			y2				() const	{ return int(Round_(m_y+m_h*0.5f-1.0f));	}

	// Corner coordenates in scaled image
	int			xs1				() const	{ return int(Round_(m_xs-m_ws*0.5f));		}
	int			xs2				() const	{ return int(Round_(m_xs+m_ws*0.5f-1.0f));}
	int			ys1				() const	{ return int(Round_(m_ys-m_hs*0.5f));		}
	int			ys2				() const	{ return int(Round_(m_ys+m_hs*0.5f-1.0f));}

	// Central coordenates in original image
	int			x				() const	{ return m_x;	}
	int			y				() const	{ return m_y;	}
	int			w				() const	{ return m_w;	}
	int			h				() const	{ return m_h;	}

	// Central coordenates in scaled image
	int			xs				() const	{ return m_xs;	}
	int			ys				() const	{ return m_ys;	}
	int			ws				() const	{ return m_ws;	}
	int			hs				() const	{ return m_hs;	}

	// World coordenates in original image
	float			xw				() const	{ return m_xw;	}
	float			yw				() const	{ return m_yw;	}
	float			zw				() const	{ return m_zw;	}
	float			ww				() const	{ return m_ww;	}
	float			hw				() const	{ return m_hw;	}
	float			dw				() const	{ return m_dw;	}

	// Scaled parametes
	float			s				() const	{ return m_s;			}
	int				scaleIndex		() const	{ return m_scaleIndex;	}

	// Time parametes
	int				timeIndex		() const	{ return m_timeIndex;	}

	// Labels, confidences and scores
	char			label			() const	{ return m_label;		}
	float			confidence		() const	{ return m_confidence;	}
	float			stdDev			() const	{ return m_stdDev;		}

	// Tracking
	unsigned int	trackerId		() const	{ return m_trackerId;	}
	bool			trackerActive	() const	{ return m_trackerActive;	}
	unsigned short	trackerAge		() const	{ return m_trackerAge;	}
	unsigned char	trackerNumSkips	() const	{ return m_trackerNumSkips;}
	float			velocityX		() const	{ return m_velocityX;	}
	float			velocityY		() const	{ return m_velocityY;	}

	// Annotation atributes
	bool			selected		() const	{ return m_selected;	}

	// Tags
	bool			obligatory		() const	{ return m_obligatory;	}
	classType		Class			() const	{ return m_class;		}
	classType*		pClass			()			{ return &m_class;		}
	viewType		view			() const	{ return m_view;		}
	visibilityType	visibility		() const	{ return m_visibility;	}
	sceneType		scene			() const	{ return m_scene;		}
	poseType		pose			() const	{ return m_pose;		}

	// Disparity
	float			dispT			() const	{ return m_dispT;	}
	float			dispR			() const	{ return m_dispR;	}
	

	///////////////////////////////////////////////////////////////////////////////
	// SETS
	///////////////////////////////////////////////////////////////////////////////
	// Central coordenates in original image
	void set_x				(const int value)				{ m_x = value;			}
	void set_y				(const int value)				{ m_y = value;			}
	void set_w				(const int value)				{ m_w = value;			}
	void set_h				(const int value)				{ m_h = value;			}
		
	// Central coordenates in scaled image
	void set_xs				(const int value)				{ m_xs = value;			}
	void set_ys				(const int value)				{ m_ys = value;			}
	void set_ws				(const int value)				{ m_ws = value;			}
	void set_hs				(const int value)				{ m_hs = value;			}
		
	// World coordenates in original image
	void set_xw				(const float value)				{ m_xw = value;			}
	void set_yw				(const float value)				{ m_yw = value;			}
	void set_zw				(const float value)				{ m_zw = value;			}
	void set_ww				(const float value)				{ m_ww = value;			}
	void set_hw				(const float value)				{ m_hw = value;			}
	void set_dw				(const float value)				{ m_dw = value;			}

	// Scaled parametes
	void set_s				(const float value)				{m_s = value;			}
	void set_scaleIndex		(const int value)				{m_scaleIndex = value;	}

	// Time parameters
	void set_timeIndex		(const int value)				{m_timeIndex = value;	}

	// Labels, confidences and scores
	void set_label			(const char value)				{ m_label = value;		}
	void set_confidence		(const float value)				{ m_confidence = value;	}
	void set_stdDev			(const float value)				{ m_stdDev = value;		}

	// Tracking
	void set_trackerId		(const unsigned int value)		{ m_trackerId = value;	}
	void set_trackerActive	(const bool value)				{ m_trackerActive = value;	}
	void set_trackerAge		(const unsigned short value)	{ m_trackerAge = value;	}
	void set_trackerNumSkips(const unsigned char value)		{ m_trackerNumSkips = value;}
	void set_velocityX		(const float value)				{ m_velocityX = value;	}
	void set_velocityY		(const float value)				{ m_velocityY = value;	}

	// Annotation atributes
	void set_selected		(const bool value)				{ m_selected = value;	}

	// Tags
	void set_obligatory		(const bool value)				{ m_obligatory = value;	}
	void set_class			(const classType value)			{ m_class = value;		}
	void set_view			(const viewType value)			{ m_view = value;		}
	void set_visibility		(const visibilityType value)	{ m_visibility = value;	}
	void set_scene			(const sceneType value)			{ m_scene = value;		}
	void set_pose			(const poseType value)			{ m_pose = value;		}

	// Disparity
	void set_dispT			(float value)					{ m_dispT = value;	}
	void set_dispR			(float value)					{ m_dispR = value;	}

	///////////////////////////////////////////////////////////////////////////////
	// Functions
	///////////////////////////////////////////////////////////////////////////////
	void DeleteMargin(const float percOfMargin)
	{
		m_h  = int(m_h - 2*m_w*percOfMargin);		m_w  = int(m_w - 2*m_w*percOfMargin);
		m_hs = int(m_hs - 2*m_ws*percOfMargin);		m_ws = int(m_ws - 2*m_ws*percOfMargin);
		m_hw = m_hw - 2*m_ww*percOfMargin;			m_ww = m_ww - 2*m_ww*percOfMargin;
	}
	
	void DeleteMarginCVC(const float percOfMargin)
	{
		m_h  = int(m_h - 2*m_h*percOfMargin);		m_w  = int(m_w - 2*m_w*percOfMargin);
		m_hs = int(m_hs - 2*m_hs*percOfMargin);		m_ws = int(m_ws - 2*m_ws*percOfMargin);
		m_hw = m_hw - 2*m_hw*percOfMargin;			m_ww = m_ww - 2*m_ww*percOfMargin;
	}

	void DeleteMarginVARIABLE(const float percOfMarginX, const float percOfMarginY)
	{
		m_h  = int(m_h - 2*m_h*percOfMarginY);			m_w  = int(m_w - 2*m_w*percOfMarginX);
		m_hs = int(m_hs - 2*m_hs*percOfMarginY);		m_ws = int(m_ws - 2*m_ws*percOfMarginX);
		m_hw = m_hw - 2*m_hw*percOfMarginY;				m_ww = m_ww - 2*m_ww*percOfMarginX;
	}

	void AddMargin(const float percOfMargin)
	{
		m_w  = int(m_w /(1-2*percOfMargin));		m_h   = int(m_h + 2*m_w*percOfMargin);
		m_ws = int(m_ws/(1-2*percOfMargin));		m_hs  = int(m_hs + 2*m_ws*percOfMargin);
		m_ww = m_ww/(1-2*percOfMargin);				m_hw  = m_hw + 2*m_ww*percOfMargin;
	}

	void AddMarginCVC(const float percOfMargin)
	{
		m_h  = int(m_h /(1-2*percOfMargin));		m_w  = int(m_w /(1-2*percOfMargin));
		m_hs = int(m_hs/(1-2*percOfMargin));		m_ws = int(m_ws/(1-2*percOfMargin));
		m_hw = m_hw/(1-2*percOfMargin);				m_ww = m_ww/(1-2*percOfMargin);
	}

	void AddMarginVARIABLE(const float percOfMarginX, const float percOfMarginY)
	{
		m_h  = int(m_h /(1-2*percOfMarginY));		m_w  = int(m_w /(1-2*percOfMarginX));
		m_hs = int(m_hs/(1-2*percOfMarginY));		m_ws = int(m_ws/(1-2*percOfMarginX));
		m_hw = m_hw/(1-2*percOfMarginY);				m_ww = m_ww/(1-2*percOfMarginX);
	}
	
	void FixAspectRatio(const float AR)
	{
		m_w  = int(m_h * AR);
		m_ws = int(m_hs * AR);
		m_ww = m_hw * AR;
	}

	void Shift(const int offsetX, const int offsetY, const int offsetScaleX, const int offsetScaleY)
	{
		//if (offsetScaleX!=0.0f || offsetScaleY!=0.0f)
			ShiftInScale(offsetScaleX, offsetScaleY);
		//if (offsetX!=0.0f || offsetY!=0.0f)
			ShiftInOriginal(offsetX, offsetY);
	}

	void ShiftInScale(const int offsetX, const int offsetY)
	{
		m_xs = m_xs-offsetX;	m_ys = m_ys-offsetY;
		m_x  = int(m_xs*m_s);	m_y  = int(m_ys*m_s);
	}

	void ShiftInOriginal(const int offsetX, const int offsetY)
	{
		m_x = m_x-offsetX;		m_y	= m_y-offsetY;
		m_xs= int(m_x/m_s);		m_ys= int(m_y/m_s);
	}

	void UnShift(const int offsetX, const int offsetY, const int offsetScaleX, const int offsetScaleY)
	{
		UnShiftInScale(offsetScaleX, offsetScaleY);
		UnShiftInOriginal(offsetX, offsetY);
	}

	void UnShiftInScale(const int offsetX, const int offsetY)
	{
		m_xs = m_xs+offsetX;	m_ys = m_ys+offsetY;
		m_x  = int(m_xs*m_s);	m_y  = int(m_ys*m_s);
	}

	void UnShiftInOriginal(const int offsetX, const int offsetY)
	{
		m_x = m_x+offsetX;		m_y	= m_y+offsetY;
		m_xs= int(m_x/m_s);		m_ys= int(m_y/m_s);
	}

	////void Resize(const float scaleX, const float scaleY)
	////{
	////	m_x = (m_x*scaleX; m_y = m_y*scaleY;
	////	m_w = m_w*scaleX; m_h = m_h*scaleY;
	////	m_xs = m_xs*scaleX; m_ys = m_ys*scaleY;
	////	m_ws = m_ws*scaleX; m_hs = m_hs*scaleY;
	////}

// ATRIBUTES
public:
	vector<float>	m_scores;								///< Scores for each feature

private:
	// Coordinates
	int				m_x, m_y, m_w, m_h;						///< Image coordinates
	int				m_xs, m_ys, m_ws, m_hs;					///< ImagePiramyd slice coordinates
	float			m_s;									///< ImagePiramyd slice scale
	int				m_scaleIndex;							///< ImagePiramyd slice index
	int				m_timeIndex;							///< ImagePiramyd time index
	float			m_xw, m_yw, m_zw, m_ww, m_hw, m_dw;		///< World coordinates (Stereo)

	// Labels, confidences and others
	char			m_label;								///< Label: [-1|0|1] = [negative|unknown|positive]
	float			m_confidence;							///< Confidence for the ROI
	float			m_stdDev;								///< Standard Deviation of the ROI image

	// Tracking
	unsigned int	m_trackerId;							///< Tracker Id number
	bool			m_trackerActive;						///< Is the tracker active
	unsigned short	m_trackerAge;							///< Tracker age in frames
	unsigned char	m_trackerNumSkips;						///< Tracker number of frame measurement skips
	float			m_velocityX;							///< Tracker velocity in X direction
	float			m_velocityY;							///< Tracker velocity in Y direction

	// Annotation atributtes
	bool			m_selected;								///< Flag used by AnnotationTool

	// Tags
	bool			m_obligatory;							///< Indicates if it is mandatory to detect (For annotations)
	classType		m_class;								///< 
	viewType		m_view;									///< 
	visibilityType	m_visibility;							///< 
	sceneType		m_scene;								///<
	poseType		m_pose;									///<

	//
	float			m_dispR;								///< Mean disparity of center area of ROI
	float			m_dispT;								///< Theorical disparity of ROI
};
#endif // _ROI_
