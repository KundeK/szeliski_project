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

    Mat gaussianBlur_img;
    Size kernelSize = Size(5,5); //5x5 Gaussian kernel
    double sigma = 1.5;          //Standard deviation

    //First apply Gaussian blur along X-axis
    Mat XgaussianBlur_img;
    GaussianBlur(img, XgaussianBlur_img, Size(kernelSize.width, 1), sigma, 0);

    //Gaussian blur along Y-axis on XgaussianBlur
    GaussianBlur(XgaussianBlur_img, gaussianBlur_img, Size(1, kernelSize.height), 0, sigma);

    /* Overall
       2D filter is said to be separable if it can be divided into two 1D filters, which can make 
       the filter more computationally efficient. In this scenario the Gaussian blur is 
       separated into two different 1D  filters to reduce noise and detail.
       GaussianBlur(src Mat, output Mat, kernel size, std x-direction, std y-direction)
    */ 
    imwrite("Pictures/separableFiltered_img.png", gaussianBlur_img);
    
    return 0;
}