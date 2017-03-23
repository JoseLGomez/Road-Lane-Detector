#include "ShowImage3D.h"
#if CV_MAJOR_VERSION>2
using namespace cv;

///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////
void CShowImage3D::Initialize()
{	
	if (!m_isInit)
	{
		m_isInit			= true;
		m_removeAllWidgets	= false;
		m_thread			= std::thread (&CShowImage3D::Thread, this);
		// Define camera pose
		Vec3d camPos(0.0f,0.0f,-5.0f), camFocalPoint(0.0f,0.0f,-4.995f), camYDir(0.0f,-1.0f,0.0f);
		m_camPose = viz::makeCameraPose(camPos, camFocalPoint, camYDir);
		m_cameraPoseChanged = true;

		cv::Matx44d proj;
		m_window.getCamera().computeProjectionMatrix(proj);
		proj(2,3) /= 100;
		m_window.setCamera(viz::Camera(proj, m_window.getCamera().getWindowSize()));

		m_window.getCamera().computeProjectionMatrix(proj);
		//for (int i=0; i<4; i++)
		//{
		//	for (int j=0; j<4; j++)
		//		std::cout << proj(i,j) << " ";
		//	std::cout << std::endl;
		//}
	}
}

void CShowImage3D::Finish()
{
	if (m_isInit)
	{
		m_mutex.lock();
		m_isInit = false;
		m_mutex.unlock();
		m_thread.join();
		m_window.close();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

void CShowImage3D::ShowWidget(const string& name, const viz::Widget& widget, const Affine3<double>& pose)
{
	m_mutex.lock();
	m_vNames.push_back(name);
	m_vWidgets.push_back(widget);
	m_vPoses.push_back(pose);
	m_mutex.unlock();
}

void CShowImage3D::RemoveAllWidgets()
{
	m_mutex.lock();
	m_removeAllWidgets = true;
	m_mutex.unlock();
}

void CShowImage3D::RemoveWidget(const string& name)
{
	m_mutex.lock();
	m_vNamesToRemove.push_back(name);
	m_mutex.unlock();
}

void CShowImage3D::SaveView(const string& fileName)
{
	m_window.saveScreenshot(fileName);
}

void CShowImage3D::Thread()
{
	while (m_isInit && !m_window.wasStopped())
	{
		if (!m_vNames.empty() || !m_vNamesToRemove.empty() || m_removeAllWidgets)
		{
			m_mutex.lock();

			// Set camera pose
			if (m_cameraPoseChanged)
			{
				m_window.setViewerPose(m_camPose);
				m_cameraPoseChanged = false;
				cv::Matx44d proj;
				m_window.getCamera().computeProjectionMatrix(proj);
			}

			// Remove widgets
			if (m_removeAllWidgets)
			{
				// Remove all widgets
				m_window.removeAllWidgets();
				m_removeAllWidgets = false;
			}
			else if (!m_vNamesToRemove.empty())
			{
				// Remove specific widgets
				for (unsigned int i=0; i<m_vNamesToRemove.size(); i++)
					m_window.removeWidget(m_vNamesToRemove[i]);
				m_vNamesToRemove.clear();
			}

			// Add new widgets if required
			for (unsigned int i=0; i<m_vWidgets.size(); i++)
				m_window.showWidget(m_vNames[i], m_vWidgets[i], m_vPoses[i]);
			m_vNames.clear();
			m_vWidgets.clear();
			m_vPoses.clear();

			m_mutex.unlock();
		}
		
		// Draw
		m_window.spinOnce(1, true);
	}
}

int	CShowImage3D::WasStoped()
{
	return m_window.wasStopped();
}
#endif