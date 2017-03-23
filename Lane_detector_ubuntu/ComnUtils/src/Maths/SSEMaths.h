#ifndef __SSE_MATH__H__
#define __SSE_MATH__H__


#include <pmmintrin.h> /* for SSE3 hadd_ps */
#include <immintrin.h> /* AVX + SSE4 intrinsics header */
//#include <stdio.h>
//#include <iostream>
//#include <fstream>
using namespace std;


// Internal class: do not use...
class not_vec4
{
	__m128 v; // bitwise inverse of our value (!!)

public:
	not_vec4(__m128 val)	{ v = val;	}
	__m128 get(void) const	{ return v;	} // returns INVERSE of our value (!!)
};


class vec4
{
public:
	// Constructors
	
	vec4()											{ v = _mm_setzero_ps();				}
	vec4(__m128 val)								{ v = val;							}
	vec4(const float *src)							{ v = _mm_loadu_ps(src);			}
//	vec4(const float *src, bool isAligned)			{ v = _mm_load_ps(src);				}
	vec4(float x)									{ v = _mm_set_ps1(x);				}
	vec4(float x1, float x2, float x3, float x4)	{ v = _mm_setr_ps(x1, x2, x3, x4);	}
	
	// Operators
	vec4	operator+	(const vec4 &rhs)	const	{ return _mm_add_ps		(v, rhs.v);	}
	vec4	operator-	(const vec4 &rhs)	const	{ return _mm_sub_ps		(v, rhs.v);	}
	vec4	operator*	(const vec4 &rhs)	const	{ return _mm_mul_ps		(v, rhs.v);	}
	vec4	operator/	(const vec4 &rhs)	const	{ return _mm_div_ps		(v, rhs.v);	}
	vec4	operator&	(const vec4 &rhs)	const	{ return _mm_and_ps		(v, rhs.v);	}
	vec4	operator|	(const vec4 &rhs)	const	{ return _mm_or_ps		(v, rhs.v);	}
	vec4	operator^	(const vec4 &rhs)	const	{ return _mm_xor_ps		(v, rhs.v);	}
	vec4	operator==	(const vec4 &rhs)	const	{ return _mm_cmpeq_ps	(v, rhs.v);	}
	vec4	operator!=	(const vec4 &rhs)	const	{ return _mm_cmpneq_ps	(v, rhs.v);	}
	vec4	operator<	(const vec4 &rhs)	const	{ return _mm_cmplt_ps	(v, rhs.v);	}
	vec4	operator<=	(const vec4 &rhs)	const	{ return _mm_cmple_ps	(v, rhs.v);	}
	vec4	operator>	(const vec4 &rhs)	const	{ return _mm_cmpgt_ps	(v, rhs.v);	}
	vec4	operator>=	(const vec4 &rhs)	const	{ return _mm_cmpge_ps	(v, rhs.v);	}
	static vec4	sqrt	(const vec4 &lhs)					{ return _mm_sqrt_ps	(lhs.v);				}
	static vec4	rsqrt	(const vec4 &lhs)					{ return _mm_rsqrt_ps	(lhs.v);				}
	static vec4	Min		(const vec4 &lhs, const vec4 &rhs)	{ return _mm_min_ps		(lhs.v, rhs.v);			}
	static vec4	Max		(const vec4 &lhs, const vec4 &rhs)	{ return _mm_max_ps		(lhs.v, rhs.v);			}
	static vec4	dot		(const vec4 &lhs, const vec4 &rhs)	{ return _mm_dp_ps		(lhs.v, rhs.v, 0xF1);	}
	static vec4	dotSSE3	(const vec4 &lhs, const vec4 &rhs)	
	{
		vec4	t = lhs*rhs;
		__m128	vt = _mm_hadd_ps(t.get(), t.get());
		return _mm_hadd_ps(vt, vt);
	}

