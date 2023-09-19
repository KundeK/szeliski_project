#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat img = imread("Pictures/truck.jpg", IMREAD_GRAYSCALE);
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
       Sobel(src Mat, output Mat, output depth, dx, dy, kernel size, scale factor, delta value)
    */
    Mat sobel_img;
    Sobel(img, sobel_img, CV_8UC1, 1, 1, 3, 1, 1);
    imwrite("Pictures/sobel_img.png", sobel_img);

    // dx=0, so the output will show horizontal lines
    Mat dx_sobel_img;
    Sobel(img, dx_sobel_img, CV_8UC1, 0, 1, 3, 1, 1);
    imwrite("Pictures/dx_sobel_img.png", dx_sobel_img);

    // dy=0, so the output will show vertical lines
    Mat dy_sobel_img;
    Sobel(img, dy_sobel_img, CV_8UC1, 1, 0, 3, 1, 1);
    imwrite("Pictures/dy_sobel_img.png", dy_sobel_img);

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


    /* Laplacian operator
       Calculates the Laplacian of the source image by adding up the second x and y derivatives calculated using Sobel operator:
       dst = delta(src) = (d^2*src)/(dx^2) + (d^2*src)/(dy^2)  **d refers to derivative and ^ refers to the order not exponent.
       The calculation is done this way if kernel size > 1, other wise if kernel size == 1, then image is filtered with
       [[0, 1, 0],
        [1,-4, 1],
        [0, 1, 0]]
       Laplacian(src Mat, output Mat, output depth, kernel size, scale factor, delta value)
    */
    Mat laplacian_img;
    Laplacian(img, laplacian_img, -1, 1, 1, 1);
    imwrite("Pictures/laplacian_img.png", laplacian_img);

    // Kernel size is optimal at 3 but can be 1, 3, 5, 7, and so on. Higher the size, the more textured, jagged, and noisy output.
    Mat kernel_laplacian_img;
    Laplacian(img, kernel_laplacian_img, -1, 7, 1, 1);
    imwrite("Pictures/kernel_laplacian_img.png", kernel_laplacian_img);

    // Increasing scale factor makes the approximately non-0 (non-black) RGB values brighter
    Mat scale_laplacian_img;
    Laplacian(img, scale_laplacian_img, -1, 1, 5, 1);
    imwrite("Pictures/scale_laplacian_img.png", scale_laplacian_img);

    // Increasing delta value makes the approximately non-255 (non-white) RGB values brighter
    Mat delta_laplacian_img;
    Laplacian(img, delta_laplacian_img, -1, 1, 1, 5);
    imwrite("Pictures/delta_laplacian_img.png", delta_laplacian_img);


    /* Gaussian Pyramid
      Higher level (Low resolution) in a Gaussian Pyramid is formed by removing consecutive rows and columns in Lower level 
      (higher resolution) image. Then each pixel in higher level is formed by the contribution from 5 pixels in underlying level 
      with gaussian weights. By doing so, a M×N image becomes M/2×N/2 image. So area reduces to one-fourth of original area. 
      It is called an Octave. The same pattern continues as we go upper in pyramid (ie, resolution decreases). Similarly while 
      expanding, area becomes 4 times in each level. We can find Gaussian pyramids using cv.pyrDown() and cv.pyrUp() functions.
    */
    Mat lowerRes_img;
    pyrDown(img, lowerRes_img);
    pyrDown(lowerRes_img, lowerRes_img);
    pyrDown(lowerRes_img, lowerRes_img);
    pyrDown(lowerRes_img, lowerRes_img);
    imwrite("Pictures/lowerRes_img.png", lowerRes_img);

    Mat higherRes_img;
    pyrUp(lowerRes_img, higherRes_img);
    pyrUp(higherRes_img, higherRes_img);
    pyrUp(higherRes_img, higherRes_img);
    pyrUp(higherRes_img, higherRes_img);
    imwrite("Pictures/higherRes_img.png", higherRes_img);

    /* Laplacian Pyramid
       Laplacian Pyramids are formed from the Gaussian Pyramids. There is no exclusive function for that. Laplacian pyramid images are 
       like edge images only. Most of its elements are zeros. They are used in image compression. A level in Laplacian Pyramid is formed 
       by the difference between that level in Gaussian Pyramid and expanded version of its upper level in Gaussian Pyramid.
    */
    

    return 0;
}