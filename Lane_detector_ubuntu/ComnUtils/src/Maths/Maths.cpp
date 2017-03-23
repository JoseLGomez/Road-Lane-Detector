#include "Maths.h"

MTRand* CUniqueMersenne::pMersenne = NULL; // initialize pointer

//(copy-paste from krystian)
void invSqrRoot(float &a, float &b, float &c, float &r_l, float &r_m, float &ea)
{
  //cout << "in a " << a << " b " << b << " c " << c << endl;
  float cos_2t = a - c;
  float sin_2t =  2*b;
  float r = sqrt(cos_2t*cos_2t + sin_2t*sin_2t);
  float u,v,l,m;
  if (r == 0) {
    // We have a = c and b = 0, i.e. the matrix is diagonal.
    u = 1;
    v = 0;
    l = a;
    m = c;
    ea=0;
    return;
  }else{ 
    cos_2t /= r;
    sin_2t /= r;
    //if(cos_2t<-1)cout << cos_2t<< endl;
    if(cos_2t<-1)cos_2t=-1;
    else if(cos_2t>1)cos_2t=1;
    
    // use half-angle formulae:
    float cos_t = sqrt((1 + cos_2t)/2);
    float sin_t = sqrt(1 - cos_t*cos_t);
    if (sin_2t < 0)
      sin_t = -sin_t;
    u = cos_t;  
    v = sin_t;
    
    l = a*u*u + 2*b*v*u + c*v*v;
    m = a*v*v - 2*b*u*v + c*u*u;
    ea=atan2(u,-v);
    //ea=(ea>0)?ea:ea+M_PI; 
    //cout << " l "<< l << " m "<< m << " r " << r<< " cos_t " << cos_t<< " sin_t "<< sin_t<< " atan " << 180*atan2(u,-v)/M_PI << endl;  
  }
 
  if ((l >= 0) && (m >= 0)) {
    r_l = 1/sqrt(l);
    r_m = 1/sqrt(m);
    float x=sqrt(r_l*r_m);
    r_l/=x;
    r_m/=x;
    if(r_l/r_m<0.1){
      r_l=3;
      r_m=0.33f;
    }
    a = r_l*u*u + r_m*v*v;
    b = r_l*u*v - r_m*u*v;
    c = r_l*v*v + r_m*u*u;
    //cout << "out a " << a << " b " << b << " c " << c << endl;
  }else{
//    cout << "out a " << a << " b " << b << " c " << c << " r " << r << " cos_2t " << cos_2t<< " sin_2t "<< sin_2t<<  endl;
//    cout << "errors "<<  " l "<< l << " m "<< m << " r " << r<< " cos_t " << u<< " sin_t "<< v<< " atan " << 180*atan2(u,-v)/M_PI <<endl;
  }  
}

void HistogramStats(const float* src, float* stats, const int N)
{
	/*
	stats[0] = mean
	stats[1] = mean dev
	stats[2] = std dev
	stats[3] = variance
	stats[4] = skewness
	stats[5] = kurtosis
	stats[6] = min
	stats[7] = max
	*/
	
	// Step 0: Initialize variables
	memset(stats, 0, 6*sizeof(float));
	stats[6] = FLT_MAX;
	stats[7] = -FLT_MAX;

	// Step 1: Compute mean, min and max
	for (int i=0; i<N; i++)
	{
		// Mean
		stats[0] += src[i];
		// Min
		stats[6] = (stats[6] < src[i]) ? stats[6] : src[i];
		// Max
		stats[7] = (stats[7] > src[i]) ? stats[7] : src[i];
	}
	stats[0] /= N;

	// Step 2: compute second, third and fourth moments from mean deviation
	for (int i=0; i<N; i++)
	{
		// Deviation
		float dev = src[i] - stats[0];
		float p;
		// Mean dev (|dev|)
		stats[1] += fabs(dev);
		// Variance (dev^2)
		stats[3] += (p = dev*dev);
		// Skewness (dev^3)
		stats[4] += (p *= dev);
		// Kurtosis (dev^4)
		stats[5] += (p *= dev);
	}

	// Mean Dev
	stats[1] /= N;

	// Variance (biased)
	float variance = stats[3]/N;

	// Variance (unbiased)
	stats[3] = stats[3]/(N-1);

	// Std dev (unbiased)
	stats[2] = sqrt(stats[3]);

	if (stats[3] > 0.0f)
	{
		// Skewness (biased)
		stats[4] /= N*variance*sqrt(variance);
		// Skewness (unbiased)
		stats[4] *= sqrt(float(N-1)/N) * N/float(N-2);

		// Kurtosis (biased)
		stats[5] = (stats[5])/(N*variance*variance);
		// Kurtosis (unbiased)
		stats[5] = ((N+1)*stats[5] - 3*(N-1)) * float(N-1)/((N-2)*(N-3)) + 3.0f;
	} 
} 