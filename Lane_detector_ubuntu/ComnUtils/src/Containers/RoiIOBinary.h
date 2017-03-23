/* ********************************* FILE ************************************/
/** @file		RoiIOBinary.h
 *
 *	@ingroup	
 * 
 *	@brief		This file describes the declaration of the class CRoiIOBin
 * 
 *	@author		
 *
 *	@date		
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _ROI_IO_BINARY_
#define _ROI_IO_BINARY_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include "Containers/Roi.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

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
 *	@ingroup	
 *	@brief		It defines the class CRoiIOBin
 *
 *	@author		
 *
 *	@date		
 *	\sa			-
 *****************************************************************************/
class CRoiIOBinary
{
private:

	struct VideoData
	{
		unsigned int		m_nFrames;
		unsigned int		m_fpsRois;
		VideoData	(const unsigned int nFrames, const unsigned int fpsRois)	:	
			m_nFrames(nFrames), m_fpsRois(fpsRois)	{}
		VideoData	()								:	m_nFrames(0), m_fpsRois(1)		{}
	};

	struct FrameData
	{
		unsigned int		m_frameId;
		unsigned int		m_nObject;
		FrameData	(const unsigned int frameId, const unsigned int nObject)	:	
			m_frameId(frameId), m_nObject(nObject)	{}
		FrameData	()	:	
			m_frameId(0), m_nObject(0)				{}
	};

	struct ObjectData
	{
		int		m_x, m_y, m_width, m_height;
		float	m_confidence;
		ObjectData	(const int x, const int y, const int width, const int height, const float confidence)	:
			m_x(x), m_y(y), m_width(width), m_height(height), m_confidence(confidence) {}
		ObjectData	()	:
			m_x(0), m_y(0), m_width(0), m_height(0), m_confidence(0.0f) {}
		ObjectData	(const CRoi* pRoi) : 
			m_x(pRoi->x()), m_y(pRoi->y()), m_width(pRoi->w()), m_height(pRoi->h()), m_confidence(pRoi->confidence()) {}
		CRoi*	GetRoi() const	{ return new CRoi(m_x, m_y, m_width, m_height, m_confidence);	}
	};

	struct Frame
	{
		FrameData			m_frameData;
		vector<ObjectData>	m_vObjects;
		Frame	(const FrameData& frameData, const vector<ObjectData>& vObjects)	:	
			m_frameData	(frameData), m_vObjects(vObjects)	{}
		Frame	()	:	
            m_frameData	(FrameData(0,0))	{}
		Frame	(const unsigned int frameId, const vector<CRoi*> vRois)	:	
			m_frameData (FrameData(frameId, (unsigned int)vRois.size()))
		{
			m_vObjects.reserve(vRois.size());
			for (unsigned int i=0; i<vRois.size(); i++)
				m_vObjects.push_back(ObjectData(vRois[i]));
		}

		unsigned int	GetNObjets		() const						{ return (unsigned int)m_vObjects.size();	}
		FrameData&		GetFrameData	()						{ return m_frameData;		}
		const ObjectData&		GetObjectData	(unsigned int i) const		{ return m_vObjects[i];		}
		void			GetObjects		(vector<CRoi*>& vRois) const
		{
			vRois.clear();
			vRois.reserve(m_vObjects.size());
			for (size_t i=0; i<m_vObjects.size(); i++)
				vRois.push_back(m_vObjects[i].GetRoi());
		}
	};

