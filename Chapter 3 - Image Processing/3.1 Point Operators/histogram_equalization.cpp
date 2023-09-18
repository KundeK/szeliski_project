#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    //Load original image as a grayscale
    Mat img = imread("Pictures/truck.jpg", IMREAD_GRAYSCALE);
    if(img.empty()) {
        cout << "Failed to read image" << endl;
        return 1;
    }
    
    /* Applying Histogram Equalization
       By using Histogram Equalization techniques, we enhance contrast by spreading out the most frequent intensity values,
       improve visibility, and help standardize sets of images for processing. equalizeHist(src Mat, output Mat)
    */
    Mat histEqualization_img;
    equalizeHist(img, histEqualization_img);
    imwrite("Pictures/histEqualization_img.png", histEqualization_img);

    return 0;
}