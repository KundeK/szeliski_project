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

   
    return 0;
}