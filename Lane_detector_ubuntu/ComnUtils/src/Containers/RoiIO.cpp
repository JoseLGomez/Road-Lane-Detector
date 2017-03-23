#include "RoiIO.h"

#include "Image.h"
#include "ImageConversion.h"
#include "ImageProcessing.h"
#include "StereoCamera.h"
#include "../Utils/Utils.h"
#include "../Maths/MersenneTwister.h"
#include "../Maths/Maths.h"
#include "opencv2/opencv.hpp"
using namespace cv;

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Default constructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CRoiIO::CRoiIO()
{
}


/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CRoiIO::~CRoiIO()
{
}



///////////////////////////////////////////////////////////////////////////////
// Functions: General Read, Write and Filters
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Reads the given file.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	 	Filename of the file.
 * @param	type		 	The type.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::Read(const string& filename, const ERoiIOType type, vector<CRoi*>& vRois)
{
	switch (type)
	{
		case eriot_confidences:
			ReadConfidences(filename, vRois);
		break;
		case eriot_rois:
			ReadAnnotations(filename, vRois);
		break;
		case eriot_disparity:
			ReadDisparities(filename,vRois);
		break;
		case eriot_unknown:
		default:
			ErrorQuit(VSys_UnkFormat, "");
		break;
	};
}


/**************************************************************************//**
 * @brief	Writes the given file.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	Filename of the file.
 * @param	type		The type.
 * @param	vRois   	The rois.
 *****************************************************************************/
void CRoiIO::Write(const string& filename, const ERoiIOType type, const vector<CRoi*>& vRois, const string& object)
{
	switch (type)
	{
		case eriot_confidences:
			WriteConfidences(filename, vRois);
		break;
		case eriot_rois:
			WriteAnnotations(filename, vRois);
		break;
		case eriot_evaluationCaltech:
			WriteEvaluationCaltech(filename, vRois);
			break;
		case eriot_evaluationCaltechViewpoint:
			WriteEvaluationCaltech(filename, vRois, true);
			break;
		case eriot_disparity:
			WriteDisparities(filename, vRois);
			break;
		case eriot_evaluationKitti:
			WriteEvaluationKitti(filename, vRois, object);
			break;
		case eriot_unknown:
		default:
			ErrorQuit(VSys_UnkFormat, "");
		break;
	};
}


/**************************************************************************//**
 * @brief	Reads the annotations.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	 	Filename of the file.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::ReadAnnotations(const string& filename, vector<CRoi*>& vRois)
{
	// 0. Auxiliar variables
	char str[1024];
	vector<string> tokens;
	vRois.clear();

	// 1. Open the file
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp==NULL)
	{
		//ErrorQuit(VSys_FileNotFound, filename);
		VSysError(VSys_FileNotFound, filename);
		return;
	}

	// 2. Read the data
	while(fgets (str, 1024, fp)!=NULL)
	{
		// 2.1 Init default parameters
		float x=0.0f, y=0.0f, w=0.0f, h=0.0f, conf=0.0f, wx=0.0f, wy=0.0f, wz=0.0f, ww=0.0f, wh=0.0f, wd=0.0f;
		int a=1, b=CT_Pedestrian, c=VT_Front, d=VBT_FullyVisible, e=ST_Street, f=0, g=PT_Stand;

		// 2.2 Tokenize
		Tokenize(str, tokens, " ");
		if (tokens.size()>=4)
		{
			// Extract tokens
			x = (float)atof(tokens[0].c_str());
			y = (float)atof(tokens[1].c_str());
			w = (float)atof(tokens[2].c_str());
			h = (float)atof(tokens[3].c_str());
			if (tokens.size()>4)
			{
				if (strncmp(tokens[4].c_str(), "PEDESTRIAN-OBLIGATORY", 20)==0||strncmp(tokens[4].c_str(), "1",1)==0)
					a = 1;
				else
					a = 0;
			}
			if (tokens.size()>5) conf = (float)atof(tokens[5].c_str());
			if (tokens.size()>6) b = atoi(tokens[6].c_str());
			if (tokens.size()>7) c = atoi(tokens[7].c_str());
			if (tokens.size()>8) d = atoi(tokens[8].c_str());
			if (tokens.size()>9) e = atoi(tokens[9].c_str());
			if (tokens.size()>10) f = atoi(tokens[10].c_str());
			if (tokens.size()>11) g = atoi(tokens[11].c_str());
			if (tokens.size()>12) wx = (float)atof(tokens[12].c_str());
			if (tokens.size()>13) wy = (float)atof(tokens[13].c_str());
			if (tokens.size()>14) wz = (float)atof(tokens[14].c_str());
			if (tokens.size()>15) ww = (float)atof(tokens[15].c_str());
			if (tokens.size()>16) wh = (float)atof(tokens[16].c_str());
			if (tokens.size()>17) wd = (float)atof(tokens[17].c_str());

			// Set the ROI
			CRoi* pRoi = new CRoi((int)x, (int)y, (int)w, (int)h);
			pRoi->set_confidence(conf);
			pRoi->set_obligatory(a!=0);
			pRoi->set_class((classType)b);
			pRoi->set_view((viewType)c);
			pRoi->set_visibility((visibilityType)d);
			pRoi->set_scene((sceneType)e);
			pRoi->set_trackerId(f);
			pRoi->set_pose((poseType)g);
			pRoi->SetWorldCoordinates(wx,wy,wz,ww,wh,wd);

			// Save the ROI
			vRois.push_back(pRoi);
		}
	}

	// 3. Close the file
	fclose(fp);
}


/**************************************************************************//**
 * @brief	Writes the annotations.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	Filename of the file.
 * @param	vRois   	The rois.
 *****************************************************************************/
void CRoiIO::WriteAnnotations(const string& filename, const vector<CRoi*>& vRois)
{
	// 1. Open the file
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 3. Write the ROIs
	for (unsigned int i=0; i<vRois.size(); i++)
		Fprintf(fp, "%d %d %d %d %d %.6f %d %d %d %d %d %d %.6f %.6f %.6f %.6f %.6f %.6f \n", vRois[i]->x(), vRois[i]->y(), vRois[i]->w(), vRois[i]->h(), (int)vRois[i]->obligatory(), vRois[i]->confidence(), (int)vRois[i]->Class(), (int)vRois[i]->view(), (int)vRois[i]->visibility(), (int)vRois[i]->scene(), (int)vRois[i]->trackerId(), (int)vRois[i]->pose(), vRois[i]->xw(), vRois[i]->yw(), vRois[i]->zw(), vRois[i]->ww(), vRois[i]->hw(), vRois[i]->dw());
		
	// 4. Close the file
	fclose(fp);
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Confidences
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Reads the confidences.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename				Filename of the file.
 * @param [in,out]	vConfidences	The confidences.
 *****************************************************************************/
void CRoiIO::ReadConfidences(const string& filename,  vector<float>& vConfidences)
{
	// 0. Auxiliar variables
	float c;
	vConfidences.clear();

	// 1. Open the file
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 2. Read the data
	while(fscanf(fp, "%f", &c) == 1)
	{
		vConfidences.push_back(c);
		fscanf(fp, "\n");
	}

	// 3. Close the file
	fclose(fp);
}


/**************************************************************************//**
 * @brief	Writes the confidences.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename		Filename of the file.
 * @param	vConfidences	The confidences.
 *****************************************************************************/
void CRoiIO::WriteConfidences(const string& filename, const  vector<float>& vConfidences)
{
	// 1. Open the file in append mode
	FILE*fp=fopen(filename.c_str(), "w");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 2. Write the confidences
	for (unsigned int i=0; i<vConfidences.size(); i++)
	{
		Fprintf(fp, "%e\n", vConfidences[i]);
	}

	// 3. Close the file
	fclose(fp);
}


/**************************************************************************//**
 * @brief	Reads the confidences.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	 	Filename of the file.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::ReadConfidences(const string& filename, vector<CRoi*>& vRois)
{
	// 0. Auxiliar variables
	float c;
	vRois.clear();

	// 1. Open file
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 2. Read data
	while(fscanf(fp, "%f\n", &c) == 1)
	{
		// Create ROI
		CRoi* pRoi = new CRoi();
		pRoi->set_confidence(c);
		vRois.push_back(pRoi);
	}

	// 3. Close file
	fclose(fp);
}


/**************************************************************************//**
 * @brief	Writes the confidences.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	Filename of the file.
 * @param	vRois   	The rois.
 *****************************************************************************/
void CRoiIO::WriteConfidences(const string& filename, const vector<CRoi*>& vRois)
{
	// 1. Open the file in append mode
	FILE*fp=fopen(filename.c_str(), "a");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 2. Write the confidences
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		Fprintf(fp, "%e\n", vRois[i]->confidence());
	}

	// 3. Close the file
	fclose(fp);
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Remove Filters
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Removes the rois by confidence.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minConfidence	The minimum confidence.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
template <class T>
class RoiMinConf:public std::unary_function<T, bool> 
{
	public:
		RoiMinConf(const float minConf):minConf(minConf) {}
		bool operator()(const T& val) const	{ return (val->confidence()>=minConf); }
	private:
        float minConf;
};

void CRoiIO::ClearRoisConfLabel(vector<CRoi*>& vRois)
{
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		vRois[i]->set_confidence(0);
		vRois[i]->set_label(0);
	}
}


