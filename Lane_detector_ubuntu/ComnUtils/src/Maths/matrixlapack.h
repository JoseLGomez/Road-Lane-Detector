///////////////////////////////////////////////////////////////////////////////
//   MatrixLapack.h: interface for the Maths library
// 
//   Author      : Dani Ponsa (daniel@cvc.uab.es)
//   Modified by : David Vazquez (David.Vazquez@cvc.uab.es)
//   Modified    : 2008/08/26
//
//   Description : It is the Maths library.
//
//	 TODO:
//
///////////////////////////////////////////////////////////////////////////////	

#ifndef MATRIX_LAPACK_H
#define MATRIX_LAPACK_H

#if _MSC_VER > 1000
	#pragma once
#endif



#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "clapack.h"
#undef ddot_	// (by david G.) to avoid note C6311
#undef dgetrf_
#undef dgetri_
#undef dposv_
#undef dpotrf_
#undef dpodi_
#undef dgemm_

#define	ddot_	sdot_
#define	dgetrf_ sgetrf_
#define dgetri_ sgetri_
#define dposv_	sposv_
#define dpotrf_	spotrf_
#define	dpodi_	spodi_
#define dgemm_	sgemm_

template<int M, int N> class CMatrix
{
	public:
		float m_staticData[N][M];				
		float *m_data[N];
	public:						
		const int m_m, m_n;

		CMatrix() : m_m(M), m_n(N){
			memset(m_staticData,0,M*N*sizeof(float));
			for (int i=0;i<N; i++) m_data[i] = &m_staticData[i][0];			
		};

//	Versió vella
//		CMatrix(float pExternalData[M]) : m_m(M), m_n(N) {
//			for (int i=0;i<N; i++) m_data[i] = &pExternalData[i];					};

		CMatrix(float pExternalData[N*M]) : m_m(M), m_n(N) {
			for (int i=0;i<N; i++) m_data[i] = &pExternalData[i*M];	};

        CMatrix(float pExternalData[N][M]) : m_m(M), m_n(N) {
			for (int i=0;i<N; i++) m_data[i] = &pExternalData[i][0];			
		};	
		
		CMatrix(const CMatrix& obj) : m_m(M), m_n(N)
		{
			assert( this->m_m==obj.m_m);
			assert( this->m_n==obj.m_n);

			// Is essential to copy from m_data (not m_staticData) because if obj
			// was a matrix with external data, trash data would be copied.
			memcpy(this->m_staticData, obj.m_data[0], M*N*sizeof(float));

			for (int i=0;i<N; i++) m_data[i] = &m_staticData[i][0];
		}

//		float *operator [](int i) {
//			return m_data[i];			
//		};


		void	Clean()
		{
			memset(m_staticData,0,M*N*sizeof(float));
		}

		float&	operator()(int row, int col = 0)
		{
			return m_data[col][row];
		};

		float&	operator()(int row, int col = 0) const
		{
			return m_data[col][row];
		};

		CMatrix&	operator=(const CMatrix& obj)
		{
			if(this!=&obj)
			{
				assert( this->m_m==obj.m_m);
				assert( this->m_n==obj.m_n);

				memcpy(this->m_staticData, obj.m_data[0], M*N*sizeof(float));
				// Com que és un array estàtic, no cal reapuntar m_data.
			}
			return *this;
		}

		bool	operator==(const CMatrix& obj)
		{
			int	i;
			bool equals;
			float*	pData1;
			float*	pData2;

			i = 0;
            equals = true;

			pData1 = m_data[0];
			pData2 = obj.m_data[0];

			while((i<N*M)&&(*pData1++==*pData2++))
			{
				i++;
			}
			return (i==N*M);	
		}

		bool	operator!=(const CMatrix& obj) 	{ return !(*this==obj); }
		
		const float *GetPointer() { return &m_data[0][0]; };

		const float *GetPointer() const { return &m_data[0][0]; };
};

