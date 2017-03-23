/* ********************************* FILE ************************************/
/** @file		Maths.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the Maths functions
 * 
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _MATHS_
#define _MATHS_

#if _MSC_VER > 1000
	#pragma once
#endif



/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::swap;
using std::max;
using std::min;

#include "MersenneTwister.h"

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * DEFINES
 *****************************************************************************/
#define PI 3.1415926535897932384f
#define EPSILON 0.00001f
#define FINFINITY 9999999
#define MAX_FLOAT 3.40282e+038f

/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define MIN3(x,y,z) ((y)<=(z) ? ((x)<=(y) ? (x) : (y)) : ((x)<=(z) ? (x) : (z)))
#define MAX3(x,y,z) ((y)>=(z) ? ((x)>=(y) ? (x) : (y)) : ((x)>=(z) ? (x) : (z)))


/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

template<class T>
void ShowMatrix(vector<vector<T> > matrix, char* name)
{
	size_t sizeX = matrix.size();
	size_t sizeY = (sizeX>0) ? matrix[0].size() : 0;

	if (sizeX*sizeY>0)
	{
		if (name) cout << "[" << name << "] = " << endl;
		for(int y=0; y<sizeY; y++)
		{
			for(int x=0; x<sizeX; x++) cout << "\t" << matrix[x][y];
			cout << endl;
		}
		cout << endl;
	}
}

//////////////////////////////////////////////////////////////////////
// Random
//////////////////////////////////////////////////////////////////////

class CUniqueMersenne
{
// METHODS
public:
	static MTRand* Instance()
	{
		if (pMersenne==NULL) pMersenne = new MTRand();
		return pMersenne;
	}

	static float	GetRandom()					{ return (float)Instance()->rand(); }
	static int		GetRandom(int min, int max)	{ return int(Instance()->rand()*10000) % (max-min+1)+min; }

private:
	CUniqueMersenne() {}

// ATTRIBUTES
private:
	static MTRand* pMersenne;
};

inline float Random(float min, float max)	{ return min+(max-min)*CUniqueMersenne::GetRandom(); }