void CRoiIO::RemoveRoisByConfidence(const float minConfidence, vector<CRoi*>& vRois)
{
    vector<CRoi*>::iterator last = std::partition(vRois.begin(), vRois.end(), RoiMinConf<CRoi*>(minConfidence) ) ;
    for(vector<CRoi*>::iterator i=last; i!=vRois.end(); ++i)
        delete *i;
    vRois.erase(last, vRois.end());
}

//void CRoiIO::RemoveRoisByConfidence(const float minConfidence, vector<CRoi*>& vRois)
//{
//    // Copy acepted Rois and delete pointers of declined ones
//    vector<CRoi*> vRoisOut;
//    for (unsigned int i=0; i<vRois.size(); i++)
//    {
//        if (vRois[i]->confidence() > minConfidence)
//            vRoisOut.push_back(vRois[i]);
//        else
//            delete vRois[i];
//    }

//    // Copy acepted Rois
//    vRois.clear();
//    vRois.resize(vRoisOut.size());
//    copy(vRoisOut.begin(), vRoisOut.end(), vRois.begin());
//}


/**************************************************************************//**
 * @brief	Removes the rois by label.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	labelToRemove	The label to remove.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::RemoveRoisByClass(const classType classToRemove, vector<CRoi*>& vRois)
{
	// Copy acepted Rois and delete pointers of declined ones
	vector<CRoi*> vRoisOut;
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->Class() != classToRemove)
			vRoisOut.push_back(vRois[i]);
		else
			delete vRois[i];
	}

	// Copy acepted Rois
	vRois.clear();
	vRois.resize(vRoisOut.size());
	copy(vRoisOut.begin(), vRoisOut.end(), vRois.begin());
}

void CRoiIO::RemoveRoisByClass2(const classType classToRetain, vector<CRoi*>& vRois)
{
	// Copy acepted Rois and delete pointers of declined ones
	vector<CRoi*> vRoisOut;
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->Class() == classToRetain)
			vRoisOut.push_back(vRois[i]);
		else
			delete vRois[i];
	}

	// Copy acepted Rois
	vRois.clear();
	vRois.resize(vRoisOut.size());
	copy(vRoisOut.begin(), vRoisOut.end(), vRois.begin());
}

/**************************************************************************//**
 * @brief	Removes the rois by label.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	labelToRemove	The label to remove.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::RemoveRoisByLabel(const float labelToRemove, vector<CRoi*>& vRois)
{
	// Copy acepted Rois and delete pointers of declined ones
	vector<CRoi*> vRoisOut;
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->label() != labelToRemove)
			vRoisOut.push_back(vRois[i]);
		else
			delete vRois[i];
	}

	// Copy acepted Rois
	vRois.clear();
	vRois.resize(vRoisOut.size());
	copy(vRoisOut.begin(), vRoisOut.end(), vRois.begin());
}

/**************************************************************************//**
 * @brief	Filter rois by label.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	labelToRemove   	The label to remove.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByLabel(const float labelToRemove, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->label() != labelToRemove)
			vRoisOut.push_back(vRois[i]);
	}
}

/**************************************************************************//**
 * @brief	Filter rois by label.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	labelToRemove   	The label to remove.
 * @param	vvRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByLabel(const float labelToRemove, const vector<vector<CRoi*>>& vvRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for(unsigned int j=0;j<vvRois.size();j++)
	for (unsigned int i=0; i<vvRois[j].size(); i++)
	{
		if (vvRois[j][i]->label() != labelToRemove)
			vRoisOut.push_back(vvRois[j][i]);
	}
}

/**************************************************************************//**
 * @brief	Removes the rois by mandatory.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Jul 23, 2012
 *
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::RemoveRoisByMandatory(vector<CRoi*>& vRois)
{
	// Copy acepted Rois and delete pointers of declined ones
	vector<CRoi*> vRoisOut;
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->obligatory())
			vRoisOut.push_back(vRois[i]);
		else
			delete vRois[i];
	}

	// Copy acepted Rois
	vRois.clear();
	vRois.resize(vRoisOut.size());
	copy(vRoisOut.begin(), vRoisOut.end(), vRois.begin());
}


/**************************************************************************//**
 * @brief	Removes the rois by height.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minHeight	 	Height of the minimum.
 * @param	maxHeight	 	Height of the maximum.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::RemoveRoisByHeight(const float minHeight, const float maxHeight, vector<CRoi*>& vRois)
{
	// Copy acepted Rois and delete pointers of declined ones
	vector<CRoi*> vRoisOut;
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if ((vRois[i]->h()>=minHeight) && (vRois[i]->h()<=maxHeight))
			vRoisOut.push_back(vRois[i]);
		else
			delete vRois[i];
	}

	// Copy acepted Rois
	vRois.clear();
	vRois.resize(vRoisOut.size());
	copy(vRoisOut.begin(), vRoisOut.end(), vRois.begin());
}

void CRoiIO::RemoveRoisFromTop(vector<CRoi*> &vRois, const int minY)
{
	vector<CRoi*> vRoisOutput;

	// Find rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->y() >= minY)
		{
			vRoisOutput.push_back(vRois[i]);
			vRois[i] = NULL;
		}
	}

	// Delete unneeded rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]!=NULL)
		{
			delete vRois[i];
			vRois[i]=NULL;
		}
	}

	// Move rois
	vRois.clear();
	vRois.resize(vRoisOutput.size());
	copy(vRoisOutput.begin(), vRoisOutput.end(), vRois.begin());
}

void CRoiIO::RemoveRoisFromBottom(vector<CRoi*> &vRois, const int maxY)
{
	vector<CRoi*> vRoisOutput;

	// Find rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->y() <= maxY)
		{
			vRoisOutput.push_back(vRois[i]);
			vRois[i] = NULL;
		}
	}

	// Delete unneeded rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]!=NULL)
		{
			delete vRois[i];
			vRois[i]=NULL;
		}
	}

	// Move rois
	vRois.clear();
	vRois.resize(vRoisOutput.size());
	copy(vRoisOutput.begin(), vRoisOutput.end(), vRois.begin());
}

template <class T>
class RoiInRange:public std::unary_function<T, bool> 
{
	public:
		RoiInRange(const int minX, const int maxX, const int minY, const int maxY):
			minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}
		bool operator()(const T& val) const	
		{
			return (val->x()>=minX && val->x()<=maxX && val->y()>=minY && val->y()<=maxY);
		}
	private:
		int minX, maxX, minY, maxY;
};

void CRoiIO::RemoveRoisFromRange(vector<CRoi*> &vRois, const int minX, const int maxX, const int minY, const int maxY)
{
	vector<CRoi*>::iterator last = std::partition(vRois.begin(), vRois.end(), RoiInRange<CRoi*>(minX, maxX, minY, maxY) ) ;
	for(vector<CRoi*>::iterator i=last; i!=vRois.end(); ++i)
		delete *i;
	vRois.erase(last, vRois.end());
}

void CRoiIO::RemoveRoisFromRange2(vector<CRoi*> &vRois, const int minX, const int maxX, const int minY, const int maxY)
{
	vector<CRoi*> Filtered_Rois(vRois.size(),NULL);
	int f=0;
	for (int i=0; i<vRois.size(); i++)
	{
		if ((vRois[i]->x()>minX)&&(vRois[i]->x()<maxX)&&(vRois[i]->y()>minY)&&(vRois[i]->y()<maxY))
		{
			Filtered_Rois[f] = new CRoi(*vRois[i]);
			f++;
		}
	}
	
	for (int i=0; i<vRois.size(); i++)
	{
		if (i<f)
			vRois[i]->Copy(*Filtered_Rois[i]);
		else
			delete vRois[i];
	}

	for (int i=0; i<Filtered_Rois.size(); i++) 
		delete Filtered_Rois[i];

	vRois.resize(f);
}



void CRoiIO::RemoveRoisFromRangeAdaptative(vector<CRoi*> &vRois, const float A, const float B, const float C, const int marginY, const float percMargin)
{
	vector<CRoi*> vRoisOutput;

	// Find rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		float y2 = vRois[i]->y2() - (float)marginY - vRois[i]->h()*percMargin;
		float h = vRois[i]->h() - vRois[i]->h()*percMargin*2;
		if (((h*A+B+C)>=y2) && ((h*A+B-C)<=y2))
		{
			vRoisOutput.push_back(vRois[i]);
			vRois[i] = NULL;
		}
	}

	// Delete unneeded rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]!=NULL)
		{
			delete vRois[i];
			vRois[i]=NULL;
		}
	}

	// Move rois
	vRois.clear();
	vRois.resize(vRoisOutput.size());
	copy(vRoisOutput.begin(), vRoisOutput.end(), vRois.begin());
}

void CRoiIO::RemoveRoisFromRangeAdaptative(vector<CRoi*> &vRois, const float A, const float B, const float D1, const float D2, const int marginY, const float percMargin)
{
	vector<CRoi*> vRoisOutput;

	// Find rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		float y2 = vRois[i]->y2() - (float)marginY - vRois[i]->h()*percMargin;
		float h = vRois[i]->h() - vRois[i]->h()*percMargin*2;
		if (((h*A+B+D1)>=y2) && ((h*A+B+D2)<=y2))
		{
			vRoisOutput.push_back(vRois[i]);
			vRois[i] = NULL;
		}
	}

	// Delete unneeded rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]!=NULL)
		{
			delete vRois[i];
			vRois[i]=NULL;
		}
	}

	// Move rois
	vRois.clear();
	vRois.resize(vRoisOutput.size());
	copy(vRoisOutput.begin(), vRoisOutput.end(), vRois.begin());
}

/**************************************************************************//**
 * @brief	Normalize roi aspect ratio.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	AR			 	The archive.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::NormalizeRoiAR(const float AR, vector<CRoi*>& vRois)
{
	// 2.2 Normalize the Rois
	if (AR!=-1)
	{
		for (unsigned int i=0; i<vRois.size(); i++)
		{
			vRois[i]->set_w(int(vRois[i]->h()*AR));
			vRois[i]->set_ws(int(vRois[i]->hs()*AR));
		}
	}
}


/**************************************************************************//**
 * @brief	Removes the rois duplicated.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::RemoveRoisDuplicated(vector<CRoi*>& vRois)
{
	//Sort ROIs
	CRoiIO::Sort(vRois); 

	//Remove duplicated ROIs
	for (int i = 1; i < vRois.size(); i++)
		if (*vRois[i-1] == *vRois[i])
		{
			delete vRois[i];
			vRois.erase(vRois.begin()+i);
			i--;
		}
}


///////////////////////////////////////////////////////////////////////////////
// Functions: Remove Filters
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Filter rois by confidence.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minConfidence   	The minimum confidence.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByConfidence(const float minConfidence, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->confidence() > minConfidence)
			vRoisOut.push_back(vRois[i]);
	}
}

/**************************************************************************//**
 * @brief	Filter rois by confidence.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minConfidence   	The minimum confidence.
 * @param	maxConfidence   	The maximum confidence.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByConfidence(const float minConfidence, const float maxConfidence,const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->confidence() > minConfidence&&vRois[i]->confidence() < maxConfidence)
			vRoisOut.push_back(vRois[i]);
	}
}

/**************************************************************************//**
 * @brief	Filter rois by label.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	labelToRemove   	The label to remove.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByClass(const classType classToRemove, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->Class() != classToRemove)
			vRoisOut.push_back(vRois[i]);
	}
}

void CRoiIO::FilterRoisByClass2(const classType classToRetain, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->Class() == classToRetain)
			vRoisOut.push_back(vRois[i]);
	}
}

void CRoiIO::FilterRoisByVisibility(const visibilityType typeToRetain, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->visibility() == typeToRetain)
			vRoisOut.push_back(vRois[i]);
	}
}

void CRoiIO::FilterRoisForTraining(const classType objClass, const float minHeight, const visibilityType visibilityType, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if ((vRois[i]->Class() == objClass) && (vRois[i]->obligatory()) && (vRois[i]->h()>=minHeight) && (vRois[i]->visibility() == visibilityType))
			vRoisOut.push_back(vRois[i]);
	}
}


/**************************************************************************//**
 * @brief	Filter rois by mandatory.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Jul 25, 2012
 *
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByMandatory(const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->obligatory())
			vRoisOut.push_back(vRois[i]);
	}
}

/**************************************************************************//**
 * @brief	Filter rois by label.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	labelToRemove   	The label to remove.
 * @param	vvRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByLabel(const classType labelToRemove, const vector<vector<CRoi*>>& vvRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for(unsigned int j=0;j<vvRois.size();j++)
	for (unsigned int i=0; i<vvRois[j].size(); i++)
	{
		if (vvRois[j][i]->Class() != labelToRemove)
			vRoisOut.push_back(vvRois[j][i]);
	}
}


/**************************************************************************//**
 * @brief	Filter rois by height.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minHeight			Height of the minimum.
 * @param	maxHeight			Height of the maximum.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByHeight(const float minHeight, const float maxHeight, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if ((vRois[i]->h()>=minHeight) && (vRois[i]->h()<=maxHeight))
			vRoisOut.push_back(vRois[i]);
	}
}

void CRoiIO::FilterRoisByContactToBorder(const int imgSizeX, const int imgSizeY, const float maxPercX, const float maxPercY, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut, const float percMargin)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		// Compute the size with the margins added
		float aspectRatio = 0.5f;
		float h = (float)vRois[i]->h();
		int margin = int(percMargin * h + 0.5f);
		h = h + 2*margin;
		float w = float(int(h*aspectRatio + 0.5f));
		h = w/aspectRatio;

		int x1, y1, x2, y2;
		CentralToCorners((float)vRois[i]->x(), (float)vRois[i]->y(), w, h, x1, y1, x2, y2);

		float pOutL = max(0.0f, (0.0f-x1)/w);
		float pOutR = max(0.0f, (x2-imgSizeX)/w);
		float pOutT = max(0.0f, (0.0f-y1)/h);
		float pOutB = max(0.0f, (y2-imgSizeY)/h);

		if (pOutL<=maxPercX && pOutR<=maxPercX && pOutT<=maxPercY && pOutB<=maxPercY)
			vRoisOut.push_back(vRois[i]);
	}
}


/**************************************************************************//**
 * @brief	Filter rois random.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	nRois				The rois.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisRandom(const int nRois, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{

	if (int(vRois.size())<=nRois)
	{
		vRoisOut.clear();
		vRoisOut.resize(vRois.size());
		copy(vRois.begin(), vRois.end(), vRoisOut.begin());
		return;
	}
	MTRand m_mersenneRandom(10);

	vector<unsigned long> vec;
	m_mersenneRandom.GenerateRandomVector((unsigned long)0, (unsigned long)vRois.size(), (unsigned long)nRois, vec);
	sort(vec.begin(), vec.end());

	vector<CRoi*>::iterator it;
	for (unsigned long i=0; i<vec.size(); i++)
	{
		int index = vec[i];
		vRoisOut.push_back(vRois[index]);
	}
}

/**************************************************************************//**
 * @brief	Filter rois random.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	nRois				The rois.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisRandomInRange(const int nRois,const float minConfidence,const float maxConfidence, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vector<CRoi*> tempRois;
	FilterRoisByConfidence(minConfidence,maxConfidence,vRois,tempRois);
	if (int(tempRois.size())<=nRois)
	{
		vRoisOut.clear();
		vRoisOut.resize(tempRois.size());
		copy(tempRois.begin(), tempRois.end(), vRoisOut.begin());
		return;
	}
	MTRand m_mersenneRandom(10);

	vector<unsigned long> vec;
	m_mersenneRandom.GenerateRandomVector((unsigned long)0, (unsigned long)tempRois.size(), (unsigned long)nRois, vec);
	sort(vec.begin(), vec.end());

	vector<CRoi*>::iterator it;
	for (unsigned long i=0; i<vec.size(); i++)
	{
		int index = vec[i];
		vRoisOut.push_back(tempRois[index]);
	}
}
///////////////////////////////////////////////////////////////////////////////
// Functions: Remove Filters
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Filter rois by scale factor.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minScalefactor  	The minimum scalefactor.
 * @param	vRois				The rois.
 * @param [in,out]	vRoisOut	[in,out] If non-null, the rois out.
 *****************************************************************************/