// Following functions should be codified making use of the
// BLAS library in order to gain efficiency.

// Note that care should be taken, to perform correctly even if 
// the matrix manages external data.

template <int M, int N> 
inline void Set(const float &a,CMatrix<M,N> &r) {	
	for (int i=0; i<M; i++) 
		for (int j=0; j<N; j++)
			r(i,j) = a;
}


template <int M, int N> 
inline void Set(const CMatrix<M,1>& vec,CMatrix<M,N> &r) {	

	int	i;
	int	j;

	float*	pVec;
	float*	pR;

	pVec = &vec(0,0);
	for(i=0;i<M;i++)
	{
		pR = &r(i,0);
		for(j=0;j<N;j++)
		{
			*pR = *pVec;
			pR += M;
		}
		pVec++;
	}
}

template <int M, int N> 
inline void Copy(const float a[M*N],CMatrix<M,N> &r) {
	memcpy(&r(0,0),&a[0],M*N*sizeof(float));
}

template <int M, int N> 
inline void Copy(const CMatrix<M,N> &a,CMatrix<M,N> &r) {
	for (int i=0; i<M; i++) 
		for (int j=0; j<N; j++)
			r(i,j) = a(i,j);
}


template <int M, int N> 
inline void Add(const CMatrix<M,N> &a,const CMatrix<M,N> &b, CMatrix<M,N> &r) {
	for (int i=0; i<M; i++) 
		for (int j=0; j<N; j++)
			r(i,j) = a(i,j) + b(i,j);
}


template <int M, int N> 
inline void AddVector(const CMatrix<M,N> &a,const CMatrix<M,1> &vec, CMatrix<M,N> &r) {
	for (int i=0; i<M; i++) 
		for (int j=0; j<N; j++)
			r(i,j) = a(i,j) + vec(i,0);
}


template <int M, int N> 
inline void Subtract(const CMatrix<M,N> &a,const CMatrix<M,N> &b, CMatrix<M,N> &r) {
	for (int i=0; i<M; i++) 
		for (int j=0; j<N; j++)
			r(i,j) = a(i,j) - b(i,j);
}


template <int M, int N> 
inline void MultScalar(const CMatrix<M,N> &a, float b, CMatrix<M,N> &r) {
	for (int i=0; i<M; i++) {
		for (int j=0; j<N; j++)  {
			r(i,j) = b * a(i,j);
		}
	}
}

template <class TA, class TB, class TR> 
void Mult(TA &a, TB &b, TR &r)
 {
	assert( a.GetPointer()!=r.GetPointer());
	assert( b.GetPointer()!=r.GetPointer());
	assert( a.m_n==b.m_m);
	assert( a.m_m == r.m_m);
	assert( b.m_n == r.m_n);
	for (int i=0; i<a.m_m; i++) {
		for (int j=0; j<b.m_n; j++)  {
			r(i,j) = 0.0f;	
			for (int n=0; n<a.m_n; n++) {
				r(i,j) += a(i,n) * b(n,j);
			}
		}
	}

//	char trans = 'N';
//	integer m = a.m_m;
//	integer k = a.m_n;
//	integer n = b.m_n;
//	float alpha = 1.0;
//	float	beta = 0;
//	integer incX = 1;

//	dgemm_(&trans,&trans,&m,&n,&k,&alpha,&a(0,0),&m,&b(0,0),&k,&beta,&r(0,0),&m,0,0);



}

// Multiplica TA*TB'
template <class TA, class TB, class TR> 
inline void MultTrans(TA &a, TB &b, TR &r) {
	assert( a.GetPointer()!=r.GetPointer());
	assert( b.GetPointer()!=r.GetPointer());
	assert( a.m_n==b.m_n);
	assert( a.m_m == r.m_m);
	assert( b.m_m == r.m_n);
	for (int i=0; i<a.m_m; i++) {
		for (int j=0; j<b.m_m; j++)  {
			r(i,j) = 0;	
			for (int n=0; n<a.m_n; n++) {
				r(i,j) += (a(i,n) * b(j,n));
			}
		}
	}
}

