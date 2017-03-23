// CalculusOfSupport.h: interface for the CCalculusOfSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULUSOFSUPPORT_H__A850A0C7_6DAD_413C_BA64_84E060878982__INCLUDED_)
#define AFX_CALCULUSOFSUPPORT_H__A850A0C7_6DAD_413C_BA64_84E060878982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <iostream>

/// Global Variables

using namespace std;

struct SLandscapePixel
{
    float x;
    float y;
    float value;
};

struct SPixel
{
    float x;
    float y;
};

typedef enum
{
    etol_unknown=-1,
    etol_whiteLight,
    etol_redLight
} ETypeOfHeadlight;

const double c_fromDegreesToRadians = 0.017453292519943295769236907684886; // Aprox. de Pi/180

class CCalculusOfSupport
{
public:
    static void ExteriorProduct(float * input1, float * input2, float * result);
    static void Intersection(float * line1, float * line2, SPixel & pixel);
    static float EscalarProduct(float * input1, float * input2);

    double GaussianRandom(double a,double d);
    double WhiteRandom(double mean, double variance);
};

#endif // !defined(AFX_CALCULUSOFSUPPORT_H__A850A0C7_6DAD_413C_BA64_84E060878982__INCLUDED_)