void CRoiIO::FilterRoisByScaleFactor(const float minScalefactor, const vector<CRoi*>& vRois, vector<CRoi*>& vRoisOut)
{
	vRoisOut.clear();
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->s() > minScalefactor)
			vRoisOut.push_back(vRois[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Functions: Set Filters
///////////////////////////////////////////////////////////////////////////////

/**************************************************************************//**
 * @brief	Divides the rois by scale
 *
 * @author	David Gerónimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
  * @param [in,out]	vRois	[in,out] The multiplexed rois.
 *****************************************************************************/
void CRoiIO::DivideRoisByScale(const vector<CRoi*>& vRois, vector<vector<CRoi*> >& vvRois, const int nScales)
{
	// Count number of scales
	//int nScales=0;
	//int maxScaleIndex=0;
	//for (unsigned int i=0; i<vRois.size(); i++)
	//{
	//	int si = vRois[i]->scaleIndex();
	//	if (si>maxScaleIndex)
	//		si = maxScaleIndex;
	//}
	//nScales=maxScaleIndex+1;
 
	// Create a roi vector for each scale
	//for (int i=0; i<nScales; i++)
	//{
	//	vector<CRoi*> vRois;
	//	vvRois.push_back(vRois);
	//}

	vvRois.clear();
	vvRois.resize(nScales, vector<CRoi*>());

	// Multiplex the rois
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		vvRois[	vRois[i]->scaleIndex() ].push_back(vRois[i]);
	}
}

/**************************************************************************//**
 * @brief	Divides the rois by scale
 *
 * @author	David Gerónimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
  * @param [in,out]	The multiplexed rois	[in,out] vRois.
 *****************************************************************************/
void CRoiIO::UndivideRoisByScale(const vector<vector<CRoi*> >& vvRois, vector<CRoi*>& vRois)
{
	vRois.clear();
	for (unsigned int si=0; si<vvRois.size(); si++)
	{
		for (unsigned int i=0; i<vvRois[si].size(); i++)
		{
			vRois.push_back(vvRois[si][i]);
		}
	}
}

/**************************************************************************//**
 * @brief	Sets the labels by confidence.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minConfidence	The minimum confidence.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::SetLabelsByConfidence(const float minConfidence, vector<CRoi*>& vRois)
{
	for (unsigned int i=0; i<vRois.size(); i++)
		if (vRois[i]->confidence() < minConfidence)
			vRois[i]->set_class(CT_Background);
}


/**************************************************************************//**
 * @brief	Sets the obligatories by height.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	minHeight	 	Height of the minimum.
 * @param	maxHeight	 	Height of the maximum.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::SetObligatoriesByHeight(const float minHeight, const float maxHeight, vector<CRoi*>& vRois)
{
	for (unsigned int i=0; i<vRois.size(); i++)
	if (!(vRois[i]->h()>=minHeight && vRois[i]->h()<=maxHeight))
		vRois[i]->set_obligatory(false);
}

void DrawArrow(Mat& image, const Point& begin, const Point& end, const CColor& color, const int width, const float shaftScale, const float HeadLength)
{
//	Point p = begin;
//	Point q = end;
//	float angle = atan2((float) p.y - q.y, (float) p.x - q.x);
//	float hypotenuse = sqrt(float((p.y - q.y)*(p.y - q.y) + (p.x - q.x)*(p.x - q.x)));
//	// Here we lengthen the arrow by a factor of three.
//	q.x = (int) (p.x - shaftScale * hypotenuse * cos(angle));
//	q.y = (int) (p.y - shaftScale * hypotenuse * sin(angle));
//	// Draw the arrow shaft
//	line(image, p, q, Scalar(color.b, color.g, color.r, color.a), width, CV_AA, 0 );
//	// Draw the arrow head
//	p.x = (int) (q.x + HeadLength * cos(angle + PI / 4));
//	p.y = (int) (q.y + HeadLength * sin(angle + PI / 4));
//	line(image, p, q, Scalar(color.b, color.g, color.r, color.a), width, CV_AA, 0 );
//	p.x = (int) (q.x + HeadLength * cos(angle - PI / 4));
//	p.y = (int) (q.y + HeadLength * sin(angle - PI / 4));
//    line(image, p, q, Scalar(color.b, color.g, color.r, color.a), width, CV_AA, 0 );
    VSysError(VSys_NotImplemented, "Use arrowedLine");
    //cv::arrowedLine!!!!!!!!!!!!!!!!!
}

void CRoiIO::Show(Mat& image, const CRoi& roi, const CColor& color, const int width, const bool showConfidence, const bool showDistance, const bool showTrackingInfo)
{
	// Writing parameters
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 2;

	// Text
	char textC[64], textD[64], textT[64];
	sprintf(textC, "%.3f", roi.confidence());
	sprintf(textD, "%.2fm", sqrt(roi.zw()*roi.zw()+roi.xw()*roi.xw()));
	sprintf(textT, "Id:%2u, Ag:%2u, Sk%2u", roi.trackerId(), roi.trackerAge(), roi.trackerNumSkips());

	// Get text size
	int baseline=0;
	Size textSizeC = getTextSize(textC, fontFace, fontScale, thickness, &baseline);
	Size textSizeD = getTextSize(textD, fontFace, fontScale/2, thickness/2, &baseline);
	Size textSizeT = getTextSize(textT, fontFace, fontScale/2, thickness/2, &baseline);
	baseline += thickness;
	Point textOrgC((int)roi.x()-textSizeC.width/2, (int)roi.y1()-textSizeC.height/2);
	Point textOrgD((int)roi.x()-textSizeD.width/2, (int)roi.y2()+textSizeD.height+2);
	Point textOrgT((int)roi.x()-textSizeT.width/2, (int)roi.y2()+textSizeT.height+2);

	rectangle(image, Point((int)roi.x1(), (int)roi.y1()), Point((int)roi.x2(), (int)roi.y2()), Scalar(color.b, color.g, color.r, color.a), width);
	if (showConfidence)
		putText(image, textC, textOrgC, fontFace, fontScale, Scalar::all(255), thickness);
	if (showDistance)
		putText(image, textD, textOrgD, fontFace, fontScale, Scalar::all(255), thickness);
	if (showTrackingInfo)
	{
		putText(image, textT, textOrgT, fontFace, fontScale, Scalar::all(255), thickness);
		// Draw velocity vector
		Point begin((int)roi.x(), (int)roi.y());
		Point end(int(roi.x()+roi.velocityX()), int(roi.y()+roi.velocityY()));
		DrawArrow(image, begin, end, CColor(255,255,255), width, 3.0f, 4.0f);
	}
}

void CRoiIO::ShowAsPoint(Mat& image, const CRoi& roi, const CColor& color, const bool showConfidence, const bool showDistance)
{
	// Writing parameters
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.25;
	int thickness = 1;

	// Text
	char textC[64], textD[64];
	sprintf(textC, "%.3f", roi.confidence());
	sprintf(textD, "%.2fm", roi.zw());

	// Get text size
	int baseline=0;
	Size textSizeC = getTextSize(textC, fontFace, fontScale, thickness, &baseline);
	Size textSizeD = getTextSize(textC, fontFace, fontScale, thickness, &baseline);
	baseline += thickness;
	Point textOrgC((int)roi.x()-textSizeC.width/2, (int)roi.y1()-textSizeC.height/2);
	Point textOrgD((int)roi.x()-textSizeD.width/2, (int)roi.y2()+textSizeD.height+2);

	circle(image, Point((int)roi.x(), (int)roi.y2()), int(1.0f*roi.h()/128.0f), Scalar(color.b, color.g, color.r, color.a));
	if (showConfidence)
		putText(image, textC, textOrgC, fontFace, fontScale, Scalar::all(255), thickness);
	if (showDistance)
		putText(image, textD, textOrgD, fontFace, fontScale, Scalar::all(255), thickness);
}

void CRoiIO::Show(Mat& image, const vector<CRoi*>& vRois, RoiDisplayParam dispParam)
{
	if (dispParam.m_isEnabled)
	for (unsigned int i=0; i<vRois.size(); i++)
	{		
		if (vRois[i]->confidence()>dispParam.m_threshold)
		{
			if (!vRois[i]->obligatory() || (vRois[i]->trackerAge()>0 && !vRois[i]->trackerActive()))
				Show(image, *vRois[i], dispParam.m_colorOptional, dispParam.m_lineWidthOptional, dispParam.m_showConfidence, dispParam.m_showDistance, dispParam.m_showTrackingInfo);
			else
				Show(image, *vRois[i], dispParam.m_color, dispParam.m_lineWidth, dispParam.m_showConfidence, dispParam.m_showDistance, dispParam.m_showTrackingInfo);
		}
	}
}

void CRoiIO::ShowAsPoint(Mat& image, const vector<CRoi*>& vRois, RoiDisplayParam dispParam)
{
	if (dispParam.m_isEnabled)
	for (unsigned int i=0; i<vRois.size(); i++)
	{		
		if (vRois[i]->confidence()>dispParam.m_threshold)
		{
			if (vRois[i]->obligatory())
				ShowAsPoint(image, *vRois[i], dispParam.m_color, dispParam.m_showConfidence, dispParam.m_showDistance);
			else
				ShowAsPoint(image, *vRois[i], dispParam.m_colorOptional, dispParam.m_showConfidence, dispParam.m_showDistance);
		}
	}
}

/**************************************************************************//**
 * @brief	Reads a roi display parameter.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param [in,out]	pIniFileIO	If non-null, the initialise file reader.
 * @param	section				  	The section.
 *
 * @return	The roi display parameter.
 *****************************************************************************/
RoiDisplayParam CRoiIO::ReadRoiDisplayParam(CIniFileIO* pIniFileIO, const string& section)
{
	// Create the output struct
	RoiDisplayParam out;

	// Default parameres
	CColor defColor;
	int defWidth;
	bool defShowConf;
	bool defEnabled;
	bool defTracking = false;
	if (section=="REGIONS")
	{
		defColor	= CColor(255,255,0,0.5f);
		defWidth	= 1;
		defShowConf	= false;
		defEnabled	= false;
	}
	else if (section=="ROIS")
	{
		defColor	= CColor(0,0,255,0.5f);
		defWidth	= 1;
		defShowConf	= true;
		defEnabled	= false;
	}
	else if (section=="DETECTIONS")
	{
		defColor	= CColor(0,0,255,0.5f);
		defWidth	= 3;
		defShowConf	= true;
//		defShowClassName = false;
//		defShowClassImage = false;
		defEnabled	= true;
	}
	else if (section=="TRACKS")
	{
		defColor	= CColor(0,0,255,0.5f);
		defWidth	= 3;
		defShowConf	= true;
		defEnabled	= true;
		defTracking = true;
	}
	else if (section=="ANNOTATIONS")
	{
		defColor	= CColor(0,0,0,0.5f);
		defWidth	= 4;
		defShowConf	= false;
		defEnabled	= true;
	}
	else if (section=="TP")
	{
		defColor	= CColor(0,0,255,0.5f);
		defWidth	= 3;
		defShowConf	= true;
		defEnabled	= true;
	}
	else if (section=="TPGT")
	{
		defColor	= CColor(0,0,255,0.5f);
		defWidth	= 3;
		defShowConf	= false;
		defEnabled	= true;
	}
	else if (section=="FN")
	{
		defColor	= CColor(255,0,0,0.5f);
		defWidth	= 3;
		defShowConf	= false;
		defEnabled	= true;
	}
	else if (section=="FP")
	{
		defColor	= CColor(255,255,0,0.5f);
		defWidth	= 3;
		defShowConf	= false;
		defEnabled	= true;
	}
	else
	{
		defColor	= CColor(0,0,255,0.5f);
		defWidth	= 3;
		defShowConf	= true;
		defEnabled	= true;
	}

	// Read the parameters
	pIniFileIO->ReadSection(section, "Enabled",			out.m_isEnabled, defEnabled, "Enable the display of this ROI");
    pIniFileIO->ReadSection(section, "ShowConfidence",	out.m_showConfidence, defShowConf, "Shows the ROI confidence");
	pIniFileIO->ReadSection(section, "ShowDistance",	out.m_showDistance, false, "Shows the ROI distance to the GT");
	pIniFileIO->ReadSection(section, "ShowTrackingInfo",out.m_showTrackingInfo, defTracking, "Shows the tracking info");
	pIniFileIO->ReadSection(section, "Color",			out.m_color, defColor, "Color of the BBox lines");
	pIniFileIO->ReadSection(section, "ColorOptional",	out.m_colorOptional, CColor(128,128,0,0.5f), "Color of the BBox lines for optinal objects");
	pIniFileIO->ReadSection(section, "LineWidth",		out.m_lineWidth, defWidth, "With of the BBox lines");
	pIniFileIO->ReadSection(section, "LineWidthOptional",out.m_lineWidthOptional, 1, "With of the BBox lines for optional objects");
	pIniFileIO->ReadSection(section, "Threshold",		out.m_threshold, 0, "Only shown the ROI If the confidence is grather than the threshold");

	// Return the filled struct
	return out;
}


/**************************************************************************//**
 * @brief	Writes the detections for Caltech evaluation.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez  (David.Vazquez@cvc.uab.es)
 * @author	Jiaolong Xu	   (jiaolong@cvc.uab.es)
 * @date	Aug 16, 2012
 *
 * @param	filename	  Filename of the file.
 * @param	vRois   	  The rois.
 * @param   saveViewpoint 1: save viewpoint estimation; 0: not save
 *****************************************************************************/
void CRoiIO::WriteEvaluationCaltech(const string& filename, const vector<CRoi*>& vRois, const bool saveViewpoint)
{
	// Caltech Evaluation: [x1, y1, w, h, score]
	// e.g. 467.17,177.73,35.625,71.663,0.34431
	// 1. Open the file
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 3. Write the ROIs
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		if (vRois[i]->Class()!=CT_Unknown && vRois[i]->Class()!=CT_Background)
		{
			if(saveViewpoint)
				Fprintf(fp, "%0.3f,%0.3f,%0.3f,%0.3f,%.6f, %d\n", vRois[i]->x()-vRois[i]->w()/2, vRois[i]->y()-vRois[i]->h()/2, vRois[i]->w(), vRois[i]->h(), vRois[i]->confidence(), 0);
			else
				Fprintf(fp, "%0.3f,%0.3f,%0.3f,%0.3f,%.6f,%d\n", vRois[i]->x()-vRois[i]->w()/2, vRois[i]->y()-vRois[i]->h()/2, vRois[i]->w(), vRois[i]->h(), vRois[i]->confidence(), vRois[i]->Class());
		}
	}

	// 4. Close the file
	fclose(fp);
}


