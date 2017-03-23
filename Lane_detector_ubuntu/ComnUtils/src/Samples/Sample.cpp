#include "Sample.h"

#include "Image.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
CSample::CSample()
{
	m_pImage				= NULL;
	
	m_imageFullFileName		= "";
	m_imagePath				= "";
	m_imageFileName			= "";
	m_imageRawFilename		= "";
	m_imageExtension		= "";

	m_featuresFullFileName	= "";
	m_featuresPath			= "";
	m_featuresFileName		= "";
	m_featuresRawFilename	= "";
	m_featuresExtension		= "";

	m_id = -1;
}

CSample::CSample(const string& imageFileName, const string& featuresFileName, const int id)
{
	// Create the image
	m_pImage = new CImageFloat;

	// Initialize
	Initialize(imageFileName, featuresFileName, id);
}





///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////
void CSample::Initialize(const string& imageFileName, const string& featuresFileName, const int id)
{
	// Save Data
	SetImageFileName(imageFileName);
	SetFeaturesFileName(featuresFileName);
	m_id = id;
}

void CSample::Finish()
{
	if (m_pImage!=NULL)
	{
		m_pImage->Free();
		delete m_pImage;
		m_pImage = NULL;
	}
}



///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////
void CSample::SetImageFileName(const string& value)
{
	// Save data
	m_imageFullFileName = value;

	// Extract paths
	RetrievePath (m_imageFullFileName, m_imagePath, m_imageFileName);
	RetrieveExtension (m_imageFileName, m_imageRawFilename, m_imageExtension);
}

void CSample::SetFeaturesFileName(const string& value)
{
	// Save data
	m_featuresFullFileName = value;

	// Extract paths
	RetrievePath (m_featuresFullFileName, m_featuresPath, m_featuresFileName);
	RetrieveExtension (m_featuresFileName, m_featuresRawFilename, m_featuresExtension);
}