	__m128	get			(void)				const	{ return v;							}
	void	set			(__m128 val)				{ v = val;							}
	void	set			(const float *src)			{ v = _mm_loadu_ps(src);			}
	void	setAligned	(const float *src)			{ v = _mm_load_ps(src);				}
	void	set			(float x)					{ v = _mm_set_ps1(x);				}
	void	set			(float x1, float x2, float x3, float x4)	{ v = _mm_setr_ps(x1, x2, x3, x4);	}
	float*	store		(float *ptr)				{ _mm_store_ps(ptr,v); return ptr;	}
	float&	operator[]	(int index)					{ return ((float *)&v)[index];		}
	float	operator[]	(int index)			const	{ return ((const float *)&v)[index];}

	friend ostream &operator<<(ostream &o,const vec4 &y)
		{ o<<y[0]<<" "<<y[1]<<" "<<y[2]<<" "<<y[3]; return o;	}

	friend vec4 operator&(const vec4 &lhs, const not_vec4 &rhs)
		{ return _mm_andnot_ps(rhs.get(), lhs.get());			}

	friend vec4 operator&(const not_vec4 &lhs, const vec4 &rhs)
		{ return _mm_andnot_ps(lhs.get(), rhs.get());			}

	vec4 if_then_else(const vec4 &then, const vec4 &else_part) const 
		{ return _mm_or_ps( _mm_and_ps(v,then.v), _mm_andnot_ps(v, else_part.v)); }

//	static void Gradient(const float* vImg, float*vMag, float*vOri, const unsigned int sizeX, const unsigned int sizeY)
//	{
//		// Auxiliar variables
////		float output = 0.0f;
//		vec4 x0(0.0f);
//		vec4 x2(0.0f);
//		vec4 y0(0.0f);
//		vec4 y2(0.0f);
//		vec4 dx(0.0f);
//		vec4 dy(0.0f);
//		vec4 mag(0.0f);
//		vec4 ori(0.0f);

//		for (unsigned int y=1; y<sizeY-1; y++)
//		{
//			// Set initial pointers to the image data
//			const float* pX0 = &vImg[y*sizeX];
//			const float* pX2 = &vImg[y*sizeX+2];
//			const float* pY0 = &vImg[(y-1)*sizeX];
//			const float* pY2 = &vImg[(y+1)*sizeX];
//			float* pMag= &vMag[y*sizeX];

//			// Compute first two by elements
//			for (unsigned int i=0; i<3; i++)
//			{
//				float dx = pX2[i] - pX0[i];
//				float dy = pY2[i] - pY0[i];
//				pMag[i+1] = std::sqrt(dx*dx + dy*dy);
//			}

//			// Compute gradient by blocks
//			for (unsigned int x=1; x<(sizeX-2)/nElements; x++)
//			{
//				x0.set(&pX0[x*nElements]);
//				x2.set(&pX2[x*nElements]);
//				y0.set(&pY0[x*nElements]);
//				y2.set(&pY2[x*nElements]);
//				dx = x2 - x0;
//				dy = y2 - y0;
//				mag= sqrt(dx*dx + dy*dy);
//				mag.store(&pMag[x*nElements]);
//				//ori = atan2(dy, dx);
//				//orientation.SetPixel(x, y, RadToDeg(mod2PI(FastAtan2(dy, dx) + 2*PI)));
//			}

//			// Compute the rest by elements
//			if ((sizeX-2)%nElements != 0)
//			for (unsigned int i=(sizeX-2)-(sizeX-2)%nElements; i<(sizeX-2); i++)
//			{
//				float dx = pX2[i] - pX0[i];
//				float dy = pY2[i] - pY0[i];
//				pMag[i]  = std::sqrt(dx*dx + dy*dy);
//			}
//			pMag[-1] = pMag[0];
//			pMag[sizeX-2] = pMag[sizeX-3];
//		}
//	}

	static float Dot(const float* vA, const float*vB, const unsigned int size)
	{
		// Auxiliar variables
		float output = 0.0f;
		vec4 a(0.0f);
		vec4 b(0.0f);

		// Compute by blocks
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.set(&vA[nElements*i]);
			b.set(&vB[nElements*i]);
			output += dot(a,b)[0];
		}

