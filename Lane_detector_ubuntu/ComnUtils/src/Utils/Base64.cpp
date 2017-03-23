#ifndef __unix__
#include "Base64.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CBase64::CBase64()
{

}

CBase64::~CBase64()
{

}

/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void CBase64::EncodeBlock( unsigned char in[3], unsigned char out[4], int len )
{
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

/*
** encode
**
** base64 encode a stream adding padding and line breaks as per spec.
*/
void CBase64::Encode( unsigned char* input, int inputLength, CString& base64 )
{
    unsigned char in[3], out[5];
    int i, len;//, blocksout = 0;
	int	bytesProcessed = 0;
	unsigned char*	pInput = input;

	out[4] = 0;	// Per permetre agregar 'out' a 'base64' simplement amb sumes
	base64 = "";

    while( bytesProcessed<inputLength ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
			if(bytesProcessed<inputLength)
			{
				in[i] = (unsigned char) *pInput;
				pInput++;
				bytesProcessed++;
				len++;
			}
			else{
                in[i] = 0;
            }
        }
        if( len ) {
            EncodeBlock( in, out, len );
			//base64 += out;
			base64 = base64 + out;
        }
    }
}


/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
void CBase64::DecodeBlock( unsigned char in[4], unsigned char out[3] )
{   
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}


/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
void CBase64::Decode( unsigned char* input, int inputLength, unsigned char* output )
{
    unsigned char in[4], out[3], v;
    int i, len;
	int	bytesProcessed = 0;
	unsigned char*	pInput = input;
	unsigned char*	pOutput = output;

    while( bytesProcessed < inputLength ) {
        for( len = 0, i = 0; (i < 4) && ( bytesProcessed < inputLength ); i++ ) {
            v = 0;
            while( ( bytesProcessed < inputLength ) && v == 0 ) {
                v = (unsigned char) *pInput++;
				bytesProcessed++;
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( bytesProcessed <= inputLength ) {
                if( v ) {
                    in[ i ] = (unsigned char) (v - 1);
					len++;
                }
            }
            else {
                in[i] = 0;
            }
        }
        if( len ) {
            DecodeBlock( in, out );
            for( i = 0; i < len - 1; i++ ) {
                *pOutput++ = out[i];
            }
        }
    }
}



CString	CBase64::DoubleToBase64(double val)
{
	int	i;
	int j;
	unsigned char	in[3];
	unsigned char	out[5];	// darrer caracter a zero, per poder sumar 'out' amb 'base64'
	unsigned char*	p;
	CString	base64;

	out[4] = 0;

	p = (unsigned char*)(&val);

	for(i=0;i<sizeof(double)/3; i++)
	{
		for(j=0;j<3;j++)
		{
			in[j] = *p;
			p++;
		}

		EncodeBlock(in,out,3);
		//base64 += out;
		base64 = base64 + out;
	}

	if(sizeof(double)%3!=0)
	{
		for(j=0;j<sizeof(double)%3;j++)
		{
			in[j] = *p;
			p++;
		}

		for(j=sizeof(double)%3;j<3;j++)
		{
			in[j] = 0;
		}

		EncodeBlock(in,out,sizeof(double)%3);
		//base64 += out;
		base64 = base64 + out;
	}
	
	return base64;
}

double  CBase64::Base64ToDouble(const CString& text)
{
	int	i;
	int	j;
	double	val;
	unsigned char*	p = (unsigned char*) &val;
	unsigned char v;

	unsigned char	in[4];
	unsigned char	out[3];
	int		len = 0;

	for(i=0; i<text.GetLength()/4;i++)
	{
		for(j=0;j<4;j++)
		{
			if((i*4+j)<text.GetLength())
			{
				v = (unsigned char) (text[i*4+j]);

				v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61-1);
                }

				in[j] = v;
			}else{
				in[j] = 0;
			}
		}
		DecodeBlock(in,out);

		for(j=0;j<3;j++)
		{
			if(len<sizeof(double))
			{
				*p = out[j];
				p++;
				len++;
			}
		}
	}

	return val;
}
#endif //__unix__