inline void SelectTwoRandomIntNoRepeat(int from, int to, int& r1, int& r2)
{
	if (from==to)
	{
		r1=r2=0; 
		return;
	}

	do
	{
		r1 = (int)Random((float)from, (float)to);
		r2 = (int)Random((float)from, (float)to);
	} while (r1==r2);
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Low level
///////////////////////////////////////////////////////////////////////////////
inline int		sign				(const float a)					{ return (a>0)?1:((a<0)?-1:0);							}
inline int		Round				(const float v)					{ return (v>=0)? int(v+0.5f) : int(v-0.5f);				}
inline bool		GreaterThanEpsilon	(const float v)					{ return ((v>EPSILON) || (v<-EPSILON))? true : false;	}
inline int		Factorial			(const int n)					{ return (n>0)?n*Factorial(n-1):1;						}
inline int		Combination			(const int m, const int n)		{ return Factorial(m) / (Factorial(n)*(Factorial(m-n)));} // m over n
inline bool		IsFloatEqual		(const float a, const float b)	{ return fabs(a-b)<EPSILON;								}
inline float	werd				(const float v)					{ return (v<EPSILON && v>-EPSILON)?0.0f:v;				}
inline float	RadToDeg			(const float rad)				{ return rad*180.0f/PI;									}
inline float	ModAngle			(float angle)
{
	while (angle<0)		angle+=360;
	while (angle>360)	angle-=360;
	return angle;
}
inline float InterpolatePeak(float a, float b, float c)	// from krystian
{
	if (b < 0.0f) {a = -a; b = -b; c = -c;}
	if ((b>a && b<c) || (b<a && b > c) || (b==a && b==c)) return 0.0f;
	return 0.5f * (a - c) / (a - 2.0f * b + c);
}
inline float InterpolateScale(float a, float b, float c, int i, vector<float> scale) {return (1.0f+InterpolatePeak(a,b,c)*(scale[i]/scale[i-1]-1.0f))*scale[i];}
void invSqrRoot(float &a, float &b, float &c, float &r_l, float &r_m, float &ea);
//#define MY_FLT_EPSILON     1.192092896e-07F
inline float FastAtan2 (float y, float x)
{
	float angle, r;
	float const c3	= 0.1821F;
	float const c1	= 0.9675F;
    float abs_y		= fabs(y) + FLT_EPSILON;
 
	if (x >= 0)
	{
		r = (x-abs_y) / (x+abs_y);
		angle = (float) (PI/4);
	}
	else
	{
		r = (x+abs_y) / (abs_y-x);
		angle = (float) (3*PI/4);
	}
	angle += (c3*r*r - c1) * r;
	return (y < 0) ? - angle : angle;
}

 inline float mod2PI (float x)
 {
   while (x > (float)(2 * PI)) x -= (float) (2 * PI) ;
   while (x < 0.0F) x += (float) (2 * PI);
   return x ;
 }

inline float fastResqrt (float x)
{
	/* 32-bit version */
	union {
		float x;
		int i ;
	} u;
 
	float xhalf = (float) 0.5 * x ;
 
	/* convert floating point value in RAW integer */
	u.x = x ;
 
	/* gives initial guess y0 */
    u.i = 0x5f3759df - (u.i >> 1);
	/*u.i = 0xdf59375f - (u.i> >1);*/
 
	/* two Newton steps */
	u.x = u.x * ( (float) 1.5  - xhalf*u.x*u.x) ;
	u.x = u.x * ( (float) 1.5  - xhalf*u.x*u.x) ;
	return u.x ;
}

inline float fastSqrt (float x) { return (x < 1e-8) ? 0 : x * fastResqrt(x); }



///////////////////////////////////////////////////////////////////////////////
// Functions: Distance
///////////////////////////////////////////////////////////////////////////////
inline float DistanceEuclideanNoSqrt(const float* v1, const float* v2, const int dim)
{
	float sum=0;
	for (int i=0; i<dim; i++) sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
	return sum;
}
inline float DistanceEuclidean(const float* v1, const float* v2, const int dim) {return (float)sqrt(DistanceEuclideanNoSqrt(v1, v2, dim));}

//BHATTACHARYYA(histogram1, histogram2)
//compute the BHATTACHARYYA distance between 2 histograms
//where each histogram is a 1xN vector
//
//Based on the estimation presented in 
//"Real-Time Tracking of Non-Rigid Objects using Mean Shift"
// D. Comaniciu, V. Ramesh & P. Meer (IEEE CVPR 2000, 142-151)
//
//N.B. both histograms must be normalised
//(i.e. bin values lie in range 0->1 and SUM(bins(i)) = 1
//      for i = {histogram1, histogram2} )
//
//Author / Copyright: T. Breckon, October 2005.
//School of Informatics, University of Edinburgh.
//License: http://www.gnu.org/licenses/gpl.txt

//Modified from Matlab

inline float DistanceBhattacharyya(const vector<float> hist1, const vector<float> hist2)
{
    
    //estimate the bhattacharyya co-efficient
    
    float bcoeff = 0;
	float sum1 = 0, sum2 = 0;
    for (unsigned int i=0; i<hist1.size(); i++)
	{
        bcoeff = bcoeff + sqrt(hist1[i] * hist2[i]);
		sum1 += hist1[i];
		sum2 += hist2[i];
	}
    
    //get the distance between the two distributions as follows
    
    return sqrt(fabs(1 - (bcoeff/sqrt(sum1*sum2))));
}

inline float DistanceBhattacharyya1(const vector<float> hist1, const vector<float> hist2)
{
    
    //estimate the bhattacharyya co-efficient
    
    float bcoeff = 0;
	float sum1 = 0, sum2 = 0;
    for (unsigned int i=0; i<hist1.size(); i++)
	{
		sum1 += hist1[i];
		sum2 += hist2[i];
	}
	for (unsigned int i=0; i<hist1.size(); i++)
	{
		bcoeff = bcoeff + sqrt(hist1[i]*hist2[i])/sqrt(sum1*sum2);
	}
    
    //get the distance between the two distributions as follows
    return sqrt(fabs(1 - bcoeff));
}


inline float DistanceChiSquare(const vector<float> hist1, const vector<float> hist2)
{
    float sum = 0;
	for (unsigned int i=0; i<hist1.size(); i++)
	{
		float temp = (hist1[i]+hist2[i]);
		temp = temp==0?1:(hist1[i]+hist2[i]);
		sum += ((hist1[i]-hist2[i])*(hist1[i]-hist2[i]))/temp;
	}
	return sum;
}

inline float HistIntersection(const vector<float> hist1, const vector<float> hist2)
{
    float sum = 0;
	for (unsigned int i=0; i<hist1.size(); i++)
	{
		float temp = (hist1[i]>hist2[i])?hist2[i]:hist1[i];
		sum += temp;
	}
	return sum;
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Probability Distribution Functions (PDF)
///////////////////////////////////////////////////////////////////////////////
inline float SigmoidPDF(const float value) {return 1.0f/(1.0f+exp(-value));}

//inline float LambdaFi(float* g)
//{
//	float lambda = 0.0f;
//
//	// Find the coefficients of p(lambda)
//	gb = prod(g)*poly(-1./g); 
//
//	// Correct the last two
//	gb(end-1:end) = gb(end-1:end) -1; 
//
//	// Find the roots of p(lambda)
//	gc = roots(gb); 
//
//	// Identify lambda's index
//	ii = (imag(gc)==0)&(gc>=-1)&(abs(gc)>0.001); 
//
//	// find lambda
//	lambda = (gc(ii)); 
//	
//	return lambda;
//}

///////////////////////////////////////////////////////////////////////////////
// Functions: Histograms
///////////////////////////////////////////////////////////////////////////////
inline void SmoothHistogram(float *hist, const int bins)
{
	float prev = hist[bins - 1];
	float first= hist[0];
	float temp;
	for (int i = 0; i<bins; i++)
	{
		temp = hist[i];
		hist[i] = (prev + hist[i] + ((i + 1 == bins) ? first : hist[i + 1])) / 3.0f;
		prev = temp;
	}
}

inline void NormalizeL2(float* vect, const int dim)
{
	float acc=0;
	for (int i=0; i<dim; i++) acc += vect[i]*vect[i];
	float fac = 1/sqrt(acc);
	for (int i=0; i<dim; i++) vect[i]*=fac;
}

inline void NormalizeL1(vector<float>& vect)
{
	float acc=0;
	for (unsigned int i=0; i<vect.size(); i++)  acc += vect[i];
	for (unsigned int i=0; i<vect.size(); i++)  vect[i] /= acc;
}

inline void NormalizeL1Sqrt(vector<float>& vect)
{
	float acc=0;
	for (unsigned int i=0; i<vect.size(); i++)  acc += vect[i];
	for (unsigned int i=0; i<vect.size(); i++)  vect[i] = sqrt(vect[i]/acc);
}

inline bool Clip(float* v, const int dim, const float threshold)
{
	bool changed = false;
	for (int i=0; i<dim; i++)
	{
		if (v[i]>threshold)
		{
			v[i]=threshold;
			changed = true;
		}
	}
	return changed;
}

void HistogramStats(const float* src, float* stats, const int N);


///////////////////////////////////////////////////////////////////////////////
// Functions: Vectors
///////////////////////////////////////////////////////////////////////////////
inline void VectorMean(const float* v1, const float* v2, float* v3, const int dim) {for (int i=0; i<dim; i++) v3[i] = (v1[i]+v2[i])*0.5f;}
inline void VectorMean(const vector<float*> v, float* vOut, const int dim)
{
	for (int d=0; d<dim; d++)
	{
		float r=0;
		for (unsigned int i=0; i<v.size(); i++) r += v[i][d];
		vOut[d] = r/(float)v.size();
	}
}
inline int VectorPosmax(const float* v, const int dim)
{
	int maxPos=0;
	for (int k=0; k<dim; k++) if (v[k]>v[maxPos]) maxPos = k;
	return maxPos;
}
inline float Vectormax(const float* v, const int dim) {return v[VectorPosmax(v, dim)];}
inline bool VectorEqual(const float* v1, const float* v2, const int dim)
{
	for (int i=0; i<dim; i++) if (v1[i] != v2[i]) return false;
	return true;
}





inline int Min(const vector<float>& vValues, float& minValue)
{
	// 1. Auxiliar variables
	int minPos = -1;
	minValue = FLT_MAX;

	// 2. Look for the minimum value
	for (unsigned int i=0; i<vValues.size(); i++)
	{
		if (vValues[i] < minValue)
		{
			minValue = vValues[i];
			minPos = i;
		}
	}

	// 3. Return the nearest position
	return minPos;
}

inline int Max(const vector<float>& vValues, float& maxValue)
{
	// 1. Auxiliar variables
	int maxPos = -1;
	maxValue = -FLT_MAX;

	// 2. Look for the maximum value
	for (unsigned int i=0; i<vValues.size(); i++)
	{
		if (vValues[i] > maxValue)
		{
			maxValue = vValues[i];
			maxPos = i;
		}
	}

	// 3. Return the nearest position
	return maxPos;
}

inline void Add(vector<float>& vValues, float value) {for (unsigned int i=0; i<vValues.size(); i++) vValues[i] += value;}
inline void Mult(vector<float>& vValues, float value) {for (unsigned int i=0; i<vValues.size(); i++) vValues[i] *= value;}

inline int FindNearestValue(const vector<float>& vValues, const float valueToFind)
{
	// 1. Auxiliar variables
	int minPos = -1;
	float minValue = FLT_MAX;

	// 2. 
	for (unsigned int i=0; i<vValues.size(); i++)
	{
        if (fabs(vValues[i]-valueToFind) < minValue)
		{
            minValue = fabs(vValues[i]-valueToFind);
			minPos = i;
		}
	}

	// 3. Return the nearest position
	return minPos;
}

inline void MaxCon(const vector<float>& input, vector<float>& output, const float minValue, const float maxValue) 
{
	// Retrieve maximum and minimum
	float maxi = -1000;
	float mini = 1000;
	for (unsigned int i=0; i<input.size(); i++)
	{
		if (input[i] > maxi)
			maxi = input[i];

		if (input[i] < mini)
			mini = input[i];
	}

	// Normalize
	for (unsigned int i=0; i<input.size(); i++)
		output[i] =((maxValue-minValue)/(maxi-mini)) * (input[i]-mini);
}

///////////////////////////////////////////////////////////////////////////////
// Functions: ROI's union, intersection...
///////////////////////////////////////////////////////////////////////////////
//inline float RoiArea(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2)
//{
//    return abs((roi01x2-roi01x1)*(roi01y2-roi01y1));
//}
//
//inline float RoiIntersection(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2, 
//							 const float roi02x1, const float roi02x2, const float roi02y1, const float roi02y2)
//{
//	return (max(0.0f, min(roi01x2, roi02x2) - max(roi01x1, roi02x1))) *
//		   (max(0.0f, min(roi01y2, roi02y2) - max(roi01y1, roi02y1)));
//}
//
//inline float RoiUnion(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2, 
//					  const float roi02x1, const float roi02x2, const float roi02y1, const float roi02y2)
//{
//    float intersect = RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
//    float roi01Area = RoiArea(roi01x1, roi01x2, roi01y1, roi01y2);
//    float roi02Area = RoiArea(roi02x1, roi02x2, roi02y1, roi02y2);
//
//    return (roi01Area + roi02Area - intersect);
//}
//
//inline float RoiMinArea(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2, 
//					    const float roi02x1, const float roi02x2, const float roi02y1, const float roi02y2)
//{
//    float roi01Area = RoiArea(roi01x1, roi01x2, roi01y1, roi01y2);
//    float roi02Area = RoiArea(roi02x1, roi02x2, roi02y1, roi02y2);
//
//    return min(roi01Area, roi02Area);
//}
//
//inline float RoiOverlappingPascal(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2, 
//								const float roi02x1, const float roi02x2, const float roi02y1, const float roi02y2)
//{
//	float intersection = RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
//	float uniona = RoiUnion(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
//	return (uniona==0) ? 0.0f : intersection/uniona;
//}
//
//inline float RoiOverlappingTUD(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2, 
//							const float roi02x1, const float roi02x2, const float roi02y1, const float roi02y2)
//{
//	float intersection = RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
//	float minArea = RoiMinArea(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
//	return (minArea==0) ? 0.0f : intersection/minArea;
//}
//
//inline float RoiOverlappingPedro(const float roi01x1, const float roi01x2, const float roi01y1, const float roi01y2, 
//							const float roi02x1, const float roi02x2, const float roi02y1, const float roi02y2)
//{
//	float intersection = RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
//	float refArea = RoiArea(roi01x1, roi01x2, roi01y1, roi01y2);
//	return (refArea==0) ? 0.0f : intersection/refArea;
//}
inline int RoiArea(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2)
{
    return abs((roi01x2-roi01x1)*(roi01y2-roi01y1));
}

inline int RoiIntersection(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2, 
							 const int roi02x1, const int roi02x2, const int roi02y1, const int roi02y2)
{
	return (max(0, min(roi01x2, roi02x2) - max(roi01x1, roi02x1))) *
		   (max(0, min(roi01y2, roi02y2) - max(roi01y1, roi02y1)));
}

inline int RoiUnion(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2, 
					  const int roi02x1, const int roi02x2, const int roi02y1, const int roi02y2)
{
    int intersect = RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
    int roi01Area = RoiArea(roi01x1, roi01x2, roi01y1, roi01y2);
    int roi02Area = RoiArea(roi02x1, roi02x2, roi02y1, roi02y2);

    return (roi01Area + roi02Area - intersect);
}

inline int RoiMinArea(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2, 
					    const int roi02x1, const int roi02x2, const int roi02y1, const int roi02y2)
{
    int roi01Area = RoiArea(roi01x1, roi01x2, roi01y1, roi01y2);
    int roi02Area = RoiArea(roi02x1, roi02x2, roi02y1, roi02y2);

    return min(roi01Area, roi02Area);
}

inline float RoiOverlappingPascal(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2, 
								const int roi02x1, const int roi02x2, const int roi02y1, const int roi02y2)
{
	float intersection = (float) RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
	float uniona       = (float) RoiUnion(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
	return (uniona==0) ? 0.0f : intersection/uniona;
}

inline float RoiOverlappingTUD(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2, 
							const int roi02x1, const int roi02x2, const int roi02y1, const int roi02y2)
{
	float intersection = (float) RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
	float minArea      = (float) RoiMinArea(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
	return (minArea==0) ? 0.0f : intersection/minArea;
}

inline float RoiOverlappingPedro(const int roi01x1, const int roi01x2, const int roi01y1, const int roi01y2, 
							const int roi02x1, const int roi02x2, const int roi02y1, const int roi02y2)
{
	float intersection = (float) RoiIntersection(roi01x1, roi01x2, roi01y1, roi01y2, roi02x1, roi02x2, roi02y1, roi02y2);
	float refArea      = (float) RoiArea(roi01x1, roi01x2, roi01y1, roi01y2);
	return (refArea==0) ? 0.0f : intersection/refArea;
}


///////////////////////////////////////////////////////////////////////////////
// Functions: Image reference change...
///////////////////////////////////////////////////////////////////////////////
inline bool IsInsideImage(const int x, const int y, const int w, const int h, const int sizeX, const int sizeY) 
	{return (x-w/2>-1 && x+w/2<sizeX && y-h/2>-1 && y+h/2<sizeY);}

inline void CentralToCorners(const float x, const float y, const float w, const float h, int& x1, int& y1, int& x2, int& y2)
{
	// Compute corner points
	x1 = int((x-w*0.5f)+0.5f);
	y1 = int((y-h*0.5f)+0.5f);
	x2 = int((x+w*0.5f)+0.5f)-1;
	y2 = int((y+h*0.5f)+0.5f)-1;
}

inline void CentralToCornersInt(const int x, const int y, const int w, const int h, int& x1, int& y1, int& x2, int& y2)
{
	// Compute corner points
	x1 = int((x-w*0.5f)+0.5f);
	y1 = int((y-h*0.5f)+0.5f);
	x2 = int((x+w*0.5f)+0.5f)-1;
	y2 = int((y+h*0.5f)+0.5f)-1;
}

inline void CornersToCentral(int x1, int y1, int x2, int y2, int& x, int& y, int& w, int& h)
{
	// Compute central points
	w = x2-x1+1;
	h = y2-y1+1;
	x = int(x1+w*0.5f);
	y = int(y1+h*0.5f);
}

inline void CornerSizeToCentral(int x1, int y1, int w, int h, int& x, int& y)
{
	// Compute central points
	x = int(x1+w*0.5f);
	y = int(y1+h*0.5f);
}

inline void PyramidToImage(const int xs, const int ys, const int ws, const int hs, const float s, const int marginPyrX, const int marginPyrY, int& x, int& y, int& w, int& h)
{
	x = int((xs-marginPyrX)*s);
	y = int((ys-marginPyrY)*s);
	w = int(ws*s);
	h = int(hs*s);
}

inline void ImageToPyramid(const int x, const int y, const int w, const int h, const int marginPyrX, const int marginPyrY, const float s, int& xs, int& ys, int& ws, int& hs)
{
	xs = int(x/s+marginPyrX);
    ys = int(y/s+marginPyrY);
	ws = int(w/s);
	hs = int(h/s);
}

inline void CentralToCornersInsideImage(const float x, const float y, const float w, const float h, const int imageSizeX, const int imageSizeY, int& x1, int& y1, int& x2, int& y2)
{
	// Compute corner points
	CentralToCorners(x, y, w, h, x1, y1, x2, y2);
	
	// Restrict to inside the image
	x1 = max(0, x1);
	y1 = max(0, y1);
	x2 = min(imageSizeX-1, x2);
	y2 = min(imageSizeY-1, y2);
}

inline void CentralToCornersInsideImageInt(const int x, const int y, const int w, const int h, const int imageSizeX, const int imageSizeY, int& x1, int& y1, int& x2, int& y2)
{
	// Compute corner points
	CentralToCornersInt(x, y, w, h, x1, y1, x2, y2);
	
	// Restrict to inside the image
	x1 = max(0, x1);
	y1 = max(0, y1);
	x2 = min(imageSizeX-1, x2);
	y2 = min(imageSizeY-1, y2);
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Perspective Projection
///////////////////////////////////////////////////////////////////////////////
inline void ProjectPointWorldToImage(float focalInPixels, int imageCenterX, int imageCenterY, float Xw, float Yw, float Zw, int& Xi, int& Yi)
{
	Xi = int(imageCenterX + focalInPixels * (Xw/Zw));
	Yi = int(imageCenterY + focalInPixels * (Yw/Zw));
}



///////////////////////////////////////////////////////////////////////////////
// Classes: Points2D, Points3D, Matrix
///////////////////////////////////////////////////////////////////////////////
class CPoint2D
{
public:
	///@name Constructors and destructors///@{
	CPoint2D() {}
	~CPoint2D() {}
	CPoint2D(const float x_in, const float y_in) { x=x_in; y=y_in; }
	CPoint2D(const float x_in, const float y_in, const float value_in) { x=x_in; y=y_in; value=value_in; }

	// Atributes
	float x, y, value;
};


class CPoint3D
{
public:
	///@name Constructors and destructors///@{
	CPoint3D() {}
	CPoint3D(const float x_in, const float y_in, const float z_in) { x=x_in; y=y_in; z=z_in; }
	CPoint3D(const float x_in, const float y_in, const float z_in, const float value_in) { x=x_in; y=y_in; z=z_in; value=value_in; }
	CPoint3D(const CPoint2D& point2D, const float z_in) { x=point2D.x; y=point2D.y; z=z_in; }
	~CPoint3D() {}

	// Atributes
	float x, y, z, value;
};


class CMatrix2D
{
public:
	CMatrix2D ():
		m_sizeX(0),
        m_sizeY(0),
        m_pData(NULL)
	{}

	CMatrix2D (const CMatrix2D& copy) { Copy(copy); }
	CMatrix2D (const int sizeX, const int sizeY):
		m_sizeX(sizeX),
        m_sizeY(sizeY),
        m_pData(NULL)
	{
		m_pData = new float*[sizeY];
		for (int y=0; y<sizeY; y++)
		{
			m_pData[y] = new float[sizeX];
			memset(m_pData[y], 0, sizeX*sizeof(float));
		}
	}
	
	~CMatrix2D() { Clear(); }

	void Copy(const CMatrix2D& copy)
	{
		m_sizeX = copy.GetSizeX();
		m_sizeY = copy.GetSizeY();
		float** pDataToCopy = copy.GetData();
		m_pData = new float*[m_sizeY];
		for (int y=0; y<m_sizeY; y++)
		{
			m_pData[y] = new float[m_sizeX];
			memcpy(m_pData[y], pDataToCopy[y], m_sizeX*sizeof(float));
		}
	}

	void Clear()
	{
		if (m_pData!=NULL)
		{
			for (int y=0; y<m_sizeY; y++)
				delete [] m_pData[y];
			delete [] m_pData;
			m_pData = NULL;
		}
	}

	void Transpose()
	{
		CMatrix2D* pAux = new CMatrix2D(*this);

		Clear();
		m_sizeX = pAux->GetSizeY();
		m_sizeY = pAux->GetSizeX();

		m_pData = new float*[m_sizeY];
		for (int y=0; y<m_sizeY; y++)
		{
			m_pData[y] = new float[m_sizeX];
			for (int x=0; x<m_sizeX; x++)
				m_pData[y][x]=pAux->Get(y,x);
		}

		delete pAux;
	}

	void Show(char* name)
	{
		//if (m_sizeX*m_sizeY>0)
		//{
		//	printf("\n");
		//	for (int y=0; y<m_sizeY; y++)
		//	{
		//		for (int x=0; x<m_sizeX; x++)
		//			printf("%f ", Get(x,y));
		//		printf("\n");
		//	}
		//}

		
		if (m_sizeX*m_sizeY>0)
		{
			if (name) cout << "[" << name << "] = " << endl;
			for(int y=0; y<m_sizeY; y++)
			{
				for(int x=0; x<m_sizeX; x++) cout << "\t" << Get(x, y);
				cout << endl;
			}
			cout << endl;
		}
	}

	// Get and Set methods
	float	Get(int x, int y)	const		{return m_pData[y][x];}
	float**	GetData()			const		{return m_pData;}
	int		GetSizeX()			const		{return m_sizeX;}
	int		GetSizeY()			const		{return m_sizeY;}
	void	Set(int x, int y, float value)	{m_pData[y][x]=value;}

// ATTRIBUTES
private:
	int			m_sizeX;
	int			m_sizeY;
	float**		m_pData;
};

#endif