/**************************************************************************//**
 * @brief	Writes the detections for Kitti evaluation.
 *
 * @author	Alejandro Gonzalez (agalzate@cvc.uab.es)
 * @date	Jan 8, 2015
 *
 * @param	filename	  Filename of the file.
 * @param	vRois   	  The rois.
 *****************************************************************************/
void CRoiIO::WriteEvaluationKitti(const string& filename, const vector<CRoi*>& vRois, const string& object)
{
	// 1. Open the file
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 3. Write the ROIs
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		Fprintf(fp, "%s %d %d %d %0.2f %0.2f %0.2f %0.2f %d %d %d %d %d %d %d %.2f\n", object.c_str(), -1, -1, -10, vRois[i]->x()-(float)(vRois[i]->w())/2, vRois[i]->y()-(float)(vRois[i]->h())/2, vRois[i]->x()+(float)(vRois[i]->w())/2, vRois[i]->y()+(float)(vRois[i]->h())/2, -1, -1, -1, -1000, -1000, -1000, -10, vRois[i]->confidence());
	}

	// 4. Close the file
	fclose(fp);
}


/**************************************************************************//**
 * @brief	Writes the detections for Caltech evaluation.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez  (David.Vazquez@cvc.uab.es)
 * @author	Jiaolong Xu	   (jiaolong@cvc.uab.es)
 * @date	Aug 16, 2012
 *
 * @param	filename	  Filename of the file.
 * @param	vRois   	  The rois.
 * @param   saveViewpoint 1: save viewpoint estimation; 0: not save
 *****************************************************************************/
