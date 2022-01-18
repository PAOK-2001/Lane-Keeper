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
        Mat frame;
        // Lane lines in the format of a 4 integer vector such as  (x1, y1, x2, y2), which are the endpoint coordinates. 
        Vec4i leftLine, righLine;
        // Coordinate that represents the senter of given lane
        Point center;
        // Auxiliary functions
        static pair<double,double> linearFit(Vec4i lineCoordinates);
        static double averageCoheficient(vector<double> registeredCoheficients);

    public:
        laneDetector();
        void loadFrame(Mat cameraFrame);
        void findLanes();
        void display();     
};
// Default constructor of lane detector
laneDetector::laneDetector(){
}
// Find slope (rise/run) and intercept (b=y-slope(x)) given end coordinates
// @param Vec4i containing line end points (x1,y1,x2,y2)
// @return Pair containing <slope,intercept>
pair<double,double> laneDetector::linearFit(Vec4i lineCoordinates){
    double slope     = (lineCoordinates[3] -lineCoordinates[1])/(lineCoordinates[2]-lineCoordinates[0]);
    double intercept = lineCoordinates[1] - slope*lineCoordinates[0];
    return  make_pair(slope,intercept);
}
// Find the average from a vector of cohedicientes
// @param registerCoheficients Vector of doubles
// @return avrg double representing average of vector. Retuns 0 if empty
double laneDetector::averageCoheficient(vector<double> registeredCoheficients){
    int size, sum;
    size = registeredCoheficients.size();
    if(size == 0){
        return 0;
    }
    for(auto & num: registeredCoheficients){
        sum+=num;
    }
    double avrg = sum/size;
    return avrg;
}
// loads a frame and apply preprocessiong tecnques as well as masking to crop region of interest where lanes are
// @param cameraFrame OpenCV image matrix
void laneDetector::loadFrame(Mat cameraFrame){
    // Image preprocessing code to load camera feed
    // Saturate image
    cameraFrame = cameraFrame*1.20;
    // Convert to grayscale
    Mat grayScale;
    cvtColor(cameraFrame,grayScale,COLOR_BGR2GRAY);
    // Canny edge detection
    Canny(grayScale,frame,40,150);
}
void laneDetector::findLanes(){
    // Masking to exclude ROI
    Mat mask = frame.clone();
    mask     = Scalar(0,0,0);
    // Make mask using OpenCV poligon and aproximate coordinates based on lane width and camera FOV
    // Create points for polygon
    Point p1 = Point(10,frame.rows);
    Point p2 = Point(1120,650);
    Point p3  = Point(1740,frame.rows);
    vector<Point> ROI ={p1,p2,p3};
    fillPoly(mask,ROI,(255,255,255));
    // Exclude Region of Interest by combining mask using bitwise_and operator
    bitwise_and(mask,frame,frame);
    // Find all lines in frame using HoughLinesP
    vector<Vec4i> lines;
    // Uses HoughTransform to fine most probable line in canny image.
    HoughLinesP(frame,lines,2,CV_PI/180,150,20);
    // Find regression for lines
    vector<double> rightSide_slopes, rightSide_intercepts, leftSide_slopes,leftSide_intercepts;
    for(auto &line: lines){
        pair<double, double> fit =linearFit(line);
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
    double rightSlope     = averageCoheficient(rightSide_slopes);
    double rightIntercept = averageCoheficient(rightSide_intercepts);
    double leftSlope      = averageCoheficient(leftSide_slopes);
    double leftIntercept  = averageCoheficient(leftSide_intercepts);
    // Make coordinates for final lanes
    
}

void laneDetector::display(){
    imshow("Dash cam",frame);
}

int main(){
    laneDetector lanes;
    // Create OpenCV frame object to store frame information
    Mat frame;
    // Create VideoCapture object
    VideoCapture dashCam("Test Footage/Test2.mp4");
    // Check if the dashCam is readable
    if(!dashCam.isOpened()){
        cout<<"Error reading dashCam feed\n";
        return -1;
    }
    for (;;){
        dashCam.read(frame);
        // Check if selected device is sending information
        if(frame.empty()){
            cout<<"NULL frame ";
            break;
        }
        lanes.loadFrame(frame);
        lanes.findLanes();
        lanes.display();
        // Run face recognition function
        // Read key board input, setting esc as break key
        if(waitKey(5)== 27){
            break;
        }
    }
    return 0;
}