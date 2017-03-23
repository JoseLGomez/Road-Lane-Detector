#include "UniqueIdentifier.h"

CUniqueIdentifier* CUniqueIdentifier::pinstance = 0; // initialize pointer


/**************************************************************************//**
 * @brief	Gets the instance.
 *
 * @author	David Geronimo (dgeronimo@cvc.uab.es)
 * @author	David Vazquez (David.Vazquez@cvc.uab.es)
 * @date	Mar 26, 2012
 *
 * @return	null if it fails, else.
 *****************************************************************************/
CUniqueIdentifier* CUniqueIdentifier::Instance()
{
	if (pinstance == 0)
		pinstance = new CUniqueIdentifier;

    return pinstance;
}
