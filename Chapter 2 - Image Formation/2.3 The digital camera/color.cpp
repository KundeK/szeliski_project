#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat img = imread("Pictures/truck.jpg", IMREAD_COLOR);
    if(img.empty()) {
        cout << "Failed to read image" << endl;
        return 1;
    }

    /* Convert to HSV color space
       Reduces the influence of light intensity or shadows from the outside by separating luma (image intensity) from chroma (color information).
    */
    Mat hsv_img;
    cvtColor(img, hsv_img, COLOR_BGR2HSV);
    imwrite("Pictures/hsv_img.png", hsv_img);

    /* Convert to Grayscale
       Reduce the amount of data needed to store the image by a factor of 3 by only keeping luminance and easier to perform edge detection and thresholding.
    */
    Mat gray_img;
    cvtColor(img, gray_img, COLOR_BGR2GRAY);
    imwrite("Pictures/gray_img.png", gray_img);

    return 0;
}