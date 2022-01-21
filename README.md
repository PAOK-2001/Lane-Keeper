# Lane Dectector

## Methodology
The focus of this project was to create a lane detector class for future implementation in lane keep. The approach for detecting the lanes consisted in finding the edges present in the image using gradient difference (Canny); then masking the image to only include the area corresponding to the road, upon which Hough Transform was applied, detecting probable lines in the image. A linear regression was obtained for each line, which allowed the classification into right (positive slope) and left (negative slope) boundaries; finally, the average coefficients  were found for each boundary, which described the average lines that delimited the lane.  
## Theory
### Hough Transform
Represents points as lines in Hough space, where the axis represents the values of a linear equation corresponding to a line that goes through that point . In particular, Hough transform uses the polar representation of a line  $/rho =Acos()+Bsin() where the axes are rho and theta.
For multiple points, Hough Transform detects the area in Hough space with the
most number of interceptions, returning the most probable values of the linear equation describing the collection of points. In OpenCV this is implemented as HoughLinesP, and returns the cartesian coordinates of the detected line in a vector of 4 integers Vec4i {x1,y1,x2,y1}.
Canny Edge Detection
	Canny is an algorithm that uses various states to detect the edge lines in an image. Firstly it applies a Gaussian Blur to the matrix of pixels to eliminate noise within the image. Then it uses the Sobel operator to calculate the pixel gradient in all directions. for those gradients Canny’s algorithm looks for those who are bigger than their neighbors, returning a thin edgeline . 