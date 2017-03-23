/* ********************************* FILE ************************************/
/** @file		Sample.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class CSample
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _SAMPLE_
#define _SAMPLE_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "../Utils/Utils.h"
#include "../Containers/Roi.h"

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
template<typename T> class CImageG;
typedef CImageG<float> CImageFloat;



/* ******************************** CLASS ************************************/
/**
 *	@ingroup	ComnUtils
 *	@brief		It defines the class CSample
 *
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class CSample : public CRoi
{
// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
	CSample();
	CSample(const string& imageFileName, const string& featuresFileName, const int id);
    ~CSample()
    {
        Finish();
    }
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize(const string& imageFileName, const string& featuresFileName, const int id);
	void Finish();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	///@}
	
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Functions:
	///@{
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	void SetImageFileName	(const string& value);
	void SetFeaturesFileName(const string& value);
	void SetId				(const int value)		{m_id = value;}
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	CImageFloat*GetImage				()	const { return m_pImage;				}
	int			GetId					()	const { return m_id;					}

	string		GetImageFullFileName	()	const { return m_imageFullFileName;		}
	string		GetImagePath			()	const { return m_imagePath;				}
	string		GetImageFileName		()	const { return m_imageFileName;			}
	string		GetImageRawFileName		()	const { return m_imageRawFilename;		}
	string		GetImageExtension		()	const { return m_imageExtension;		}

	string		GetFeaturesFullFileName	()	const { return m_featuresFullFileName;	}
	string		GetFeaturesPath			()	const { return m_featuresPath;			}
	string		GetFeaturesFileName		()	const { return m_featuresFileName;		}
	string		GetFeaturesRawFileName	()	const { return m_featuresRawFilename;	}
	string		GetFeaturesExtension	()	const { return m_featuresExtension;		}
	///@}
	
private:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	//CSample(const CSample& other);
	CSample& operator=(const CSample& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	///@}

// ATRIBUTES
private:
	// Variables
	string			m_imageFullFileName;		///< D:/ddbb/inria/train/positives/image01.png
	string			m_imagePath;				///< D:/ddbb/inria/train/positives/
	string			m_imageFileName;			///< image01.png
	string			m_imageRawFilename;			///< image01
	string			m_imageExtension;			///< png
	
	// Unique Identifier
	int				m_id;						///< identifier

	// Features file
	string			m_featuresFullFileName;		///< D:/Exp1/output/HaarEOH/1/image01.dat
	string			m_featuresPath;				///< D:/Exp1/output/HaarEOH/1/
	string			m_featuresFileName;			///< image01.dat
	string			m_featuresRawFilename;		///< image01
	string			m_featuresExtension;		///< dat

	//Image
	CImageFloat*	m_pImage;					///< Pointer to the image
};
#endif // _SAMPLE_
