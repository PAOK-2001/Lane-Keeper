# Lane Detector

## Methodology
The focus of this project was to create a lane detector class for future implementation in lane keep. The approach for detecting the lanes consisted in finding the edges present in the image using gradient difference (Canny); then masking the image to only include the area corresponding to the road, upon which Hough Transform was applied, detecting probable lines in the image. A linear regression was obtained for each line, which allowed the classification into right (positive slope) and left (negative slope) boundaries; finally, the average coefficients  were found for each boundary, which described the average lines that delimited the lane.  
## Theory
### Hough Transform
Represents points as lines in Hough space, where the axis represents the values of a linear equation corresponding to a line that goes through that point . In particular, Hough transform uses the polar representation of a line      &rho;= Acos(&theta;)+Bcos(&theta;) where the axes are &rho; and &theta;.

For multiple points, Hough Transform detects the area in Hough space with the
most number of interceptions, returning the most probable values of the linear equation describing the collection of points. In OpenCV this is implemented as HoughLinesP, and returns the cartesian coordinates of the detected line in a vector of 4 integers Vec4i {x1,y1,x2,y1}.

![Hough Space](MarkDown/HoughTransform.jpg?raw=true "HoughTransform")

### Canny Edge Detection

Canny is an algorithm that uses various states to detect the edge lines in an image. Firstly it applies a Gaussian Blur to the matrix of pixels to eliminate noise within the image. 
    
Then it uses the Sobel operator to calculate the pixel gradient in all directions. for those gradients Canny’s algorithm looks for those who are bigger than their neighbors, returning a thin edgeline. 

## Lane Detector Object

    class laneDetector{
        private:
            // Images states used in lane detection
            Mat edgeImg, lineImg;
            // Lane lines in the format of a 4 integer vector such as  (x1, y1, x2, y2), 
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
            void findCenter();
            void display(Mat cameraFrame);
    };
### Class Composition
#### Atributes
| Atribute | Description |
| --- | ----- |
| edgeIMG | Pixel matrix that stores the Canny version of the loaded frame.![Canny](MarkDown/edgeIMG.jpg?raw=true "edgeIMG")  |
| lineIMG | Pixel matrix that stores the drawed lines and center point that will blended with the orignal image in the final display. ![lineIMG](MarkDown/lineIMG.jpg?raw=true "edgeIMG") |
| leftLine, rightLine | A Vec4i structure that stores the coordinates of the endpoints of the respective lines that delimit the lane. Format: {x1, y1, x2, y2}|
|center|Point object that stores the coordinates for the center of the lanes|
#### Methods
##### Static
|Method| Description  |
| --- | ----- |
|linearFit(Vec4i lineCoordinates)|Takes a vector of 4 integers representing the endpoints of a line and returns the slope and intercept as a pair.|
|averageCoheficient(vector<float> registeredCoheficients)|Takes a vector of coheficients and returns the average value.|
|makeCoordinate(float slope, float intercept, int imgHeight)|Takes the coheficients of a linear equation and the height of the image and calcultates the coordinates of a a line.|
##### Non-Static
|Method| Description  |
| --- | ----- |
|void loadFrame(Mat cameraFrame) | Loads the image where the lanes wish to be found. It applies converts it to grayscale, and applies Canny Edge Detection, storing the result in *edgeIMG*.|
|void findLanes()| It makes mask using polygons to isolate the area where the road is, updating *edgeIMG*. Once the region on interest is obtained Hough Transform is used to detect the lines present; afterwards the **linearFit** method is used to find the slope and intercept for each line. Based on the slope, the lines are categorized into right and left boundaries. Finally, it finds the average lines for each side using **averageCoheficient** and it makes the endpoint coordinates (using **makeCoordinates**) to store them in *rightLine* and *leftLine*. It draws said lines on *lineIMG*|
|void findCenter()|Finds the center of the lane using the coordinates stored in *rightLine* and *leftLine*. Draws the center as a circle in *lineIMG*|
|void display(Mat cameraFrame) | Overlays the *lineIMG* on the real fram captured by the camera. |

# Implementaion
The Lane Detector can be used to find lanes in a single image or a video (read from a file or camera.)

**Still image:**

    laneDetector lanes;
    frame = imread("imageName");
    lanes.loadFrame(frame);
    lanes.findLanes();
    lanes.findCenter();
    lanes.display(frame);

**Camera:**

    VideoCapture dashCam(path);
    for (;;){
        dashCam.read(frame);
        lanes.loadFrame(frame);
        lanes.findLanes();
        lanes.findCenter();
        lanes.display(frame);
        if(waitKey(5)== 27){
            break;
        }
    }

## Demo

### Light Curve
![Test1](MarkDown/Test2.gif?raw=true "Test1")

### Regular Curve
![Test2](MarkDown/Test1.gif?raw=true "Test2")
