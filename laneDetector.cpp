#ifndef LANEDECTOR_CPP
#define LANEDECTOR_CPP

#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

class laneDetector{
    private:
        // Region of intereset from where to detect lines
        Mat ROI;
        // Lane lines in the format of a 4 integer vector such as  (x1, y1, x2, y2), which are the endpoint coordinates. 
        Vec4i leftLine, righLine;
        // Coordinate that represents the senter of given lane
        Point center;


    public:
        laneDetector();
        void loadFrame(Mat cameraFrame);
        void findLanes();
        void display(Mat cameraFeed);
        // Auxiliary functions
        pair<double,double> linearFit(Vec4i lineCoordinates);
       

};

// Find slope (rise/run) and intercept (b=y-slope(x)) given end coordinates
// @param Vec4i containing line end points (x1,y1,x2,y2)
// @return Pair containing <slope,intercept>
pair<double,double> laneDetector::linearFit(Vec4i lineCoordinates){
    double slope     = (lineCoordinates[3] -lineCoordinates[1])/(lineCoordinates[2]-lineCoordinates[0]);
    double intercept = lineCoordinates[1] - slope*lineCoordinates[0];
    return  make_pair(slope,intercept);
}

 
// Default constructor of lane detector
laneDetector::laneDetector(){
}
// loads a frame and apply preprocessiong tecnques as well as masking to crop region of interest where lanes are
// @param cameraFrame OpenCV image matrix
void laneDetector::loadFrame(Mat cameraFrame){
    // Image preprocessing code to load camera feed
    //1. Convert to grayscale
    Mat grayScale;
    cvtColor(cameraFrame,grayScale,COLOR_BGR2GRAY);
    //2. Gaussion blur with 5x5 kernel to facilitate edge detection
    Mat proccesedImage;
    GaussianBlur(grayScale,proccesedImage,Size(5,5),0);
    // Masking to exclude ROI
    Mat mask;
    //Make mask using OpenCV poligon and aproximate coordinates based on lane width and camera FOV
    
    // Exclude Region of Interest by combining mask using bitwise_and operator
}

void laneDetector::findLanes(){
    // Find all lines in frame using HoughLinesP

    // Seperate left lane (negative slope)
    // Separate right lane (positive slope)
    // For each side, find average lane
    // Make coordinates for final lanes
}

#endif