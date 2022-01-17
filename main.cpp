#include"laneDetector.cpp"

#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

int main(){
    laneDetector lanes;
    // Create OpenCV frame object to store frame information
    Mat frame;
    // Create VideoCapture object, reading video device (USB camera)
    VideoCapture camera(0);
    // Check if the camera is readable
    if(!camera.isOpened()){
        cout<<"Error reading camera feed";
        return -1;
    }
    for (;;){
        camera.read(frame);
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
