#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat img = imread("Pictures/truck.jpg");
    if(img.empty()) {
        cout << "Failed to read image" << endl;
        return 1;
    }
    
    // Convert to grayscale
    Mat gray_img;
    cvtColor(img, gray_img, COLOR_BGR2GRAY);

    /* Perform binary thresholding
       Helps to separate regions of interest in an image from the backround. It is computationally inexpensive, 
       helps in isolating specific objects or features in an image, and removes a certain level of detail which
       might include noise. threshold(src Mat, output Mat, threshold value, max value for threshold type, threshold type)
    */
    Mat binaryThresholded_img;
    threshold(gray_img, binaryThresholded_img, 127, 255, THRESH_BINARY);
    imwrite("Pictures/binaryThresholded_img.png", binaryThresholded_img);

    return 0;
}