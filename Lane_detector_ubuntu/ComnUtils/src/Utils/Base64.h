///////////////////////////////////////////////////////////////////////////////
//   Base64.h: interface for the Base64 class.
// 
//   Author      : From Dani Ponsa, re-using code by Bob Trower (MIT) at http://base64.sourceforge.net/b64.c
//   Modified by : David Vazquez (David.Vazquez@cvc.uab.es)
//   Modified    : 2010/06/08
//
//   Description : It is the Base64 class.
//
//	 TODO:Change CString by string
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __unix__
#ifndef _BASE_64_
#define _BASE_64_

#if _MSC_VER > 1000
	#pragma once
#endif

#include <atlstr.h>

/*
** Translation Table as described in RFC1113
*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** Translation Table to decode (created by author)
*/
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

class CBase64  
{
public:
	CBase64();
	virtual ~CBase64();

	CString	DoubleToBase64(double val);
	double  Base64ToDouble(const CString& text);

	void EncodeBlock( unsigned char in[3], unsigned char out[4], int len );
	void DecodeBlock( unsigned char in[4], unsigned char out[3] );

	void Encode( unsigned char* input, int inputLength, CString& base64 );
	void Decode( unsigned char* input, int inputLength, unsigned char* output );

};

#endif //_BASE_64_
#endif //__unix__