void CRoiIO::WriteDisparities(const string& filename, const vector<CRoi*>& vRois)
{
	// 1. Open the file in append mode
	FILE*fp=fopen(filename.c_str(), "a");
	if (fp==NULL)
	{
		ErrorQuit(VSys_FileNotFound, filename);
		return;
	}

	// 2. Write the disparities
	for (unsigned int i=0; i<vRois.size(); i++)
	{
		Fprintf(fp, "%d %d %d %d %d %.6f %.6f %d %d %d %d %d %d %d\n", vRois[i]->x(), vRois[i]->y(), vRois[i]->w(), vRois[i]->h(), (int)vRois[i]->obligatory(), vRois[i]->dispT(), vRois[i]->dispR(), (int)vRois[i]->Class(), (int)vRois[i]->view(), (int)vRois[i]->visibility(), (int)vRois[i]->scene(), (int)vRois[i]->trackerId(), (int)vRois[i]->pose());
	}

	// 3. Close the file
	fclose(fp);
}

/**************************************************************************//**
 * @brief	Reads the disparities.
 *
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	filename	 	Filename of the file.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
void CRoiIO::ReadDisparities(const string& filename, vector<CRoi*>& vRois)
{
	// 0. Auxiliar variables
	char str[1024];
	vector<string> tokens;
	vRois.clear();

	// 1. Open the file
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp==NULL)
	{
		//ErrorQuit(VSys_FileNotFound, filename);
		VSysError(VSys_FileNotFound, filename);
		return;
	}

	// 2. Read the data
	while(fgets (str, 1024, fp)!=NULL)
	{
		// 2.1 Init default parameters
		float x=0.0f, y=0.0f, w=0.0f, h=0.0f, dispT=0.0f, dispR=0.0f;
		int a=1, b=CT_Pedestrian, c=VT_Front, d=VBT_FullyVisible, e=ST_Street, f=0, g=PT_Stand;

		// 2.2 Tokenize
		Tokenize(str, tokens, " ");
		if (tokens.size()>=4)
		{
			// Extract tokens
			x = (float)atof(tokens[0].c_str());
			y = (float)atof(tokens[1].c_str());
			w = (float)atof(tokens[2].c_str());
			h = (float)atof(tokens[3].c_str());
			if (tokens.size()>4)
			{
				if (strncmp(tokens[4].c_str(), "PEDESTRIAN-OBLIGATORY", 20)==0||strncmp(tokens[4].c_str(), "1",1)==0)
					a = 1;
				else
					a = 0;
			}
			if (tokens.size()>5) dispT = (float)atof(tokens[5].c_str());
			if (tokens.size()>6) dispR = (float)atof(tokens[6].c_str());
			if (tokens.size()>7) b = atoi(tokens[7].c_str());
			if (tokens.size()>8) c = atoi(tokens[8].c_str());
			if (tokens.size()>9) d = atoi(tokens[9].c_str());
			if (tokens.size()>10) e = atoi(tokens[10].c_str());
			if (tokens.size()>11) f = atoi(tokens[11].c_str());
			if (tokens.size()>12) g = atoi(tokens[12].c_str());

			// Set the ROI
			CRoi* pRoi = new CRoi((int)x, (int)y, (int)w, (int)h);
			pRoi->set_dispT(dispT);
			pRoi->set_dispR(dispR);
			pRoi->set_obligatory(a!=0);
			pRoi->set_class((classType)b);
			pRoi->set_view((viewType)c);
			pRoi->set_visibility((visibilityType)d);
			pRoi->set_scene((sceneType)e);
			pRoi->set_trackerId(f);
			pRoi->set_pose((poseType)g);

			// Save the ROI
			vRois.push_back(pRoi);
		}
	}

	// 3. Close the file
	fclose(fp);
}


///////////////////////////////////////////////////////////////////////////////
// Functions: Sort Functions
///////////////////////////////////////////////////////////////////////////////


void CRoiIO::Quicksort(vector<CRoi*>& vRois, int lo, int hi)
{
    if (lo < hi)
	{
        int p = Partition(vRois, lo, hi);
        Quicksort(vRois, lo, p-1);
        Quicksort(vRois, p+1, hi);
	}
}

int CRoiIO::Partition(vector<CRoi*>& vRois, int left, int right)
{
    int pivotIndex = (left+right)/2;
    CRoi* auxRoi,*pivotValue = vRois[pivotIndex];
	auxRoi = vRois[pivotIndex];
	vRois[pivotIndex] = vRois[right];
	vRois[right] = auxRoi;
    int storeIndex = left;
    for (int i=left; i < right; i++)
	{
		if (*vRois[i] <= *pivotValue)
		{
			auxRoi = vRois[i];
			vRois[i] = vRois[storeIndex];
			vRois[storeIndex] = auxRoi;
			storeIndex++;
		}
	}
	auxRoi = vRois[storeIndex];
	vRois[storeIndex] = vRois[right];
	vRois[right] = auxRoi;
    return storeIndex;
}


/**************************************************************************//**
 * @brief	Calculates the disparity values of ROIs
 *
 * @author	
 * @date	
 *
 * @param	imageSizeX   	The image size x coordinate.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
 void CRoiIO::saveDisparityMean(vector<CRoi*>& vRois, const cv::Mat& disp, const float minDisparityPixels, const CStereoCamera& pStereoCamera, bool recompute3Drois)
 {
	//Create integral disparity images to save ROI`s disparity
	CImageG<float> disparity(disp.cols,disp.rows);
	CImageG<float> disparityIntegral(disp.cols+1,disp.rows+1);
	CImageG<float> disparityMask(disp.cols,disp.rows);
	CImageG<float> disparityMaskIntegral(disp.cols+1,disp.rows+1);

	//Create disparity mask: pixel has disparity = 1. pixel hasn't disparity = 0.
	for (int i = 0; i < disp.cols; i++)
		for (int j = 0; j < disp.rows; j++)
			if ((int)disp.ptr<uchar>(j)[i]!=0) disparityMask.SetPixel(i,j,1);
			else disparityMask.SetPixel(i,j,0);

	//Calculate integral disparity image
	CImageConversion::OpenCVMatToImageG(disp,disparity);
	CImageProcessing::IntegralImageFast(disparity, disparityIntegral);
	CImageProcessing::IntegralImageFast(disparityMask, disparityMaskIntegral);

	for (int i = 0; i < vRois.size(); i++)
	{
		int x1,x2,y1,y2;
		CentralToCornersInt(vRois[i]->x(),vRois[i]->y(),vRois[i]->w(),vRois[i]->h(),x1,y1,x2,y2);

		// Get number of pixels with disparity and total number of pixels.
		int sizeX = x2-x1;
		int sizeY = y2-y1;
		x1 = x1 + sizeX/3;
		x2 = x2 - sizeX/3;
		y1 = y1 + sizeY/3;
		y2 = y2 - sizeY/3;
		float numPixel = CImageProcessing::SumIntegralRegionFast(disparityMaskIntegral,x1,y1,x2,y2); //number of pixels with disparity
		float sum = CImageProcessing::SumIntegralRegionFast(disparityIntegral,x1,y1,x2,y2); //disparity of all pixels
		int regionPixels = (x2-x1+1)*(y2-y1+1); //number total of pixels

		// Delete roi if it don't meet conditions
		/*float z = vRois[i]->zw();
        float disparityValue = pStereoCamera.DisparityFromDistance(vRois[i]->zw());

		float cap = (float)sum/numPixel;
		if ((float)numPixel/regionPixels > minDisparityPixels)
		{
			vRois[i]->set_dispR((float)sum/numPixel);
			vRois[i]->set_dispT(disparityValue);
		}*/
		float wz = 0.0f, wx1=0.0f,wy1=0.0f,wx2=0.0f,wy2=0.0f, disparityValue = sum/numPixel;
		if ( numPixel/regionPixels > minDisparityPixels  && disparityValue > 0.0f)
		{
			if (recompute3Drois)
			{
				/*pStereoCamera.Image2WorldFromDisparity(x1-(float)sizeX/3, y1-(float)sizeY/3, disparityValue, wx1, wy1, wz);
				pStereoCamera.Image2WorldFromDisparity(x2+(float)sizeX/3, y2+(float)sizeY/3, disparityValue, wx2, wy2, wz);
				wz = pStereoCamera.DistanceFromDisparity(disparityValue);
				vRois[i]->SetWorldCoordinates((wx2+wx1)/2,(wy2+wy1)/2,wz,wx2-wx1,-(wy2-wy1),1);*/
				float wz = 0, wx1=0,wy1=0,wx2=0,wy2=0;
				pStereoCamera.Image2WorldFromDisparity((float)x1,(float)y1,(float)disparityValue,wx1,wy1,wz);
				pStereoCamera.Image2WorldFromDisparity((float)x2,(float)y2,(float)disparityValue,wx2,wy2,wz);
				wz = pStereoCamera.DistanceFromDisparity((float)disparityValue);
				float margin = 0.3;
				vRois[i]->SetWorldCoordinates((wx2+wx1)/2,(wy2+wy1)/2-margin/2,wz,wx2-wx1+margin,(wy2+wy1)-margin,0.5);
			}
			vRois[i]->set_dispR(disparityValue);
		}
		
	}
 }

 