// Multiplica TA'*TB
template <class TA, class TB, class TR> 
inline void TransMult(TA &a, TB &b, TR &r) {
	assert( a.GetPointer()!=r.GetPointer());
	assert( b.GetPointer()!=r.GetPointer());
	assert( a.m_m==b.m_m);
	assert( a.m_n == r.m_m);
	assert( b.m_n == r.m_n);
	for (int i=0; i<a.m_n; i++) {
		for (int j=0; j<b.m_n; j++)  {
			r(i,j) = 0;	
			for (int n=0; n<a.m_m; n++) {
				r(i,j) += a(n,i) * b(n,j);
			}
		}
	}
}


template <int M, int N>
inline void SetIdentity(CMatrix<M,N> &r) {
	assert(M == N);
	memset(&r(0,0), (int)0.0f, N*N*sizeof(float));
	for (int i=0;i<N;i++) r(i,i) = 1;
}

//// <vec1,vec2> = vec1'*vec2
//template <int M>
//inline float DotProduct(CMatrix<M,1>& vec1, CMatrix<M,1>& vec2) 
//{
//	integer		n;
//	integer		incX;
//
//	n = M;
//	incX = 1;
//	return ddot_(&n,&vec1(0,0),&incX,&vec2(0,0),&incX);
//}

// <vec1,vec2> = vec1'*metric*vec2
//template <int M, int N>
//inline float DotProduct(CMatrix<M,1>& vec1, CMatrix<M,1>& vec2, CMatrix<M,N>& metric) 
//{
//	assert(M == N);
//
//	CMatrix<M,1>	tmp;
//	float			dotProduct;
//
//	Mult(metric,vec2,tmp);
//
//	integer		n;
//	integer		incX;
//
//	n = M;
//	incX = 1;
//	return ddot_(&n,&vec1(0,0),&incX,&tmp(0,0),&incX);
//}

template <int M, int N>
inline void Inverse(CMatrix<M,N> &a, CMatrix<M,N> &r)
{
	//#ifdef VOLKSWAGEN
		assert(M == N);
		//assert (A.GetPointer()!=R.GetPointer());
		//memcpy(&R[0][0],&A[0][0],sizeof(float)*N*N);
		if (a.GetPointer()!=r.GetPointer()) {
			memcpy(&r(0,0),&a(0,0),sizeof(float)*N*N);
		}
		integer n = N;
		integer lda = N;
		integer ipiv[N];
		float work[N];
		integer lwork = N;
		integer info;
		dgetrf_(&n,&n,&r(0,0),&lda,ipiv,&info);	
		dgetri_(&n, &r(0,0), &lda, ipiv, work, &lwork, &info);	
	//#endif
}

//template <int M, int N>
//inline void InverseCov(CMatrix<M,N> a, CMatrix<M,N> &r) {
//	assert(M == N);
//	//assert (A.GetPointer()!=R.GetPointer());
//	//memcpy(&R[0][0],&A[0][0],sizeof(float)*N*N);
//	assert(a.GetPointer()!=r.GetPointer());
//
//	SetIdentity(r);
//	// a is transferred by value, because dposv overwrite
//	// its Cholesky factorization on it.
//
//	char	uplo='L';
//	integer n = N;
//	integer nrhs = N;
//	integer lda = N;
//	integer ldb = N;
//	integer info;
//	dposv_(&uplo,&n,&nrhs,&a(0,0),&lda,&r(0,0),&ldb,&info);	
//}


