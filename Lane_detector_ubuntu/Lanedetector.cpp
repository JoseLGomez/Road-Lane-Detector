#include "Lanedetector.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Constructor.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 *****************************************************************************/

CLaneDetector::CLaneDetector()
{
}

/**************************************************************************//**
 * @brief	Destructor.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
CLaneDetector::~CLaneDetector()
{
}

///////////////////////////////////////////////////////////////////////////////
// Initialization/Finalization
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief	Initializes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
void CLaneDetector::Initialize()
{
}

/**************************************************************************//**
 * @brief	Finishes this object.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/
void CLaneDetector::Finish()
{
}

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

/**************************************************************************//**
 * @brief	Detect the type of the line: continuous or discontinuous
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *****************************************************************************/

void CLaneDetector::LaneTypeDetection()
{
}

/**************************************************************************//**
 * @brief	Computes.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param	Image		  	Mat format image.
 *****************************************************************************/
void CLaneDetector::Compute()
{
}

/**************************************************************************//**
 * @brief	Show an image making before a char convertion.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param	src		  	Mat format image.
 * @param   name        Name for the imshow
 *****************************************************************************/

void CLaneDetector::PrintImage(Mat src, String name)
{
    src = CV8Uconvertor(src);
    imshow(name, src);
}

/**************************************************************************//**
 * @brief	Convert Mat image to CV8U (unsigned 8 bits).
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param	input		  	Mat format image.
 *****************************************************************************/

