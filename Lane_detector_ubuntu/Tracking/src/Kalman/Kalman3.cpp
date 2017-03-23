#include "Kalman3.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Default constructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CKalman3::CKalman3()
{
	Initialize(0, 0, 0, 0);
}


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	a_in					The in.
 * @param	b_in					The in.
 * @param	c_in					The in.
 * @param	maxDistanceThreshold	The maximum distance threshold.
 *****************************************************************************/
CKalman3::CKalman3(float a_in, float b_in, float c_in, float maxDistanceThreshold)
{
	Initialize(a_in, b_in, c_in, maxDistanceThreshold);
}


/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
CKalman3::~CKalman3()
{
	Finish();
}


///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Initializes this object.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	a_in					The in.
 * @param	b_in					The in.
 * @param	c_in					The in.
 * @param	maxDistanceThreshold	The maximum distance threshold.
 *****************************************************************************/
void CKalman3::Initialize(float a_in, float b_in, float c_in, float maxDistanceThreshold)
{
	/*
	m_maxDistanceThreshold = maxDistanceThreshold;

	SetIdentity(I);

	a = a_in;
	b = b_in;
	c = c_in;

	X(0,0)=a;
	X(1,0)=b;
	X(2,0)=c;
	X(3,0)=0;
	X(4,0)=0;
	X(5,0)=0;

	// We don't know what the error is at the beginning, so get the first measures
	P(0,0)=1; P(0,1)=0; P(0,2)=0; P(0,3)=0; P(0,4)=0; P(0,5)=0;
	P(1,0)=0; P(1,1)=1; P(1,2)=0; P(1,3)=0; P(1,4)=0; P(1,5)=0;
	P(2,0)=0; P(2,1)=0; P(2,2)=1; P(2,3)=0; P(2,4)=0; P(2,5)=0;
	P(3,0)=0; P(3,1)=0; P(3,2)=0; P(3,3)=1; P(3,4)=0; P(3,5)=0;
	P(4,0)=0; P(4,1)=0; P(4,2)=0; P(4,3)=0; P(4,4)=1; P(4,5)=0;
	P(5,0)=0; P(5,1)=0; P(5,2)=0; P(5,3)=0; P(5,4)=0; P(5,5)=1;

	// State transition
	A(0,0)=1; A(0,1)=0; A(0,2)=0; A(0,3)=0; A(0,4)=0; A(0,5)=0;
	A(1,0)=0; A(1,1)=1; A(1,2)=0; A(1,3)=0; A(1,4)=0; A(1,5)=0;
	A(2,0)=0; A(2,1)=0; A(2,2)=1; A(2,3)=0; A(2,4)=0; A(2,5)=0;
	A(3,0)=0; A(3,1)=0; A(3,2)=0; A(3,3)=1; A(3,4)=0; A(3,5)=0;
	A(4,0)=0; A(4,1)=0; A(4,2)=0; A(4,3)=0; A(4,4)=1; A(4,5)=0;
	A(5,0)=0; A(5,1)=0; A(5,2)=0; A(5,3)=0; A(5,4)=0; A(5,5)=1;

	// State to measurement
	H(0,0)=1; H(0,1)=0; H(0,2)=0; H(0,3)=0; H(0,4)=0; H(0,5)=0;
	H(1,0)=0; H(1,1)=1; H(1,2)=0; H(1,3)=0; H(1,4)=0; H(1,5)=0;
	H(2,0)=0; H(2,1)=0; H(2,2)=1; H(2,3)=0; H(2,4)=0; H(2,5)=0;

	// Process noise covariance
	Q(0,0)=1; Q(0,1)=0; Q(0,2)=0; Q(0,3)=-1; Q(0,4)= 0; Q(0,5)= 0;
	Q(1,0)=0; Q(1,1)=1; Q(1,2)=0; Q(1,3)= 0; Q(1,4)=-1; Q(1,5)= 0;
	Q(2,0)=0; Q(2,1)=0; Q(2,2)=1; Q(2,3)= 0; Q(2,4)= 0; Q(2,5)=-1;
	Q(3,0)=1; Q(3,1)=0; Q(3,2)=0; Q(3,3)= 0; Q(3,4)= 0; Q(3,5)= 0;
	Q(4,0)=0; Q(4,1)=1; Q(4,2)=0; Q(4,3)= 0; Q(4,4)= 0; Q(4,5)= 0;
	Q(5,0)=0; Q(5,1)=0; Q(5,2)=1; Q(5,3)= 0; Q(5,4)= 0; Q(5,5)= 0;

	// Measurement noise covariance
	R(0,0)=1; R(0,1)=0; R(0,2)=0;
	R(1,0)=0; R(1,1)=1; R(1,2)=0;
	R(2,0)=0; R(2,1)=0; R(2,2)=1;
	*/
}


/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::Finish()
{
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Public
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Predicts this object.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::Predict()
{
	/*
	ProjectState();
	ProjectErrorCovariance();
	*/
}


/**************************************************************************//**
 * @brief	Corrects.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @param	a_in	The in.
 * @param	b_in	The in.
 * @param	c_in	The in.
 *****************************************************************************/
void CKalman3::Correct(float a_in, float b_in, float c_in)
{
	/*
	Z(0,0) = a_in;
	Z(1,0) = b_in;
	Z(2,0) = c_in;

	KalmanGain();
	UpdateEstimateFromMeasurement();
	UpdateErrorCovariance();

	a = X(0,0);
	b = X(1,0);
	c = X(2,0);
	*/
}



///////////////////////////////////////////////////////////////////////////////
// Functions: Kalman process
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Project state.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::ProjectState()
{
	/*
	Mult(A, X, tmp61_1);			// AX
	Copy(tmp61_1, X);
	*/
}


/**************************************************************************//**
 * @brief	Project error covariance.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::ProjectErrorCovariance()
{
	/*
	Mult(A,P,tmp66_1);			// AP
	MultTrans(tmp66_1,A,tmp66_2);// APA^t
	Add(tmp66_2,Q,P);				// APA^t + Q
	*/
}


/**************************************************************************//**
 * @brief	Kalman gain.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::KalmanGain()
{
	/*
	Mult(H,P,tmp36_1);				// HP
	MultTrans(tmp36_1,H,tmp33_1);	// HPH^t
	Add(tmp33_1, R, tmp33_2);		// HPH^t+R
	Inverse(tmp33_2,tmp33_1);		// (HPH^t+R)^-1

	Mult(tmp33_2,tmp33_1,tmp33_3);

	MultTrans(P,H,tmp63_1);		// PH^t
	Mult(tmp63_1,tmp33_1,K);		// PH^t * (HPH^t+R)^-1
	*/
}


/**************************************************************************//**
 * @brief	Updates the estimate from measurement.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::UpdateEstimateFromMeasurement()
{/*
	Mult(H,X,tmp31_1);				// HX
	Subtract(Z,tmp31_1,tmp31_2);	// Z-HX
	Mult(K,tmp31_2,tmp61_1);		// K(Z-HX)
	Add(X,tmp61_1,tmp61_2);		// X+K(Z-HX)
	Copy(tmp61_2,X);
	*/
}


/**************************************************************************//**
 * @brief	Updates the error covariance.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @date	Mar 26, 2012
 *****************************************************************************/
void CKalman3::UpdateErrorCovariance()
{
	/*
	Mult(K,H,tmp66_1);			// KH
	Subtract(I,tmp66_1,tmp66_2);// I-KH
	Mult(tmp66_2,P,tmp66_3);	// (I-KH)P
	Copy(tmp66_3,P);
	*/
}
