#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

float dotProduct(Vec3f a, Vec3f b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

Vec3f normalize(Vec3f a) {
    float length = sqrt(dotProduct(a, a));
    return a/length;
}

/* I = K_d * I_light * (N x L)
   I - The final intensity
   K_d - Diffuse reflectance coefficient (diffuseColor)
   I_light - Intensity of the light (lightIntensity)
   N - Normalized surface normal (normal)
   L - Normalized light direction vector (lightDir)
   (N x L) - Dot product of surfance normal and light direction vector
*/
Vec3f lambertShading(Vec3f normal, Vec3f lightDir, Vec3f diffuseColor, float lightIntensity) {
    normal = normalize(normal);     //Normalizing the vectors to accurately use dot product to calculate angles
    lightDir = normalize(lightDir);

    //Gives the cosine of the angle between the vectors, helping understand how aligned the surface is to light source
    float lambertTerm = dotProduct(normal, lightDir);   
    if(lambertTerm < 0) { //Negative values indicate that surface is facing away from light, so set those to 0
        lambertTerm = 0;
    }
    return lambertTerm*lightIntensity*diffuseColor;
}

/* I = K_s * I_light * (R x V)^n
   I - The final intensity
   K_s - Specular reflectance coefficient (diffuseColor)
   I_light - Intensity of the light (lightIntensity)
   R - Reflection vector (reflection)
   V - View direction (viewDir)
   (R x V)^n - Dot product of reflectance vector and view direction to the power of shininess

   **This Phong model depends only on specular intensity but there is also diffuse (similar to lambertian) and ambient intensities
*/
Vec3f phongShading(Vec3f normal, Vec3f lightDir, Vec3f viewDir, Vec3f specularColor, float shininess, float lightIntensity) {
    normal = normalize(normal);     //Normalizing the vectors to accurately use dot product to calculate angles
    lightDir = normalize(lightDir);
    viewDir = normalize(viewDir);
    Vec3f reflection = 2*dotProduct(normal, lightDir) * normal - lightDir; //Calculating the reflection vector
    reflection = normalize(reflection);

    //Gives the cosine of the angle betweeen the vectors, helping understand the direction pointing towards the camera
    float phongTerm = dotProduct(reflection, viewDir); 
    if(phongTerm < 0) { //Negative values indicate that the surface is not reflecting ligh towards the viewer in those areas
        phongTerm = 0;
    }
    phongTerm = pow(phongTerm, shininess);
    return phongTerm*lightIntensity*specularColor;
}

int main() {
    Mat object = Mat::zeros(Size(512, 512), CV_32FC3);

    Vec3f diffuseColor(0.0, 0.0, 1.0);      //Color red
    Vec3f specularColor(1.0, 1.0, 1.0);     //Color white
    float lightIntensity = 1.0;
    float shininess = 20;

    //Dummy loop for each '3D point' (in this 2D representation)
    for(int y = 0; y < object.rows; y++) {
        for(int x = 0; x < object.cols; x++) {
            //For each point, get the normal, light direction, and view direction from data (these are all dummy values)
            Vec3f normal(x/512.0, y/512.0, 1);
            Vec3f lightDir(x/512.0, 1-y/512.0, 1);
            Vec3f viewDir(2,2,4);

            Vec3f lambert = lambertShading(normal, lightDir, diffuseColor, lightIntensity);
            Vec3f phong = phongShading(normal, lightDir, viewDir, specularColor, shininess, lightIntensity);
            object.at<Vec3f>(y,x) = lambert + phong;
        }
    }
    
    //Convert to 8-bit image for display
    object.convertTo(object, CV_8UC3, 255.0);
    imwrite("Pictures/shaded.png", object);

    return 0;
}