/**************************************************************************//**
 * @brief	Calculates the disparity values of ROIs
 *
 * @author	
 * @date	
 *
 * @param	imageSizeX   	The image size x coordinate.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
 void CRoiIO::saveDisparityMedian(vector<CRoi*>& vRois, const cv::Mat& disp, const float minDisparityPixels, const CStereoCamera& pStereoCamera, bool recompute3Drois)
 {
	//Create integral disparity images to save ROI`s disparity
	CImageG<int> disparity(disp.cols,disp.rows);
	CImageG<int> disparity2(disp.cols,disp.rows);
	CImageG<int> disparityIntegral(disp.cols+1,disp.rows+1);
	CImageG<int> disparityIntegral2(disp.cols+1,disp.rows+1);
	CImageG<int> disparityMask(disp.cols,disp.rows);
	CImageG<int> disparityMaskIntegral(disp.cols+1,disp.rows+1);

	//Create disparity mask: pixel has disparity = 1. pixel hasn't disparity = 0.
	for (int i = 0; i < disp.cols; i++)
		for (int j = 0; j < disp.rows; j++)
			if ((int)disp.ptr<uchar>(j)[i]!=0) disparityMask.SetPixel(i,j,1);
			else disparityMask.SetPixel(i,j,0);

	//Create disparity^2 matrix
	for (int i = 0; i < disp.cols; i++)
		for (int j = 0; j < disp.rows; j++)
			if ((int)disp.ptr<uchar>(j)[i]!=0) disparity2.SetPixel(i,j,(int)disp.ptr<uchar>(j)[i]*(int)disp.ptr<uchar>(j)[i]);
			else disparity2.SetPixel(i,j,0);

	//Calculate integral disparity image
	CImageConversion::OpenCVMatToImageG(disp,disparity);
	CImageProcessing::IntegralImageFast(disparity, disparityIntegral);
	CImageProcessing::IntegralImageFast(disparity2, disparityIntegral2);
	CImageProcessing::IntegralImageFast(disparityMask, disparityMaskIntegral);

	for (int i = 0; i < vRois.size(); i++)
	{
		int x1,x2,y1,y2;
		CentralToCornersInt(vRois[i]->x(),vRois[i]->y(),vRois[i]->w(),vRois[i]->h(),x1,y1,x2,y2);

		// Get number of pixels with disparity and total number of pixels.
		if (y1<0) y1 =0;
		if (x1<0) x1 =0;
		if (x2>=disp.cols) x2 = disp.cols-1;
		if (y2>=disp.rows) y2 = disp.rows-1;
		int sizeX = x2-x1;
		int sizeY = y2-y1;
		x1 = x1 + sizeX/3;
		x2 = x2 - sizeX/3;
		y1 = y1 + sizeY/3;
		y2 = y2 - sizeY/3;
		int numPixel = CImageProcessing::SumIntegralRegionFast(disparityMaskIntegral,x1,y1,x2,y2); //number of pixels with disparity
		int sum = CImageProcessing::SumIntegralRegionFast(disparityIntegral,x1,y1,x2,y2); //disparity of all pixels
		int regionPixels = (x2-x1+1)*(y2-y1+1); //number total of pixels

		// Delete roi if it don't meet conditions
		float z = vRois[i]->zw();
        float disparityValue = pStereoCamera.DisparityFromDistance(vRois[i]->zw());

		float cap = (float)sum/numPixel;
		if ((float)numPixel/regionPixels > minDisparityPixels)
		{
			//Compute desviacion standard
			int sum2 = CImageProcessing::SumIntegralRegionFast(disparityIntegral2,x1,y1,x2,y2); //disparity of all pixels
			float mean = (float)sum/numPixel;
			float mean2 = (float)sum2/numPixel;
			float variance = (mean2 - mean*mean);
			if (variance <=0) variance = FLT_EPSILON;
			float desvStd = sqrt(variance);

			//Create array from roi
			vector<int> ArrayRoi(numPixel);
			int cont = 0;
			for (int j = x1; j<=x2; j++)
				for (int k = y1; k<=y2; k++)
					if ((int)disp.ptr<uchar>(k)[j]!=0)
					{
						ArrayRoi[cont] = ((int)disp.ptr<uchar>(k)[j]);
						cont++;
					}

			//Compute median
			int B = int(desvStd*0.2f);
			float median = MedianBinsApproximation(1000, mean, desvStd, ArrayRoi);

		float wz = 0, wx1=0,wy1=0,wx2=0,wy2=0;
		if ((float)numPixel/regionPixels > minDisparityPixels && median > 0.0f)
		{
			if (recompute3Drois)
			{
				pStereoCamera.Image2WorldFromDisparity(x1-(float)sizeX/3,y1-(float)sizeY/3,median,wx1,wy1,wz);
				pStereoCamera.Image2WorldFromDisparity(x2+(float)sizeX/3,y2+(float)sizeY/3,median,wx2,wy2,wz);
				wz = pStereoCamera.DistanceFromDisparity(median);
				float margin = 0.3;
				vRois[i]->SetWorldCoordinates((wx2+wx1)/2,(wy2+wy1)/2-margin/2,wz,wx2-wx1+margin,(wy2+wy1)-margin,0.5);
			}
			vRois[i]->set_dispR(median);
		}
		

		}
	}
 }

float CRoiIO::MedianBinsApproximation(const int NBins, const float mean, const float desvStd, const vector<int>& arrayData)
{
			//Initialize variables
			float iniBin = mean - desvStd;
			float endBin = mean + desvStd;
			float scaleFactor = (desvStd*2)/(NBins-1);
			int sizeRoi = (int)arrayData.size();
			vector<int> bins(NBins);

			//Accum bins
			int countDown = 0;
			for (int j = 0; j<sizeRoi; j++)
			{
				if (arrayData[j] < iniBin)
					countDown++;
				else if (arrayData[j] < endBin)
				{
					float aux = (arrayData[j] - iniBin);
					int bin = (int) (aux / scaleFactor);
					bins[bin]++;
				}
			}

			//get median
			float median = 0;
				// If sizeRoi is odd
			if ((sizeRoi % 2) != 0)
			{
				// Find the bin that contains the median
				int middle = (int) ((sizeRoi) / 2);
				int count = countDown;

				for (int j = 0; j < NBins; j++)
				{
					count += bins[j];

					if (count >= middle) 
						//odd
					{
						median = j*scaleFactor + iniBin;
						break;
					}
				}
			}
				// If n is even
			else
			{
				// Find the bins that contains the medians
				int middle = (int) (sizeRoi / 2);
				int count = countDown;

				for (int j = 0; j < NBins; j++) 
				{
					count += bins[j];

					if (count >= middle) 
					{
						int k = j;
						while (count == middle && k < NBins-1)
						{
							k++;
							count += bins[k];
						}
						//even
						median = ((j + k)/2)*scaleFactor + iniBin;
						break;
					}
				}
			}
	return median;
}

 
/**************************************************************************//**
 * @brief	Calculates the disparity values of ROIs
 *
 * @author	
 * @date	
 *
 * @param	imageSizeX   	The image size x coordinate.
 * @param [in,out]	vRois	[in,out] If non-null, the rois.
 *****************************************************************************/
 void CRoiIO::saveDisparityFoot(vector<CRoi*>& vRois, const cv::Mat& disp, const CStereoCamera& pStereoCamera, bool recompute3Drois)
 {

	for (int i = 0; i < vRois.size(); i++)
	{
		int x1,x2,y1,y2;
		CentralToCornersInt(vRois[i]->x(),vRois[i]->y(),vRois[i]->w(),vRois[i]->h(),x1,y1,x2,y2);

		// Get disparity at foot level
		if (y2>disp.rows) y2=disp.rows-1;
		int disparityValue = (int)disp.ptr<uchar>(y2)[(int)(vRois[i]->x())];

		// Save disparity and wolrd coordinates
		float wz = 0, wx1=0,wy1=0,wx2=0,wy2=0;
		if (disparityValue >0)
		{
			if (recompute3Drois)
			{
				pStereoCamera.Image2WorldFromDisparity((float)x1,(float)y1,(float)disparityValue,wx1,wy1,wz);
				pStereoCamera.Image2WorldFromDisparity((float)x2,(float)y2,(float)disparityValue,wx2,wy2,wz);
				wz = pStereoCamera.DistanceFromDisparity((float)disparityValue);
				float margin = 0.3;
				vRois[i]->SetWorldCoordinates((wx2+wx1)/2,(wy2+wy1)/2-margin/2,wz,wx2-wx1+margin,(wy2+wy1)-margin,0.5);
			}
			vRois[i]->set_dispR((float)disparityValue);
		}
	}
 }
