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

    /* Sobel operator
       Combine Gaussian smoothing and differentiation, so the result is more or less resistant to the noise.
       Calculates output image derivative by convolving image with appropriate kernel:
       dst = (d^(xorder+yorder)src)/((dx^xorder)(dy^yorder))  **d refers to derivative and ^ refers to the order not exponent.
       Most often the function is called with (xorder=1, yorder=0, ksize=3) or (xorder=0, yorder=1, ksize=3),
       respectively with kernels of [[-1, 0, 1],] and [[-1, -2, -1],]
                                     [-2, 0, 2],       [ 0,  0,  0],
                                     [-1, 0, 1]]       [ 1,  2,  1]]
       Sobel(src Mat, output Mat, data type, dx, dy, kernel size, scale factor, delta value)
    */
    Mat sobel_img;
    Sobel(img, sobel_img, CV_8UC1, 1, 1, 3, 1, 1);
    imwrite("Pictures/sobel_img.png", sobel_img);

    // dx=0, so the output will show vertical lines
    Mat dy_sobel_img;
    Sobel(img, dy_sobel_img, CV_8UC1, 0, 1, 3, 1, 1);
    imwrite("Pictures/dy_sobel_img.png", dy_sobel_img);

    // dy=0, so the output will show horizontal lines
    Mat dx_sobel_img;
    Sobel(img, dx_sobel_img, CV_8UC1, 1, 0, 3, 1, 1);
    imwrite("Pictures/dx_sobel_img.png", dx_sobel_img);

    // Kernel size is optimal at 3 but can be 1, 3, 5, 7, and so on. Higher the size, the more textured, jagged, and noisy output.
    Mat kernel_sobel_img;
    Sobel(img, kernel_sobel_img, CV_8UC1, 1, 1, 7, 1, 1);
    imwrite("Pictures/kernel_sobel_img.png", kernel_sobel_img);

    // Increasing scale factor makes the approximately non-0 (non-black) RGB values brighter
    Mat scale_sobel_img;
    Sobel(img, scale_sobel_img, CV_8UC1, 1, 1, 3, 5, 1);
    imwrite("Pictures/scale_sobel_img.png", scale_sobel_img);

    // Increasing delta value makes the approximately non-255 (non-white) RGB values brighter
    Mat delta_sobel_img;
    Sobel(img, delta_sobel_img, CV_8UC1, 1, 1, 3, 1, 5);
    imwrite("Pictures/delta_sobel_img.png", delta_sobel_img);

    return 0;
}