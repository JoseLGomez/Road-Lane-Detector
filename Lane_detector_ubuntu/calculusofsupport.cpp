// CalculusOfSupport.cpp: implementation of the CCalculusOfSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "calculusofsupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CCalculusOfSupport::Intersection(float * line1, float * line2, SPixel & pixel)
{
    float intersectionLine[3];

    // Vectorial product
    ExteriorProduct(line1,line2,intersectionLine);

    // Normalization to go from homogeneous coordinates to euclidean ones (image)
    pixel.x = intersectionLine[0]/intersectionLine[2];
    pixel.y = intersectionLine[1]/intersectionLine[2];
    //we should: if(fabs(intersectionLine[2])>EPSILON) throw ...;
}

void CCalculusOfSupport::ExteriorProduct(float * input1, float * input2, float * result)
{
    result[0] = input1[1] * input2[2] - input1[2] * input2[1];
    result[1] = input1[2] * input2[0] - input1[0] * input2[2];
    result[2] = input1[0] * input2[1] - input1[1] * input2[0];
}

float CCalculusOfSupport::EscalarProduct(float * input1, float * input2)
{
    return (input1[0]*input2[0]+input1[1]*input2[1]+input1[2]*input2[2]);
}

double CCalculusOfSupport::GaussianRandom(double a,double d)
{
    static double t = 0.0;
    double x,v1,v2,r,u1,u2;
    if (t == 0) {
        do {
            u1 = ((2.0*rand())/RAND_MAX)-1;
            u2 = ((2.0*rand())/RAND_MAX)-1;
            v1 = 2.0 * u1 - 1.0;
            v2 = 2.0 * u2 - 1.0;
            r = v1 * v1 + v2 * v2;
        } while (r>=1.0);
    r = sqrt((-2.0*log(r))/r);
    t = v2*r;
    return(a+v1*r*d);
    }
    else {
        x = t;
        t = 0.0;
        return(a+x*d);
    }
}

double CCalculusOfSupport::WhiteRandom(double mean, double variance)
{
    return ((mean-variance) + ((2.0*variance*rand()) / RAND_MAX));
}