		// Compute the rest by elements
		if (size%nElements != 0) 
		for (unsigned int i=size-size%nElements; i<size; i++)
			output += vA[i] * vB[i];

		// Return the value
		return output;
	}

	static float SumSqr(const float* vA, const unsigned int size)
	{
		// Auxiliar variables
		float output = 0.0f;
		vec4 a(0.0f);

		// Compute by blocks
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.set(&vA[nElements*i]);
			output += dot(a,a)[0];
		}

		// Compute the rest by elements
		for (unsigned int i=size-size%nElements; i<size; i++)
			output += vA[i] * vA[i];

		// Return the value
		return output;
	}

	static float Sum(const float* vA, const unsigned int size)
	{
		// Auxiliar variables
		float output;
		vec4 a(0.0f);
		vec4 c(0.0f);

		// Compute by blocks
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.set(&vA[nElements*i]);
			c = c + a;
		}
		output = c[0] + c[1] + c[2] + c[3];

		// Compute the rest by elements
		if (size%nElements != 0) 
		for (unsigned int i=size-size%nElements; i<size; i++)
			output += vA[i];

		// Return the value
		return output;
	}

	static void Mult(float* vA, const float value, const unsigned int size)
	{
		// Auxiliar variables
		vec4 a(0.0f);
		vec4 b(value);

		// Compute by blocks
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.set(&vA[nElements*i]);
			a = a*b;
		}

		// Compute the rest by elements
		if (size%nElements != 0) 
		for (unsigned int i=size-size%nElements; i<size; i++)
			vA[i] *= value;
	}

	static float MultClipSum(float* vA, const float valueMult, const float valueClip, const unsigned int size)
	{
		// Auxiliar variables
		float output = 0.0f;
		vec4 a(0.0f);
		vec4 b(valueMult);
		vec4 c(valueClip);

		// Compute by blocks
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.set(&vA[nElements*i]);
			a = Min(a*b, c);
			output += dot(a,a)[0];
		}

		// Compute the rest by elements
		for (unsigned int i=size-size%nElements; i<size; i++)
		{
			vA[i] = min(vA[i]*valueMult, valueClip);
			output += vA[i]*vA[i];
		}

		// return output
		return output;
	}

	static float Mean(const float* vA, const unsigned int size)
	{
		return Sum(vA, size)/size;
	}

	static void Sqrt(float* vA, const unsigned int size)
	{
		// Auxiliar variables
		vec4 a(0.0f);

		// Compute by blocks
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.setAligned(&vA[nElements*i]);
			sqrt(a).store(&vA[nElements*i]);
		}

		// Compute the rest by elements
		if (size%nElements != 0) 
		for (unsigned int i=size-size%nElements; i<size; i++)
			vA[i] = std::sqrt(vA[i]);
	}

// Atributes
public:
	__m128				v;				// Value
	static const int	nElements = 4;	// number of elements
};


// Internal class: do not use...
class not_vec8
{
	__m256 v; // bitwise inverse of our value (!!)

public:
	not_vec8(__m256 val)	{ v = val;	}
	__m256 get(void) const	{ return v;	} // returns INVERSE of our value (!!)
};

// This is the class to use!
class vec8
{
	__m256 v;

public:
	// Constructors
	vec8(__m256 val)									{ v=val;										}
	vec8(const float *src)								{ v=_mm256_loadu_ps(src);						}
	vec8(float x)										{ v=_mm256_broadcast_ss(&x);					}
	
