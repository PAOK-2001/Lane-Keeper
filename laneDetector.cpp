

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


    public:
        laneDetector();
        void loadFrame(Mat cameraFrame);
        void findLanes();
        void display();
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
    //1. Saturate image
    cameraFrame = cameraFrame*1.5;
    //2. Convert to grayscale
    Mat grayScale;
    cvtColor(cameraFrame,grayScale,COLOR_BGR2GRAY);
    //3. Gaussion blur with 5x5 kernel to facilitate edge detection
    Mat blurred;
    GaussianBlur(grayScale,blurred,Size(5,5),0);
    // Canny edge detection
    Canny(blurred,frame,5,150);
}

void laneDetector::findLanes(){
    // Masking to exclude ROI
    Mat mask = frame.clone();
    mask     = Scalar(0,0,0);
    //Make mask using OpenCV poligon and aproximate coordinates based on lane width and camera FOV

    // Exclude Region of Interest by combining mask using bitwise_and operator
    // Find all lines in frame using HoughLinesP
    // Seperate left lane (negative slope)
    // Separate right lane (positive slope)
    // For each side, find average lane
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
    VideoCapture dashCam("Test Footage/Test1.mp4");
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
