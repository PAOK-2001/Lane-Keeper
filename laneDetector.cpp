#ifndef LANEDETECTOR_CPP
#define LANEDETECTOR_CPP

#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace std;
using namespace cv;

class laneDetector{
    private:
        // Region of intereset from where to detect lines
        Mat edgeImg, lineImg;
        // Lane lines in the format of a 4 integer vector such as  (x1, y1, x2, y2), which are the endpoint coordinates. 
        Vec4i leftLine, rightLine;
        // Coordinate that represents the senter of given lane
        Point center;
        // Auxiliary functions
        static pair<float,float> linearFit(Vec4i lineCoordinates);
        static float averageCoheficient(vector<float> registeredCoheficients);
        static Vec4i makeCoordinate(float slope, float intercept, int imgHeight);

    public:
        laneDetector();
        void loadFrame(Mat cameraFrame);
        void findLanes();
        void display(Mat cameraFrame);
};
// Default constructor of lane detector
laneDetector::laneDetector(){
}
// Find slope (rise/run) and intercept (b=y-slope(x)) given end coordinates
// @param Vec4i containing line end points (x1,y1,x2,y2)
// @return Pair containing <slope,intercept>
pair<float,float> laneDetector::linearFit(Vec4i lineCoordinates){
    float slope     = (lineCoordinates[3] -lineCoordinates[1])/(float)(lineCoordinates[2]-lineCoordinates[0]);
    float intercept =  lineCoordinates[1] - slope*lineCoordinates[0];
    return  make_pair(slope,intercept);
}
// Find the average from a vector of cohedicientes
// @param registerCoheficients Vector of floats
// @return avrg float representing average of vector. Retuns 0 if empty
float laneDetector::averageCoheficient(vector<float> registeredCoheficients){
    int size, sum;
    size = registeredCoheficients.size();
    if(size == 0){
        return 0;
    }
    for(auto & num: registeredCoheficients){
        sum+=num;
    }
    float avrg = sum/size;
    return avrg;
}
// computes coordinate based on slope and intercpet of line equation
// @param slope, intercept float representing linear equation values
// @return coordinates Vec4i in format [x1,y1,x2,y2]
Vec4i laneDetector::makeCoordinate(float slope, float intercept, int imgHeight){
    int x1, y1, x2, y2;
    y1 = imgHeight;
    y2 = round(imgHeight*(2.0/5.0));
    x1 = round((y1-intercept)/slope);
    x2 = round((y2-intercept)/slope);
    Vec4i returnCoordinates = {x1,y1,x2,y2};
    return returnCoordinates;
}
// loads a frame and apply preprocessiong tecnques as well as masking to crop region of interest where lanes are
// @param cameraFrame OpenCV image matrix
void laneDetector::loadFrame(Mat cameraFrame){
    // Image preprocessing code to load camera feed
    // Saturate image
    lineImg = cameraFrame.clone();
    lineImg = Scalar(0,0,0);
    cameraFrame = cameraFrame*1.20;
    // Convert to grayscale
    Mat grayScale;
    cvtColor(cameraFrame,grayScale,COLOR_BGR2GRAY);
    // Canny edge detection
    Canny(grayScale,edgeImg,40,150);
}
void laneDetector::findLanes(){
    // Masking to exclude ROI
    Mat mask = edgeImg.clone();
    mask     = Scalar(0,0,0);
    // Make mask using OpenCV poligon and aproximate coordinates based on lane width and camera FOV
    // Create points for polygon
    Point p1 = Point(10,mask.rows);
    Point p2 = Point(1120,650);
    Point p3  = Point(1740,mask.rows);
    vector<Point> ROI ={p1,p2,p3};
    fillPoly(mask,ROI,(255,255,255));
    // Exclude Region of Interest by combining mask using bitwise_and operator
    bitwise_and(mask,edgeImg,edgeImg);
    // Find all lines in frame using HoughLinesP
    vector<Vec4i> lines;
    // Uses HoughTransform to fine most probable line in canny image.
    HoughLinesP(edgeImg,lines,1,CV_PI/180,145,20);
    // Find regression for lines
    vector<float> rightSide_slopes, rightSide_intercepts, leftSide_slopes,leftSide_intercepts;
    for(auto &lineP: lines){
        pair<float, float> fit =linearFit(lineP);
        // Seperate left lane (negative slope)
        if(fit.first<0){
            leftSide_slopes.push_back(fit.first);
            leftSide_intercepts.push_back(fit.second);
        }
        // Separate right lane (positive slope)
        else{
            rightSide_slopes.push_back(fit.first);
            rightSide_intercepts.push_back(fit.second);
        }
    }
    // For each side, find average lane
    float rightSlope     = averageCoheficient(rightSide_slopes);
    float rightIntercept = averageCoheficient(rightSide_intercepts);
    float leftSlope      = averageCoheficient(leftSide_slopes);
    float leftIntercept  = averageCoheficient(leftSide_intercepts);
    // Make coordinates for final lanes
    leftLine  = makeCoordinate(leftSlope, leftIntercept, edgeImg.rows);
    rightLine = makeCoordinate(rightSlope, rightIntercept, edgeImg.rows);
    cout<<leftLine<<endl;
    // Draw line on lineImg
    line(lineImg,Point(leftLine[0],leftLine[1]),Point(leftLine[2],leftLine[3]),(255,0,0),5);
}
void laneDetector::display(Mat cameraFrame){
    
    
    imshow("Dash cam",lineImg);
}

#endif