/* ********************************* FILE ************************************/
/** @file		vector3D.h
 *
 *	@ingroup	ComnUtils
 * 
 *	@brief		This file describes the declaration of the class Cvector3D
 * 
 *  @author		Angel D. Sappa (asappa@cvc.uab.es)
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	@note		(C) Copyright CVC-UAB, ADAS
 * 
 *****************************************************************************/
#ifndef _VECTOR_3D_
#define _VECTOR_3D_

#if _MSC_VER > 1000
	#pragma once
#endif


/*****************************************************************************
 * INCLUDE FILES
 *****************************************************************************/
#include <math.h>
#include <vector>
using std::vector;

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
 *	@brief		It defines the class Cvector3D
 *
 *  @author		Angel D. Sappa (asappa@cvc.uab.es)
 *	@author		David Geronimo (dgeronimo@cvc.uab.es)
 *	@author		David Vazquez (David.Vazquez@cvc.uab.es)
 *
 *	@date		Mar 24, 2012
 *	\sa			-
 *****************************************************************************/
class FVector
{
public:
	float	x, y, z;

    FVector(const float i =0.0f, const float j=0.0f, const float k=0.0f):x(i), y(j), z(k) {}
    FVector(const FVector &v) { x = v.x; y = v.y; z = v.z; }
    ~FVector(){}

	FVector& operator=(const FVector &right);
	FVector& operator+=(const FVector &right);
	FVector& operator-=(const FVector &right);
	FVector& operator()(const float vx, const float vy, const float vz);
	friend FVector operator+(const FVector &v1, const FVector &v2);
	friend FVector operator-(const FVector &v1, const FVector &v2);	 // <- change for a - once service pack has been installed!
	friend FVector operator^(const FVector &v1, const FVector &v2);
	friend FVector operator/(const FVector &v1, const float d);
	friend FVector operator/(const FVector &v1, const int d);
	friend FVector operator*(const float d, const FVector &v1);
	friend FVector operator*(const FVector &v1, const float d);
	friend double  operator*(const FVector &v1, const FVector &v2);
	friend FVector cross(const FVector &v1, const FVector &v2);
	friend double dot(const FVector &v1, const FVector &v2, const bool inputsNormalised=false);
	friend double len(const FVector &v1);
	friend FVector normalise(FVector &v1);
	friend double angle(const FVector v1,const  FVector v2);
};

class FPoint3D
{
public:

	FPoint3D():x(0),y(0),z(0),r(0),g(0),b(0),index(0),valid(false),filtered(false) {}
	FPoint3D(const float x, const float y, const float z):x(x),y(y),z(z),r(0),g(0),b(0),index(0),valid(false),filtered(false) {}

	float	x, y, z;
	unsigned char r, g, b;
	int		index;
	bool	valid;
	bool	filtered;
};

class IPoint2D
{
public:
	int		i, j;
};

class Pts3D
{
public:
	vector<FPoint3D> pt;
	vector<IPoint2D> pt2D;
};
#endif //_VECTOR_3D_
