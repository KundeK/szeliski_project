#include <opencv2/core.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void loadPoints(const string& filename, vector<Point3f>& points) {
    ifstream in(filename);
    float x, y, z;
    while(in >> x >> y >> z) {
        points.push_back(Point3f(x, y, z));
    }
}

void savePoints(const string& filename, const vector<Point3f>& points) {
    ofstream out(filename);
    for(const auto& point : points) {
        out << point.x << " " << point.y << " " << point.z << endl;
    }
}

/*
1) Initialize the 3x3 rotation matrix with [[cos(radians), -sin(radians), 0], 
                                            [sin(radians), cos(radians), 0], 
                                            [0, 0, 1]]
2) Matrix multiply the original points against the rotation matrix
*/
void rotatePoints(vector<Point3f>& points, float angleDegrees) {
    Mat rotate_matrix = (Mat_<float>(3, 3) <<
                            cos(angleDegrees * CV_PI / 180), -sin(angleDegrees * CV_PI / 180), 0,
                            sin(angleDegrees * CV_PI / 180), cos(angleDegrees * CV_PI / 180), 0,
                            0, 0, 1);

    for(auto& point : points) {
        Mat point_matrix = (Mat_<float>(3,1) << point.x, point.y, point.z);
        point_matrix = rotate_matrix * point_matrix;
        point = Point3f(point_matrix.at<float>(0), point_matrix.at<float>(1), point_matrix.at<float>(2));
    }
}

/*
1) For every 3D point, add the translation values to each dimension of the point 
*/
void translatePoints(vector<Point3f>& points, float dx, float dy, float dz) {
    for(auto& point : points) {
        point.x += dx;
        point.y += dy;
        point.z += dz;
    }
}

/*
1) For every 3D point, multiply the scalar values to each dimension of the point
*/
void scalePoints(vector<Point3f>& points, float sx, float sy, float sz) {
    for(auto& point : points) {
        point.x *= sx;
        point.y *= sy;
        point.z *= sz;
    }
}

/*
1) Matrix multiply the original points against the 4x4 affine matrix
    [[1, Sxy, Sxz, Tx],
     [Syx, 1, Syz, Ty],
     [Szx, Szy, 1, Tz],
     [0, 0, 0, 1]]
*/
void affinePoints(vector<Point3f>& points, Mat& affine_matrix) {
    for(auto& point : points) {
        Mat point_matrix = (Mat_<float>(4, 1) << point.x, point.y, point.z, 1);
        point_matrix = affine_matrix * point_matrix;
        point = Point3f(point_matrix.at<float>(0), point_matrix.at<float>(1), point_matrix.at<float>(2));
    }
}

/*
1) Matrix multiply the original points against the 4x4 perspective matrix
**Important to note that we are converting homogenous coords to cartesian coords by dividing w'
**If we want to do a 3D to 2D projection, we would only return 2 of the 3 dimensions
*/
void perspectivePoints(vector<Point3f>& points, Mat& perspective_matrix) {
    for(auto& point : points) {
        Mat point_matrix = (Mat_<float>(4, 1) << point.x, point.y, point.z, 1);
        point_matrix = perspective_matrix * point_matrix;
        point = Point3f(point_matrix.at<float>(0) / point_matrix.at<float>(3), point_matrix.at<float>(1) / point_matrix.at<float>(3), point_matrix.at<float>(2) / point_matrix.at<float>(3));
    }
}

int main() {
    vector<Point3f> points, rotate_points, translate_points, scale_points, affine_points, perspective_points;
    loadPoints("cube.txt", points);
    rotate_points = points;
    translate_points = points;
    scale_points = points;
    affine_points = points;
    perspective_points = points;

    //Rotation by 45 degrees
    rotatePoints(rotate_points, 45);
    savePoints("Points/rotate_points.txt", rotate_points);

    //Translate +10, +20, and +30 for the x, y, and z coordinates respectively
    translatePoints(translate_points, 10, 20, 30);
    savePoints("Points/translate_points.txt", translate_points);
    
    //Multiply the x, y and z coordinates with a scalar value of 2
    scalePoints(scale_points, 2, 2, 2);
    savePoints("Points/scale_points.txt", scale_points);


    Mat affine_matrix = (Mat_<float>(4,4) << 1, .2, .1, 10, //Shear in x-direction by .2 and .1 relative to y and z and translation of +10
                                             .3, 1, .1, 10, //Shear in y-direction by .3 and .1 relative to x and z and translation of +10
                                             .1, .2, 1, 10, //Shear in z-direction by .1 and .2 relative to x and y and translation of +10
                                             0, 0, 0, 1);
    affinePoints(affine_points, affine_matrix);
    savePoints("Points/affine_points.txt", affine_points);

    //Important to note that we want to avoid points at infinity or having inversion
    Mat perspective_matrix = (Mat_<float>(4,4) << 1, 0, 0, 0,
                                                  0, 1, 0, 0,
                                                  0, 0, 1, 0,
                                                  0, 0, -.1, 1); //Depth perspective along z-axis makes object appear different sizes
    perspectivePoints(perspective_points, perspective_matrix);
    savePoints("Points/perspective_points.txt", perspective_points);

    return 0;
}