#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/*
Formula for changing brightness and contrast is: output_img = input_img*alpha + beta. The values of 
alpha and beta are responsible for contrast and brightness respectively with contrast being 1.0<= beta <=3.0
and brightness being 0<= alpha <=100. We use special function convertTo(output_img, -1, alpha, beta).
*/
void brightness(Mat input_img, double alpha, double beta) {
    
    Mat output_img;
    input_img.convertTo(output_img, -1, alpha, beta);
    imwrite("Pictures/bright_img.png", output_img);
}

int main() {
    Mat img = imread("Pictures/truck.jpg", IMREAD_COLOR);
    if(img.empty()) {
        cout << "Could not read image" << endl;
        return 1;
    }

    brightness(img, 1.0, 50);

    return 0;
}