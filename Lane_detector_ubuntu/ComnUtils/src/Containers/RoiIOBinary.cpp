#include "RoiIOBinary.h"
#include "Containers/RoiIO.h"
#include "Containers/Roi.h"
#include "Utils/Utils.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Initializes/Finalizes
///////////////////////////////////////////////////////////////////////////////
void CRoiIOBinary::Initialize()
{
	m_video = Video();
}

void CRoiIOBinary::Finish()
{
	m_video = Video();
}

bool CRoiIOBinary::Write(const string& fileName)
{
	// Open the file
	FILE *pFile = fopen(fileName.c_str(),"wb");
	if (pFile == NULL)
	{
		VSysError(VSys_FileNotFound, fileName);
		return false;
	}

	// Write video data
	fwrite(&(m_video.GetVideoData()), sizeof(VideoData), 1, pFile);

	// Write frames data
	for (unsigned int i=0; i<m_video.GetNFrames(); i++)
	{
		// Write frame data
		fwrite(&(m_video.GetFrameData(i)), sizeof(FrameData) , 1, pFile);

		// Write objects from this frame
		for (unsigned int j=0; j<m_video.GetFrame(i).GetNObjets(); j++)
			fwrite(&m_video.m_vFrames[i].m_vObjects[j], sizeof(ObjectData),  1, pFile);
	}
	
	// Close file
	fclose(pFile);
	return true;
}

bool CRoiIOBinary::Read(const string& fileName)
{
	// Open file
	FILE* pFile = fopen(fileName.c_str(), "rb");
	if (pFile == NULL)
	{
		VSysError(VSys_FileNotFound, fileName);
		return false;
	}
	
	// Read video data
	fread(&(m_video.GetVideoData()), sizeof(VideoData), 1, pFile);

	// Alloc space for the frames
	m_video.m_vFrames.resize(m_video.m_videoData.m_nFrames, Frame());

	// Read frames data
	for (unsigned int i=0; i<m_video.GetNFrames(); i++)
	{
		// Read frame data
		fread(&(m_video.GetFrameData(i)), sizeof(FrameData), 1, pFile);
		////
		///
		// Alloc space for the objects
		m_video.m_vFrames[i].m_vObjects.resize(m_video.m_vFrames[i].m_frameData.m_nObject, ObjectData());	

		// Read objects from this frame
		for (unsigned int j=0; j<m_video.GetFrame(i).GetNObjets(); j++)
			fread(&m_video.m_vFrames[i].m_vObjects[j], sizeof(ObjectData), 1, pFile);
	}

	// Close file
	fclose(pFile);
	return true;
}

