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
        lanes.display(frame);
        
        // Run face recognition function
        // Read key board input, setting esc as break key
        if(waitKey(5)== 27){
            break;
        }
    }
    return 0;
}