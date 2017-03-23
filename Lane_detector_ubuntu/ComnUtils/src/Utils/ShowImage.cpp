#include "ShowImage.h"
#include "StereoCamera.h"
#include "StereoImage.h"
#include <iostream>
#include <iomanip>
#include <Utils/Utils.h>
using namespace cv;

///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////
void CallBack(int event, int x, int y, int flags, void* userData)
{
    if (event == EVENT_MOUSEMOVE)
        ((CShowImage*)userData)->PutText(x,y);
}


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

void CShowImage::Initialize()
{	
	if (!m_isInit)
	{
        m_isInit = true;
        cv::startWindowThread();
        cv::namedWindow(m_windowName, WINDOW_NORMAL);
        setMouseCallback(m_windowName, CallBack, this);
	}
}


void CShowImage::Finish()
{
	if (m_isInit)
	{
		m_isInit = false;
        cv::destroyWindow(m_windowName);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

void CShowImage::ShowImage(const Mat& image, const cv::Mat& disparity, CStereoCamera* pStereoCamera)
{
	m_image	= image;
	m_pStereoCamera = pStereoCamera;
	m_disparity = disparity;
    cv::imshow(m_windowName, m_image);
	cv::waitKey(1);
}


void CShowImage::PutText(const int x, const int y)
{
	string str="Image("+std::to_string(x)+", "+std::to_string(y)+")";
	Point pos(m_image.cols/40,m_image.rows-m_image.rows/20);
	int font = FONT_HERSHEY_COMPLEX_SMALL;
	double scale = 0.5;
	int thickness = 1;
	switch(m_imageType)
	{
		case esit_depth:
			if (m_pStereoCamera)
			{
				if (m_image.channels()==1)
				{
					float depth = m_image.at<float>(y,x);
					float wx,wy,wz;
					float disp = m_pStereoCamera->DisparityFromDistance(depth);
					m_pStereoCamera->Image2WorldFromDisparity((float)x, (float)y, disp, wx, wy, wz);
					str = str+", Disparity("+std::to_string(disp)+"), World("+to_string_with_precision(wx)+", "+to_string_with_precision(wy) +", "+to_string_with_precision(wz)+")";
				}
				else if (m_image.channels()==3 && m_disparity.cols!=0)
				{
					if (m_pStereoCamera)
					{
						float wx,wy,wz;
							int disp = (int)m_disparity.at<uchar>(y,x);
							str = str+", Disparity("+std::to_string(disp)+")";
							if (disp != 0)
							{
								m_pStereoCamera->Image2WorldFromDisparity(x,y,disp,wx,wy,wz);
								str = str + ", World("+to_string_with_precision(wx)+", "+to_string_with_precision(wy) +", "+to_string_with_precision(wz)+")";
							}
					}
				}
			}
			break;
		case esit_disp:
			if (m_pStereoCamera)
			{
				if (m_disparity.cols!=0)
				{
					float wx,wy,wz;
					int disp = (int)m_disparity.at<uchar>(y,x);
					str = str+", Disparity("+std::to_string(disp)+")";
					if (disp != 0)
					{
						m_pStereoCamera->Image2WorldFromDisparity((float)x, (float)y, (float)disp, wx, wy, wz);
						str = str + ", World("+to_string_with_precision(wx)+", "+to_string_with_precision(wy) +", "+to_string_with_precision(wz)+")";
					}
				}
			}
			break;
		case esit_image:
			if (m_pStereoCamera && m_disparity.cols!=0)
			{
				float wx,wy,wz;
					int disp = (int)m_disparity.at<uchar>(y,x);
					str = str+", Disparity("+std::to_string(disp)+")";
					if (disp != 0)
					{
						m_pStereoCamera->Image2WorldFromDisparity((float)x, (float)y, (float)disp, wx, wy, wz);
						str = str + ", World("+to_string_with_precision(wx)+", "+to_string_with_precision(wy) +", "+to_string_with_precision(wz)+")";
					}
			}
			break;
		default:
			break;
		case esit_unknown:
			break;
	}

	if (m_imageType != esit_unknown && (m_image.rows>0))
	{
		int marginX = m_image.cols/100;
		int marginY = m_image.rows/40;
        rectangle(m_image, cv::Point(pos.x-marginX,pos.y-(2*marginY)), cv::Point(m_image.cols-pos.x+marginX,pos.y+marginY),Scalar( 0, 0, 0), -1);
		putText(m_image, str, pos, font, scale, Scalar(0,0,255),thickness);
		cv::imshow(m_windowName, m_image);
	}
}
