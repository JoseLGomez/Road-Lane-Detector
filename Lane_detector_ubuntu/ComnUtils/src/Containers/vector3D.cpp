#include "vector3D.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// Functions: Operators
///////////////////////////////////////////////////////////////////////////////
FVector& FVector::operator=(const FVector &right)
{
	if(&right == this)
		return *this;
	x = right.x;
	y = right.y;
	z = right.z;
	return *this;

}

FVector& FVector::operator+=(const FVector &right)
{
	if(&right == this) return *this;

	x += right.x;
	y += right.y;
	z += right.z;
	return *this;
}

FVector& FVector::operator-=(const FVector &right)
{
	if(&right == this) return *this;

	x -= right.x;
	y -= right.y;
	z -= right.z;
	return *this;
}

FVector& FVector::operator()(const float vx, const float vy, const float vz)
{
	x = vx;
	y = vy;
	z = vz;

	return *this;
}

FVector operator+(const FVector &v1, const FVector &v2)
{
	return FVector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

FVector operator-(const FVector &v1, const FVector &v2)	 // <- change for a - once service pack has been installed!
{
	return FVector(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}	

FVector operator^(const FVector &v1, const FVector &v2) 
{
	return FVector(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
}

FVector operator/(const FVector &v1, const float d)
{
	return FVector(v1.x/d, v1.y/d, v1.z/d);
}

FVector operator/(const FVector &v1, const int d)
{
	return FVector(v1.x/d, v1.y/d, v1.z/d);
}

FVector operator*(const float d, const FVector &v1)
{
	return FVector(v1.x*d, v1.y*d, v1.z*d);
}

FVector operator*(const FVector &v1, const float d)
{
	return FVector(v1.x*d, v1.y*d, v1.z*d);
}

double operator*(const FVector &v1, const FVector &v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
} 

FVector cross(const FVector &v1, const FVector &v2)
{
	return FVector(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Others
///////////////////////////////////////////////////////////////////////////////
// The maximum return values are in the range -1 < n <=1
double dot(const FVector &v1, const FVector &v2, const bool inputsNormalised)
{
	double result = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;

	if(inputsNormalised == true) {
		result = result > 1.0 ? 1.0 : result;
		result = result < -1.0 ? -1.0 : result;
	}

	return result;
} 

double len(const FVector &v1)
{
	return (sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z));
}

FVector normalise(FVector &v1)
{
	float ln = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
	if (ln != 0)
	{
		v1.x = v1.x/ln;
		v1.y = v1.y/ln;
		v1.z = v1.z/ln;
	}
	return v1;
}	

double angle(const FVector v1,const  FVector v2)
{
	// changed to pass constant vectors and cast(copy constructor called) to isolate 
	// constant vector from normalise this may have speed penalties ?
	FVector va=v1;
	FVector vb=v2;
	return acos( dot(normalise(va), normalise(vb), true) );
}
