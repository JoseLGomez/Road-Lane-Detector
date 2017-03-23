/* ********************************* FILE ************************************/
/** @file		Color.h
 *	@ingroup	Image
 * 
 *	@brief		This file describes the declaration of the class CColor
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Oct 09, 2011
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 ******************************************************************************/
#ifndef _COLOR_
#define _COLOR_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/
#define OPP1Offset 182
#define OPP2Offset 209
#define LSLM2Offset 236
#define LSLM3Offset 803
#define L1L2L3Offset 128
#define YUV2Offset 112
#define YUV3Offset 157
#define YCBCROffset 128
#define YIQ1Offset 152
#define YIQ2Offset 134

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/// Color types
enum EColorType { ect_unknown=-1, ect_rgb, ect_opp, ect_gray, ect_CMY, ect_YUV, ect_YCBCR, ect_YIQ, ect_L1L2L3, ect_LSLM, ect_W, ect_HSV, ect_HSVCC};



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	Image
 *	@brief		It encapsulates the color of a pixel in an object
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Oct 09, 2011
 *	\sa			-
 *****************************************************************************/
class CColor
{
// Methods
public:

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CColor(const int r_in=0, const int g_in=0, const int b_in=0, const float a_in=1.0f):
		r(r_in),
		g(g_in),
		b(b_in),
		a(a_in)
	{} 
	~CColor() {}
	///@}

// Atributes
public:
	int		r;		///< Red value
	int		g;		///< Green value
	int		b;		///< Blue value
	float	a;		///< Aplha value
};

#endif // _COLOR_