	// Operations
	vec8		operator+	(const vec8 &rhs)	const	{ return _mm256_add_ps	(v,rhs.v);				}
	vec8		operator-	(const vec8 &rhs)	const	{ return _mm256_sub_ps	(v,rhs.v);				}
	vec8		operator*	(const vec8 &rhs)	const	{ return _mm256_mul_ps	(v,rhs.v);				}
	vec8		operator/	(const vec8 &rhs)	const	{ return _mm256_div_ps	(v,rhs.v);				}
	vec8		operator&	(const vec8 &rhs)	const	{ return _mm256_and_ps	(v,rhs.v);				}
	vec8		operator|	(const vec8 &rhs)	const	{ return _mm256_or_ps	(v,rhs.v);				}
	vec8		operator^	(const vec8 &rhs)	const	{ return _mm256_xor_ps	(v,rhs.v);				}
	vec8		operator==	(const vec8 &rhs)	const	{ return _mm256_cmp_ps	(v,rhs.v,_CMP_EQ_OQ);	}
	vec8		operator!=	(const vec8 &rhs)	const	{ return _mm256_cmp_ps	(v,rhs.v,_CMP_NEQ_OQ);	}
	vec8		operator<	(const vec8 &rhs)	const	{ return _mm256_cmp_ps	(v,rhs.v,_CMP_LT_OQ);	}
	vec8		operator<=	(const vec8 &rhs)	const	{ return _mm256_cmp_ps	(v,rhs.v,_CMP_LE_OQ);	}
	vec8		operator>	(const vec8 &rhs)	const	{ return _mm256_cmp_ps	(v,rhs.v,_CMP_GT_OQ);	}
	vec8		operator>=	(const vec8 &rhs)	const	{ return _mm256_cmp_ps	(v,rhs.v,_CMP_GT_OQ);	}
	not_vec8	operator~	(void)				const	{ return not_vec8		(v);					}
	vec8		sqrt		()					const	{ return _mm256_sqrt_ps	(v);					}
	vec8		rsqrt		()					const	{ return _mm256_rsqrt_ps(v);					}
	static vec8	dot		(const vec8 &lhs, const vec8 &rhs)	{ return _mm256_dp_ps		(lhs.v, rhs.v, 0xFF);	}
	vec8		dotSSE3		(const vec8 &b)		const
	{
		vec8 t = *this*b;
		__m256 vt=_mm256_hadd_ps(t.get(), t.get());
		return _mm256_hadd_ps(vt, vt);
	}

	// Access to the elements
	__m256		get			(void)				const	{ return v;										}
	void		set			(__m256 val)				{ v = val;										}
	void		set			(const float *src)			{ v = _mm256_loadu_ps		(src);				}
	void		set			(float x)					{ v = _mm256_broadcast_ss	(&x);				}
	float*		store		(float *ptr)				{ _mm256_store_ps			(ptr,v); return ptr;}
	float&		operator[]	(int index)					{ return ((float*)&v)[index];					}
	float		operator[]	(int index)			const	{ return ((const float*)&v)[index];				}

	friend ostream &operator<<(ostream &o, const vec8 &y)
		{ o<<y[0]<<" "<<y[1]<<" "<<y[2]<<" "<<y[3];	return o; }

	friend vec8 operator&(const vec8 &lhs, const not_vec8 &rhs)
		{ return _mm256_andnot_ps(rhs.get(),lhs.get());	}

	friend vec8 operator&(const not_vec8 &lhs, const vec8 &rhs)
		{ return _mm256_andnot_ps(lhs.get(),rhs.get());	}

	vec8 if_then_else(const vec8 &then, const vec8 &else_part) const
		{ return _mm256_or_ps( _mm256_and_ps(v,then.v), _mm256_andnot_ps(v, else_part.v));	}

	static float Dot(const float* vA, const float*vB, const unsigned int size)
	{
		float output = 0.0f;

		vec8 a(0.0f);
		vec8 b(0.0f);

		// Compute by blocks
		if (size/nElements != 0)
		for (unsigned int i=0; i<size/nElements; i++)
		{
			a.set(&vA[nElements*i]);
			b.set(&vB[nElements*i]);
			//output += dot(a,b)[0];
			output += a.dotSSE3(b)[0];
		}

		// Compute the rest by elements
		if (size%nElements != 0) 
		for (unsigned int i=size-size%nElements; i<size; i++)
			output += vA[i] * vB[i];

		// Return the value
		return output;
	}

	private:
	static const int	nElements = 8;	// number of elements
};

#endif //__SSE_MATH__H__