Mat CLaneDetector::CV8Uconvertor(Mat input)
/* Convert Mat input type to unsigned char [0-255] */
{
    double minVal, maxVal;
    minMaxLoc(input, &minVal, &maxVal); //find minimum and maximum intensities
    input.convertTo(input, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    return input;
}

/**************************************************************************//**
 * @brief	Print Mat values on screen.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param	src		  	Mat format image.
 * @param   name        Name for the print
 *****************************************************************************/

void CLaneDetector::PrintMat(Mat src, String nombre)
{
    cout << nombre << " = " << endl << " "  << src << endl << endl;
}

/**************************************************************************//**
 * @brief	This function plots on the original image a vector of lines in Polar coordinates
 *          given by hough
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param   lines       Vector of float vector that represents a lines
 *****************************************************************************/

void CLaneDetector::PrintLinesOnimage(vector<Vec3f> lines, Scalar colour)
{
    Point linept1;
    Point linept2;
    float rho, theta;
    float a,b;
    for (unsigned int i=0; i < lines.size(); i++)
    {
        linept1.y = lineHeight;
        linept2.y = rows;
        rho = lines[i][0], theta = lines[i][1];
        a = cos(theta);
        b = sin(theta);
        linept1.x = cvRound(-(b*linept1.y/a)+(rho/a));
        linept2.x = cvRound(-(b*linept2.y/a)+(rho/a));
        //cvtColor(image, image, CV_GRAY2BGR);
        linept1.y = lineHeight + image.rows/factor - rows;
        linept2.y = image.rows/factor;
        line( image, linept1*factor, linept2*factor, colour, 2, CV_AA);
    }
}

void CLaneDetector::PrintLinesOnimage(Vec3f lineInput, Scalar colour)
{
    Point linept1;
    Point linept2;
    float rho, theta;
    float a,b;
    linept1.y = lineHeight;
    linept2.y = rows;
    rho = lineInput[0], theta = lineInput[1];
    a = cos(theta);
    b = sin(theta);
    linept1.x = cvRound(-(b*linept1.y/a)+(rho/a));
    linept2.x = cvRound(-(b*linept2.y/a)+(rho/a));
    //cvtColor(image, image, CV_GRAY2BGR);
    linept1.y = lineHeight + image.rows/factor - rows;
    linept2.y = image.rows/factor;
    line( image, linept1*factor, linept2*factor, colour, 2, CV_AA);
}

/**************************************************************************//**
 * @brief	This function convert a Point in polar coordinates to Cartesian
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param   input       Vector that represent the polar coordinates
 *****************************************************************************/

vector<Point> CLaneDetector::PolarToCartesian(Vec3f input)
{
    vector<Point> output;
    Point linept1;
    Point linept2;
    float rho, theta;
    float a,b;
    linept1.y = lineHeight;
    linept2.y = rows;
    rho = input[0], theta = input[1];
    a = cos(theta);
    b = sin(theta);
    linept1.x = cvRound(-(b*linept1.y/a)+(rho/a));
    linept2.x = cvRound(-(b*linept2.y/a)+(rho/a));
    //linept1.y = lineHeight + image.rows/FACTOR - rows;
    //linept2.y = image.rows/FACTOR;
    output.push_back(linept1);
    output.push_back(linept2);
    return output;
}

int CLaneDetector::PolarToCartesian(Vec3f input, int y)
{
    int output;
    float rho, theta;
    float a,b;
    rho = input[0], theta = input[1];
    a = cos(theta);
    b = sin(theta);
    output = cvRound(-(b*y/a)+(rho/a));
    return output;
}

int CLaneDetector::PolarToCartesian(vector<float> input, int y)
{
    int output;
    float rho, theta;
    float a,b;
    rho = input[0], theta = input[1];
    a = cos(theta);
    b = sin(theta);
    output = cvRound(-(b*y/a)+(rho/a));
    return output;
}

/**************************************************************************//**
 * @brief	This function convert a Mat input in a vector of unsigned char
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param   image       Mat input
 *****************************************************************************/

vector<unsigned char> CLaneDetector::ConvertMatToVector(Mat image)
{
    vector<unsigned char> vectorizedImage;
    if (image.isContinuous()) {
      vectorizedImage.assign(image.datastart, image.dataend);
    } else {
      for (int i = 0; i < image.rows; ++i) {
        vectorizedImage.insert(vectorizedImage.end(), image.ptr<unsigned char>(i), image.ptr<unsigned char>(i)+image.cols);
      }
    }
    return vectorizedImage;
}

/**************************************************************************//**
 * @brief	This function converts a vector input to a Mat
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param   image       Vector of unsigned char
 *****************************************************************************/

Mat CLaneDetector::ConvertVectorToMat(float * image, int rows, int cols)
{
    Mat imageProcessed(rows,cols,CV_32F);
    int aux = 0;
    for (int x=0; x < rows; x++) {
        for (int y =0; y < cols;y++) {
            imageProcessed.at<float>(x,y) = image[aux];
            aux++;
        }
    }
    return imageProcessed;
}

/**************************************************************************//**
 * @brief	Auxiliar Initialize function for homography.
 *
 * @author	Jose Luis Gomez
 * @date	Apr 4, 2016
 *
 * @param	src		  	Mat format image.
 * @param   name        Name for the print
 *****************************************************************************/

void CLaneDetector::InitializeHomography()
{
    // The 4-points at the input image
    vector<Point2f> origPoints;
    origPoints.push_back( Point2f(0, rows) );
    origPoints.push_back( Point2f(cols, rows) );
    origPoints.push_back( Point2f(826/factor, rows-(216/factor)) );
    origPoints.push_back( Point2f(588/factor, rows-(216/factor)) );

    // The 4-points correspondences in the destination image
    vector<Point2f> dstPoints;
    dstPoints.push_back( Point2f(0, rows) );
    dstPoints.push_back( Point2f(cols, rows) );
    dstPoints.push_back( Point2f(cols, 0) );
    dstPoints.push_back( Point2f(0, 0) );

    // Homography object
    homography = Homography( Size(cols, rows), Size(cols, rows), origPoints, dstPoints );
}

/**************************************************************************//**
 * @brief	Calculate a mean of widths of the road from a good detection of both lines
 *
 * @author	Jose Luis Gomez
 * @date	May 3, 2016
 *****************************************************************************/

void CLaneDetector::CalculateRoadWidthMean()
{
    int rightpoint;
    int leftpoint;
    float distance;

    rightpoint = PolarToCartesian(rightCandidateSelected,rows);
    leftpoint = PolarToCartesian(leftCandidateSelected,rows);
    distance = rightpoint - leftpoint;
    roadWidthMeasurements++;
    meanRoadWidth = (meanRoadWidth*(roadWidthMeasurements-1) + distance)/roadWidthMeasurements;
    if (roadWidthMeasurements > 4)
        roadWidthMeanActive = true;
}

/**************************************************************************//**
 * @brief	Calculate a mean of vanishing point obtained from a good detection
 *          of both lines
 *
 * @author	Jose Luis Gomez
 * @date	May 3, 2016
 *****************************************************************************/

void CLaneDetector::CalculateVanishingPointMean()
{
    int meanXtolerance = 30;
    Mat vanishingPoint = CalculateVanishingPoint(rightCandidateSelected,leftCandidateSelected);
    if (abs(vanishingPoint.at<float>(0) - cols/2) < meanXtolerance)
    {
        vanishingPointMeasurements++;
        vanishingPointmean = (vanishingPointmean*(vanishingPointMeasurements-1) + vanishingPoint)/vanishingPointMeasurements;
        if (vanishingPointMeasurements > 4)
            vanishingPointmeanActive = true;
    }
}

/**************************************************************************//**
 * @brief	Calculate Vanishing point of 2 lines
 *
 * @author	Jose Luis Gomez
 * @date	May 6, 2016
 *****************************************************************************/

Mat CLaneDetector::CalculateVanishingPoint(Vec3f right, Vec3f left)
{
    vector<Point> leftLine;
    vector<Point> rightLine;
    rightLine = PolarToCartesian(right);
    leftLine = PolarToCartesian(left);

    //Mat line1A(Scalar(,leftLine[0].y,1));
    Mat line1A = (Mat_<float>(1,3) << leftLine[0].x, leftLine[0].y, 1);
    Mat line1B = (Mat_<float>(1,3) << leftLine[1].x, leftLine[1].y, 1);
    Mat v1 = line1B.cross(line1A);
    v1 = v1/v1.at<float>(2);
    Mat line2A = (Mat_<float>(1,3) << rightLine[0].x, rightLine[0].y, 1);
    Mat line2B = (Mat_<float>(1,3) << rightLine[1].x, rightLine[1].y, 1);
    Mat v2 = line2B.cross(line2A);
    v2 = v2/v2.at<float>(2);
    Mat vanishingPoint = v2.cross(v1);
    vanishingPoint = vanishingPoint/vanishingPoint.at<float>(2);
    return vanishingPoint;
}

/**************************************************************************//**
 * @brief	Compare if a line fit with the mean vanishing point
 *
 * @author	Jose Luis Gomez
 * @date	May 6, 2016
 *****************************************************************************/

bool CLaneDetector::VanishingPointCompararison(Vec3f line)
{
    int tolerance = 50;
    int coorX;
    if (vanishingPointmeanActive)
    {
        coorX = PolarToCartesian(line, vanishingPointmean.at<float>(1));
        if (abs(coorX - vanishingPointmean.at<float>(0) < tolerance))
        {
            return true;
        } else {
            return false;
        }
    } else {
        for (size_t i = 0; i < vanishingPointsLearned.size(); i++ )
        {
            coorX = PolarToCartesian(line, vanishingPointsLearned[i].at<float>(1));
            if (abs(coorX - vanishingPointsLearned[i].at<float>(0) < tolerance))
            {
                return true;
            }
        }
        return false;
    }
}

/**************************************************************************//**
 * @brief	Compare if the vanishing point and width of two lines, fit with the
 *          mean vanishing point and road width.
 *
 * @author	Jose Luis Gomez
 * @date	May 6, 2016
 *****************************************************************************/

bool CLaneDetector::LaneCompararison(Vec3f left, Vec3f right)
{
    int vanTolerance = 50;
    int widthTolerance = 30;
    if (CalculateFitWithVanishingPoint(left,right) < vanTolerance
            && CalculateFitWithRoadWidth(left, right) < widthTolerance)
    {
        return true;
    } else {
        return false;
    }
}

/**************************************************************************//**
 * @brief	Compare if the vanishing point and width of the two lines detected
 *          and two, fit with the mean vanishing point and road width. Also
 *          choose the best candidate to this means.
 *
 * @author	Jose Luis Gomez
 * @date	May 6, 2016
 *****************************************************************************/

int CLaneDetector::DoubleLaneCompararison(Vec3f leftHough, Vec3f leftKalman, Vec3f rightHough, Vec3f rightKalman)
{
    int vanTolerance = 50;
    int widthTolerance = 30;
    int widthDiff = 0;
    int vanPointDiff = 0;
    float bestWidthDiff = widthTolerance;
    float bestVanPointDiff = vanTolerance;
    int selectionCase = 33;

    vanPointDiff = CalculateFitWithVanishingPoint(leftHough,rightHough);
    widthDiff = CalculateFitWithRoadWidth(leftHough,rightHough);
    if (vanPointDiff < bestVanPointDiff && widthDiff < bestWidthDiff)
    {
        selectionCase = 11;
        bestVanPointDiff = vanPointDiff;
        bestWidthDiff = widthDiff;
    }
    if (leftKalman != Vec3f(0,0,0))
    {
        widthDiff = CalculateFitWithVanishingPoint(leftKalman,rightHough);
        vanPointDiff = CalculateFitWithRoadWidth(leftKalman,rightHough);
        if (vanPointDiff + vanTolerance/2 < bestVanPointDiff
                && widthDiff + widthTolerance/2 < bestWidthDiff)
        {
            selectionCase = 21;
            bestVanPointDiff = vanPointDiff;
            bestWidthDiff = widthDiff;
        }
        if (rightKalman != Vec3f(0,0,0))
        {
            widthDiff = CalculateFitWithVanishingPoint(leftKalman,rightKalman);
            vanPointDiff = CalculateFitWithRoadWidth(leftKalman,rightKalman);
            if (vanPointDiff + vanTolerance/2 < bestVanPointDiff
                    && widthDiff + widthTolerance/2 < bestWidthDiff)
            {
                selectionCase = 22;
            }
        }
    } else {
        if (rightKalman != Vec3f(0,0,0))
        {
            widthDiff = CalculateFitWithVanishingPoint(leftHough,rightKalman);
            vanPointDiff = CalculateFitWithRoadWidth(leftHough,rightKalman);
            if (vanPointDiff + vanTolerance/2 < bestVanPointDiff
                    && widthDiff + widthTolerance/2 < bestWidthDiff)
            {
                selectionCase = 12;
            }
        }
    }
    return selectionCase;
}

/**************************************************************************//**
 * @brief	Calculate if the width of two lines on the bottom of the image fit
 *          with the road mean.
 *
 * @author	Jose Luis Gomez
 * @date	May 6, 2016
 *****************************************************************************/

int CLaneDetector::CalculateFitWithRoadWidth(Vec3f left, Vec3f right)
{
    int leftX = PolarToCartesian(left, rows);
    int rightX = PolarToCartesian(right, rows);
    if (roadWidthMeanActive)
        return abs(rightX - leftX - meanRoadWidth);
    else {
        int aux = rightX - leftX - roadLineSizesLearned[0];
        for (size_t i = 1; i < roadLineSizesLearned.size(); i++ )
        {
            if (abs(rightX - leftX - roadLineSizesLearned[i]) < aux)
            {
                aux = abs(rightX - leftX - roadLineSizesLearned[i]);
            }
        }
        return aux;
    }
}

/**************************************************************************//**
 * @brief	Calculate if the vanishing point of two lines fit with the vanishing
 *          point mean
 *
 * @author	Jose Luis Gomez
 * @date	May 6, 2016
 *****************************************************************************/

float CLaneDetector::CalculateFitWithVanishingPoint(Vec3f left, Vec3f right)
{
    float x;
    float y;
    Mat vanishingPoint = CalculateVanishingPoint(left,right);
    if (vanishingPointmeanActive)
    {
        x = vanishingPoint.at<float>(0) - vanishingPointmean.at<float>(0);
        y = vanishingPoint.at<float>(1) - vanishingPointmean.at<float>(1);
        return sqrt(x*x + y*y);
    } else {
        x = vanishingPoint.at<float>(0) - vanishingPointsLearned[0].at<float>(0);
        y = vanishingPoint.at<float>(1) - vanishingPointsLearned[0].at<float>(1);
        float aux = sqrt(x*x + y*y);
        for (size_t i = 1; i < vanishingPointsLearned.size(); i++ )
        {
            x = vanishingPoint.at<float>(0) - vanishingPointsLearned[i].at<float>(0);
            y = vanishingPoint.at<float>(1) - vanishingPointsLearned[i].at<float>(1);
            if (sqrt(x*x + y*y) < aux)
            {
                aux = sqrt(x*x + y*y);
            }
        }
        return aux;
    }
}

float CLaneDetector::CalculateFitWithVanishingPoint(Vec3f line)
{
    int lineX;
    if (vanishingPointmeanActive)
    {
        lineX = PolarToCartesian(line, vanishingPointmean.at<float>(1));
        lineX = abs(lineX - vanishingPointmean.at<float>(0));
        return lineX;
    } else {
        int aux;
        lineX = PolarToCartesian(line, vanishingPointsLearned[0].at<float>(1));
        lineX = abs(lineX - vanishingPointsLearned[0].at<float>(0));
        aux = lineX;
        for (size_t i = 1; i < vanishingPointsLearned.size(); i++ )
        {
            lineX = PolarToCartesian(line, vanishingPointsLearned[i].at<float>(1));
            lineX = abs(lineX - vanishingPointsLearned[i].at<float>(0));
            if (lineX < aux)
            {
                aux = lineX;
            }
        }
        return aux;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Gets/Sets
///////////////////////////////////////////////////////////////////////////////

//get method for visualize intermediate operations
Mat CLaneDetector::getThresholdResult()
{
    return thresholdResult;
}
Mat CLaneDetector::getHoughlineResult()
{
    return houghlineResult;
}
Mat CLaneDetector::getImage()
{
    return image;
}
vector<vector<Point>> CLaneDetector::getLanesDetected()
{
    return lanesDetected;
}
