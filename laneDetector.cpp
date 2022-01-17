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
        // Lane lines in the format of a 4 integer vector such as  (x_1, y_1, x_2, y_2), which are the endpoint coordinates. 
        Vec4i leftLine, righLine;
        // Coordinate that represents the senter of given lane
        Point center;
    public:
        laneDetector();
        void loadFrame(Mat cameraFrame);
        void findLanes();
        void display(Mat cameraFeed);


};

laneDetector::laneDetector(){
}

void laneDetector::loadFrame(Mat cameraFrame){
    // Image preprocessing code to load camera feed
        //1. Convert to grayscale
        //2. Gaussion blur with 5x5 kernel to facilitate edge detection

    // Masking to exclude ROI
        //Make mask using OpenCV poligon and aproximate coordinates based on lane width and camera FOV
        // Exclude Region of Interest by combining mask using bitwise_and operator
}

void laneDetector::findLanes(){
    // Find all lines in frame using HoughLinesP
    // Use polyfit to find intercepts and slopes of detected lines
    // Seperate left lane (negative slope)
    // Separate right lane (positive slope)
    // For each side, find average lane
}

#endif