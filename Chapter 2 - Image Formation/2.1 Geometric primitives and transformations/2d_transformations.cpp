#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Read an image from file
    Mat img = imread("Pictures/truck.jpg");
    Mat proj_img = imread("Pictures/sudoku.jpg");
    if (img.empty() || proj_img.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    /* Translation transformation (vector addition, preserves orientation):
    1) Create Mat output object and translation values, in this case we shift positively by quarter of width and height
    2) Acquire the 2x3 Mat transformation matrix by setting with (Mat_<double>(2, 3) << 1, 0, tx, 0, 1, ty)
                                                                                       ^^ [[1, 0, tx], ^^
                                                                                           [0, 1, ty]]
    3) Apply transformation with warpAffine(src Mat, output Mat, transformation Mat, original size)
    */
    Mat translated_img;
    int width = img.cols, height = img.rows;
    int tx = height/4;
    int ty = width/4;
    Mat translate_matrix = (Mat_<double>(2, 3) << 1, 0, tx, 0, 1, ty);
    warpAffine(img, translated_img, translate_matrix, img.size());
    imwrite("Pictures/translated_img.png", translated_img);


    /* Rotation transformation (linear transformation, preserves lengths):
    1) Create Mat output object
    2) Determine center of the image
    3) Acquire the Mat 2x3 transformation matrix with getRotationMatrix2d(center, degrees, scale)
    4) Apply transformation with warpAffine(src Mat, output Mat, transformation Mat, original size)
    */
    Mat rotated_img;
    Point2f center(img.cols/2.0, img.rows/2.0);
    Mat rotate_matrix = getRotationMatrix2D(center, 45, 1.0);
    warpAffine(img, rotated_img, rotate_matrix, img.size());
    imwrite("Pictures/rotated_img.png", rotated_img);
    

    /* Scaling transformation (linear transformation, preserves angles):
    1) Create Mat output object
    2) Scale original image with resize(src Mat, output Mat, size(scalar*img.cols, scalar*img.rows))
    */
    Mat scaled_img;
    resize(img, scaled_img, Size(.5*img.cols, .5*img.rows));
    imwrite("Pictures/scaled_img.png", scaled_img);


    /* Affine transformation (linear transformation, preserves parallelism):
    1) Create Mat output object
    2) Create 1D arrays for source and destination points for affine transformation
    3) Acquire the Mat 2x3 transformation matrix with getAffineTransform(src array, dst array)
    4) Apply transformation with warpAffine(src Mat, output Mat, transformation Mat, output size)
    */
    Mat affined_img;
    Point2f srcQuadA[] = {Point2f(0, 0), Point2f(img.cols-1, 0), Point2f(0, img.rows-1), Point2f(img.cols-1, img.rows-1)};
    Point2f dstQuadA[] = {Point2f((img.cols-1)/2, 0), Point2f(img.cols-1, 0), Point2f(0, img.rows-1), Point2f((img.cols-1)/2, img.rows-1)};
    Mat affine_matrix = getAffineTransform(srcQuadA, dstQuadA);
    warpAffine(img, affined_img, affine_matrix, affined_img.size());
    imwrite("Pictures/affined_img.png", affined_img);


    /* Perpsective transformation (preserves straight lines):
    1) Create Mat output object
    2) Create 1D arrays for source and destination points for projection transformation
    3) Acquire the Mat 2x3 transformation matrix with getPerspectiveTransform(src array, dst array)
    4) Apply the transformation with warpAffine(src Mat, output Mat, transformation Mat, output size)
        **The original dimensions are 610x507 and the dstQuad points also have difference of approximately 600x500
    */
    // Easier to eyeball sudoku coordinates since the z-axis isn't as prevalent in this example
    Mat sudoku_img;
    Point2f srcQuadP1[] = {Point2f(2, 124), Point2f(412, 1), Point2f(142, 486), Point2f(608, 329)};
    Point2f dstQuadP1[] = {Point2f(25, -100), Point2f(650, -100), Point2f(25, 450), Point2f(675, 450)};
    Mat perspective_matrix1 = getPerspectiveTransform(srcQuadP1, dstQuadP1);
    warpPerspective(proj_img, sudoku_img, perspective_matrix1, sudoku_img.size());
    imwrite("Pictures/sudoku_img.png", sudoku_img);

    // Much harder to eyeball truck coordinates since there is a lot of z-axis involved, raises question of 3D object to 2D image
    Mat projected_img;
    Point2f srcQuadP2[] = {Point2f(67, 199), Point2f(279, 19), Point2f(65, 304), Point2f(281, 382)};
    Point2f dstQuadP2[] = {Point2f(-67, -199), Point2f(1210, -199), Point2f(-67, 810), Point2f(1210, 810)};
    Mat perspective_matrix2 = getPerspectiveTransform(srcQuadP2, dstQuadP2);
    warpPerspective(img, projected_img, perspective_matrix2, projected_img.size());
    imwrite("Pictures/projected_img.png", projected_img);
    
    return 0;
}