	struct Video
	{
		VideoData			m_videoData;
		vector<Frame>		m_vFrames;
		Video	(const VideoData videoData, const vector<Frame> vFrames)	:
			m_videoData(videoData), m_vFrames(vFrames)	{}
		Video	()	:	
            m_videoData(VideoData())	{}
		unsigned int	GetNFrames		()	const								{ return (unsigned int)m_vFrames.size();				}
		unsigned int	GetFrameID		(const int pos)	const					{ return m_vFrames[pos].m_frameData.m_frameId;			}
		unsigned int	GetFpsRois		()	const								{ return m_videoData.m_fpsRois;							}
		void			SetFpsRois		(const unsigned int fpsRois)		{ m_videoData.m_fpsRois = fpsRois;						}
		void			SetFrame		(const int pos, const Frame rect)	{ m_vFrames[pos] = rect;				}
		void			SetFrame		(const int pos, const vector<CRoi*>& rect)	{ m_vFrames[pos] = Frame(m_videoData.m_fpsRois-1+(pos*m_videoData.m_fpsRois),rect); }
		void			SetFrame		(const int pos, const vector<CRoi*>& rect, const int frameId)	{ m_vFrames[pos] = Frame(frameId,rect); }
		const Frame&			GetFrame		(const unsigned int nFrame)	const		{ return m_vFrames[nFrame];				}
		void			GetFrame		(const unsigned int nFrame, vector<CRoi*>& vRois)	const	{ if (nFrame<m_vFrames.size())m_vFrames[nFrame].GetObjects(vRois);	}
		VideoData&		GetVideoData	()									{ return m_videoData;					}
		FrameData&		GetFrameData	(const unsigned int i)				{ return m_vFrames[i].GetFrameData();	}
		void			AddFrame		(const vector<CRoi*>& vRois)		{ m_videoData.m_nFrames++; m_vFrames.push_back(Frame(m_videoData.m_nFrames*m_videoData.m_fpsRois-1, vRois));	}
		void			AddFrame		(const vector<CRoi*>& vRois, const unsigned int id)	{ m_videoData.m_nFrames++; m_vFrames.push_back(Frame(id, vRois));	}
		void			InsertFrame		(const vector<CRoi*>& value, const int pos, const int frameID) { m_videoData.m_nFrames++; m_vFrames.insert(m_vFrames.begin()+pos,Frame(frameID, value)); }
		void			AddFrame		(const Frame frame)
		{
			m_videoData.m_nFrames++;
			m_vFrames.push_back(frame);
		}
		void			EraseFrame		(const unsigned int i)
		{
			m_videoData.m_nFrames--;
			m_vFrames.erase(m_vFrames.begin()+i);

		}
		void			GetFrameWithID		(const unsigned int i, vector<CRoi*>& vRois) const
		{
			vRois = vector<CRoi*>();
			bool flag_OK = false;
			int cont = 0;
			while (cont < (int)m_videoData.m_nFrames && !flag_OK)
			{
				if (m_vFrames[cont].m_frameData.m_frameId == i)
				{
					GetFrame(cont, vRois);
					flag_OK = true;
				}
				cont++;
			}
		}
		void			UpdateRois		(const vector<CRoi*> value, const unsigned int pos)
		{
            for (unsigned int i = 0; i < value.size(); i++)
			{
				bool flag = false;
				for (unsigned int j = 0; j < m_vFrames[pos].GetNObjets(); j++)
				{
					if (value[i]->w() == m_vFrames[pos].m_vObjects[j].m_width &&
						value[i]->h() == m_vFrames[pos].m_vObjects[j].m_height &&
						value[i]->x() == m_vFrames[pos].m_vObjects[j].m_confidence &&
						value[i]->y() == m_vFrames[pos].m_vObjects[j].m_x &&
						value[i]->confidence() == m_vFrames[pos].m_vObjects[j].m_y)
					{
						flag = true;
						break;
					}
				}
				if (!flag)
				{
					m_vFrames[pos].m_vObjects.push_back(value[i]);
					m_vFrames[pos].m_frameData.m_nObject++;
				}
			}
		}
	};


// METHODS
public:
 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Constructors and destructors
	///@{
    CRoiIOBinary					() { m_video = Video(); }
    ~CRoiIOBinary					() {}
	///@}

 	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Initializes and finalizes
	///@{
	void Initialize					();
	void Finish						();
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Computational methods
	///@{
	bool			Read			(const string& fileName);
	bool			Write			(const string& fileName);
	void			AddFrame		(const vector<CRoi*>& vRois)						{ m_video.AddFrame(vRois);			}
	void			AddFrame		(const vector<CRoi*>& vRois, const unsigned int id)	{ m_video.AddFrame(vRois,id);		}
	void			GetFrame		(const unsigned int i, vector<CRoi*>& vRois)	const	{ m_video.GetFrame(i, vRois);		}
	void			GetFrameWithID	(const unsigned int i, vector<CRoi*>& vRois)	const	{ m_video.GetFrameWithID(i, vRois);	}
	void			SetFrame		(const unsigned int i, const vector<CRoi*>& vRois)	{ m_video.SetFrame(i,vRois);		}
	void			SetFrame		(const unsigned int i, const vector<CRoi*>& vRois, const unsigned int frameId)	{ m_video.SetFrame(i,vRois,frameId);		}
	unsigned int	GetNFrames		()	const												{ return m_video.GetNFrames();		}
	int				GetFrameID		(const int pos)		const								{ return m_video.GetFrameID(pos);	}
	void			EraseFrame		(const unsigned int i)								{ m_video.EraseFrame(i);			}
	unsigned int	GetFpsRois		()			const										{ return m_video.GetFpsRois();		}
	void			SetFpsRois		(const unsigned int fpsRois)						{ m_video.SetFpsRois(fpsRois);		}
	void			Insert			(const vector<CRoi*>& value, const int pos, const int frameID)			{ m_video.InsertFrame(value,pos,frameID); }				
	void			UpdateRois		(const vector<CRoi*>& value, const unsigned int pos) { m_video.UpdateRois(value,pos); }
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Get methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Set methods
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Other methods
	///@{
	///@}


private:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private constructors
	///@{
	CRoiIOBinary(const CRoiIOBinary& other);
	CRoiIOBinary& operator=(const CRoiIOBinary& other);
	///@}
	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	///@name Private methods
	///@{
	///@}

// ATRIBUTES
private:
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	///@name Parameters from constructor
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	///@name Parameters from INI
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	///@name State variables
	///@{
	///@}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	///@name Auxiliar variables
	///@{
	Video				m_video;			///<  
	///@}
};
#endif // _ROI_IO_BINARY_