//// Cholesky Factorization
//// if uplo='L'	a = r*r'
//// if uplo='U'	a = r'*r
//template <int M, int N>
//inline void Cholesky(const CMatrix<M,N> &a, CMatrix<M,N> &r, char uplo = 'L') {
//	assert(M == N);
//	//assert (A.GetPointer()!=R.GetPointer());
//	//memcpy(&R[0][0],&A[0][0],sizeof(float)*N*N);
//	if (a.GetPointer()!=r.GetPointer()) {
//		memcpy(&r(0,0),&a(0,0),sizeof(float)*N*N);
//	}
////	char uplo = 'L';
//	integer n = N;
//	integer lda = N;
//	integer info;
//	dpotrf_(&uplo,&n,&r(0,0),&lda,&info);
//
//	// To avoid problems in the posteriour use of the result,
//	// the triangular zone of the matrix which do no mantain
//	// the Cholesky factorization is set to zero.
//	int i;
//	int j;
//	if(uplo=='L')
//	{
//		for(i=0;i<M;i++)
//		{
//			for(j=i+1;j<N;j++)
//			{
//				r(i,j) = 0.0;
//			}
//		}
//	}else{
//		for(i=0;i<M;i++)
//		{
//			for(j=i+1;j<N;j++)
//			{
//				r(j,i) = 0.0;
//			}
//		}	
//	}
//}

////template <int M, int N>								// Cannot use original implementation 
////inline float Determinant(const CMatrix<M,N> &q)		// by Dani because "dpodi_" gives a 
////{													// linking error.
////	assert(M == N);									// I've had to made it by hand below.
////
////	CMatrix<M,N>	b0;
////
////	Cholesky(q,b0,'U');
////
////	integer lda = M;
////	integer n = M;
////	integer job = 10;
////	float	det[2];
////	float	determinant;
////
////	dpodi_(&b0(0,0),&lda,&n,&det[0],&job);
////
////	determinant = fabs(det[0]* pow(10.0,det[1]));
////
////	return determinant;
////}


template <int M, int N>								// Fast Implementation by hand of the 
inline float Determinant(const CMatrix<M,N> &q)		// cases of 2x2 and 3x3 matrices
{
	assert(M == N);
	assert( (M==2) || (M==3) );
	
	if (M==2) // 2x2
	{
		return q(0,0)*q(1,1) - q(0,1)*q(1,0);
	}
	else if (M==3) // 3x3
	{
		return q(0,0)*q(1,1)*q(2,2) + q(0,1)*q(1,2)*q(2,0) + q(0,2)*q(1,0)*q(2,1) - q(2,0)*q(1,1)*q(0,2) - q(2,1)*q(1,2)*q(0,0) - q(2,2)*q(1,0)*q(0,1);
	}
}




template <int M, int N> 
inline void Save(CMatrix<M,N> &a, const char* pFileName) 
{
	FILE*	pFile;

	pFile = fopen(pFileName,"wb");
	fwrite(a.GetPointer(),sizeof(float),a.m_m*a.m_n,pFile);
	fclose(pFile);
}

template <int M, int N> 
inline void Load(CMatrix<M,N> &a, const char* pFileName) 
{
	FILE*	pFile = 0;
	int	i;
	double	num;
	float	*pData;

    pFile = fopen(pFileName,"rb");
	pData = ((float*) a.GetPointer());

	for(i=0;i<(N*M);i++)
	{
		// read element to element, in order to cast 
		// loaded data properly.
		fread(&num,sizeof(double),1,pFile);
		*pData = num;
		pData++;
	}
	fclose(pFile);
}

// Initializes the matrix randomly, with values N(0,1)
template <int M, int N> 
inline void RndN(CMatrix<M,N> &r) 
{
	int	i;
	float* pR;

	pR = &r(0,0);
	for(i=0;i<(M*N);i++)
	{
        *pR++ = rand();
	}
}

//inline void Eig(double** H, int n, double* Er, double* Ei, double** Evecs)
//{
//	dgeev(H, n, Er, Ei, Evecs);
//}



#ifdef _FLOAT
#undef dgetrf_ 
#undef dgetri_ 
#undef dposv_	
#undef dpotrf_	
#undef dpodi_
#undef dgemm_
#endif


#endif // MATRIX_H

//#undef assert
