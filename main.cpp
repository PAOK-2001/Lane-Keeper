// Lane Detector
#include "laneDetector.cpp"
#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;
using namespace cv;

int main(){
    laneDetector lanes;
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
        // Check if selected source is sending information
        if(frame.empty()){
            cout<<"NULL frame ";
            break;
        }
        // Load image into lane detector
        lanes.loadFrame(frame);
        // Find lanes on given frame
        lanes.findLanes();
        lanes.findCenter();
        // Overlap lanes on the video
        lanes.display(frame);
        
        // Wait 5 miliseconds
        // Read key board input, setting esc as break key
        if(waitKey(5)== 27){
            break;
        }
    }
    return 0;
}