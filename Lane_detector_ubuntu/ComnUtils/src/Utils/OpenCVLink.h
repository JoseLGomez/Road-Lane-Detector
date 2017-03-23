#ifndef _OPENCV_LINK_
#define _OPENCV_LINK_

#if _MSC_VER > 1000
	#pragma once
#endif
#ifndef __unix__
	// OpenCV include
	#include "opencv.hpp"

	// OpenCV version sufix
	#ifdef _DEBUG
		#define CVVERSION  CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION) "d.lib"
	#else
		#define CVVERSION  CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION) ".lib"
	#endif

	// Link the needed modules
#pragma comment(lib, "opencv_calib3d" CVVERSION)
//#pragma comment(lib, "opencv_contrib" CVVERSION)
#pragma comment(lib, "opencv_core" CVVERSION)
#pragma comment(lib, "opencv_features2d" CVVERSION)
#pragma comment(lib, "opencv_flann" CVVERSION)
#pragma comment(lib, "opencv_highgui" CVVERSION)
#pragma comment(lib, "opencv_imgproc" CVVERSION)
//#pragma comment(lib, "opencv_legacy" CVVERSION)
#pragma comment(lib, "opencv_ml" CVVERSION)
#pragma comment(lib, "opencv_objdetect" CVVERSION)
#pragma comment(lib, "opencv_ts" CVVERSION)
#pragma comment(lib, "opencv_video" CVVERSION)
//#pragma comment(lib, "opencv_videoio" CVVERSION)
#pragma comment(lib, "opencv_videostab" CVVERSION)
//#pragma comment(lib, "opencv_imgcodecs" CVVERSION)
#pragma comment(lib, "opencv_photo" CVVERSION)
#pragma comment(lib, "opencv_shape" CVVERSION)
#pragma comment(lib, "opencv_stitching" CVVERSION)
#pragma comment(lib, "opencv_superres" CVVERSION)
#pragma comment(lib, "opencv_cuda" CVVERSION)
#pragma comment(lib, "opencv_cudastereo" CVVERSION)
#pragma comment(lib, "opencv_superres" CVVERSION)
#pragma comment(lib, "opencv_superres" CVVERSION)

#if CV_MAJOR_VERSION>2
	#pragma comment(lib, "opencv_viz" CVVERSION)
#endif
#endif
#endif
