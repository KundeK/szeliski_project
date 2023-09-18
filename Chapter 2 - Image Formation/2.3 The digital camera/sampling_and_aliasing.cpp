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

    /* Downscale image with aliasing
       1) Data is more manageable in size
       2) Reduces dimensionality of the data for faster processing of the image
       3) Reduce the storage size of the data
    ** Can be achieved with Mipmap, Box Sampling, and Sinc algorithms
    */
    Mat downSampled_img;
    resize(img, downSampled_img, Size(), 0.25, 0.25, INTER_NEAREST);
    imwrite("Pictures/downSampled_img.png", downSampled_img);

    /* Upscale image with Nearest-Neighbor sampling
       Increasing the quality of an image, the opposite objective of downsampling. 
       resize(src Mat, output Mat, output size, fx, fy, interpolation type)
    */
    Mat upNearest_img;
    resize(downSampled_img, upNearest_img, img.size(), 0, 0, INTER_NEAREST);
    imwrite("Pictures/upNearest_img.png", upNearest_img);
    
    /* Upscale image with Bilinear Interpolation sampling
       Increasing the quality of an image, the opposite objective of downsampling. 
       resize(src Mat, output Mat, output size, fx, fy, interpolation type)
    */
    Mat upBilinear_img;
    resize(downSampled_img, upBilinear_img, img.size(), 0, 0, INTER_LINEAR);
    imwrite("Pictures/upBilinear_img.png", upBilinear_img);

    /* Anti-aliasing via gaussian blur technique
       Smoothing the image out to reduce the noise in an image and any high frequency components that 
       can cause false edges to be detected. GaussianBlur(src Mat, output Mat, kernel size, std x-direction, std y-direction)
    */
    Mat antiAliased_img;
    GaussianBlur(downSampled_img, antiAliased_img, Size(5, 5), 1.5, 1.5);
    resize(antiAliased_img, antiAliased_img, img.size(), 0, 0, INTER_LINEAR);
    imwrite("Pictures/antiAliased_img.png", antiAliased_img);

    return 0;
}