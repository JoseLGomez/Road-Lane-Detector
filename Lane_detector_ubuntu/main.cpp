//#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <StraightLaneDetector.h>
#include <CurveLaneDetector.h>
#include <LaneDetectorManager.h>
#include <iostream>

/// Global Variables

using namespace cv;
using namespace std;

void testManager();
void testManagerWithFolder();
void testCurve();

int main()
{

    //testManager();
    testManagerWithFolder();
    //testCurve();
    waitKey(0);
    return 0;
}


void testManager()
{
    clock_t tStart = clock();
    Mat image;    
    bool kalman = true;
    bool lineDetector = true;
    string filename = "2016_05_13_11_11_26_188000.tif";
    string path = "test/";
    string parametersFile = "../Parameters/ParametersFileAutopista.ini";
    //string parametersFile = "/home/master/TFG_Lane_detector/Parameters/ParametersFileAutopista.ini";
    CLaneDetectorManager* laneDetector = new CLaneDetectorManager(parametersFile);
    CStraightLaneDetector* straightLaneDetector;
    image = imread(path + filename,CV_LOAD_IMAGE_GRAYSCALE);
    //laneDetector.PrintImage(image, "Perspective result");
    laneDetector->Initialize(image.rows, image.cols);
    laneDetector->Compute(image, lineDetector, kalman);
    straightLaneDetector = laneDetector->getStraightLaneDetector();
    //straightLaneDetector->LaneTypeDetection();
    //laneDetector.Compute(image);
    //laneDetector.HomographyHoughLinesDetection(result);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    Mat resultThreshold = straightLaneDetector->CV8Uconvertor(laneDetector->getThresholdResult());
    imwrite("../results/crestas/40_1.8" + filename, resultThreshold);
    straightLaneDetector->PrintImage(laneDetector->getImage(), "result");
    straightLaneDetector->PrintImage(laneDetector->getThresholdResult(), "ridge");
    //imwrite("results/" + filename, result );
}

void testManagerWithFolder()
{
    clock_t tStart = clock();
    clock_t t2Start;
    Mat image;
    int prevImageCols = 0;
    int prevImageRows = 0;
    bool kalman = true;
    bool lineDetector = true;
    Mat resultThreshold;
    Mat resultFinal;
    Mat concatResults;
    int numImagen = 0;
    double writingTime = 0;
    string path = "/media/master/Datos/UAB/Autopista2dia/";
    //string path = "cambiocarril/";
    vector<String> filenames;
    glob(path, filenames);
    string parametersFile = "../Parameters/ParametersFileAutopista.ini";
    //string parametersFile = "/home/master/TFG_Lane_detector/Parameters/ParametersFileAutopista.ini";
    CLaneDetectorManager* laneDetector = new CLaneDetectorManager(parametersFile);
    CStraightLaneDetector* straightLaneDetector;
    int framesPorcentaje = filenames.size()/100;
    //int framesPorcentaje = 1;
    int progreso = -1;
    for(size_t i = 0; i < filenames.size(); ++i)
    {
        if ( i%framesPorcentaje == 0)
        {
            progreso++;
            printf("[Porcentaje analizado: %d%]\n", progreso);
        }
        image = imread(filenames[i],CV_LOAD_IMAGE_GRAYSCALE);   // Read the file grayscale
        if (image.rows != prevImageRows || image.cols != prevImageCols)
        {
            laneDetector->Initialize(image.rows,image.cols);
        }
        laneDetector->Compute(image, lineDetector, kalman);
        straightLaneDetector = laneDetector->getStraightLaneDetector();
        //laneDetector.HomographyHoughLinesDetection(result);
        t2Start = clock();
        resultThreshold = laneDetector->getThresholdResult();
        //imwrite("../results/ridges/" + filenames[i], resultThreshold);
        resultFinal = laneDetector->getImage();
        //straightLaneDetector->LaneTypeDetection();
        //imwrite("../results/" + filenames[i], resultFinal);
        resize(resultFinal,resultFinal,Size(0,0),1.0/2,1.0/2,INTER_NEAREST);
        resultFinal = straightLaneDetector->CV8Uconvertor(resultFinal);
        resultThreshold = straightLaneDetector->CV8Uconvertor(resultThreshold);
        cvtColor(resultThreshold, resultThreshold, CV_GRAY2BGR);
        vconcat(resultFinal, resultThreshold, concatResults);
        string num = static_cast<ostringstream*>( &(ostringstream() << i+1) )->str();
        int numzeros = 5 - num.size();
        vector<char> nombre (numzeros,'0');
        std::string str(nombre.begin(), nombre.end());
        imwrite("/media/master/Datos/UAB/Results/Autopistadia" + str + num + ".tif", concatResults);
        //imwrite("../results/concate/" + filenames[i], concatResults);
        numImagen++;
        prevImageCols = image.cols;
        prevImageRows = image.rows;
        writingTime = writingTime + (double)(clock() - t2Start)/CLOCKS_PER_SEC;
    }
    printf("Numero de imagenes: %d\n", numImagen);
    printf("Time writing: %.3fs\n", writingTime);
    printf("Total time: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void testCurve()
{
    clock_t tStart = clock();
    Mat image;
    bool kalman = true;
    bool lineDetector = true;
    string filename = "2016_05_13_11_11_34_227000.tif";
    string path = "Curvas/";
    string parametersFile = "../Parameters/ParametersFileAutopista.ini";
    CLaneDetectorManager* laneDetector = new CLaneDetectorManager(parametersFile);
    CCurveLaneDetector* curveLaneDetector;
    image = imread(path + filename,CV_LOAD_IMAGE_GRAYSCALE);
    //laneDetector.PrintImage(image, "Perspective result");
    laneDetector->Initialize(image.rows, image.cols);
    laneDetector->Compute(image, lineDetector, kalman);
    //straightLaneDetector->LaneTypeDetection();
    //laneDetector.Compute(image);
    //laneDetector.HomographyHoughLinesDetection(result);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    curveLaneDetector->PrintImage(laneDetector->getCurveImage(), "result");
    curveLaneDetector->PrintImage(laneDetector->getCurveThresholdResult(), "ridge");
    //imwrite("results/" + filename, result );
}
