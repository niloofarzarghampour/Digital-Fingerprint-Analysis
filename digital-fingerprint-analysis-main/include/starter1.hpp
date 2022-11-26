#ifndef starter1_hpp
#define starter1_hpp
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "starter1.hpp"
#define pi 3.1415926

using namespace std;
using namespace cv;

class Image{
private:
    Mat image;
    int width;
    int height;
public:
    // default constructor
    Image();
    // constructor with weight n, height m and a number of intensity in [0,1]
    Image(int m,int n,float intensity);
    // copy constructor
    Image(const Image &img);
    // constructor with a path of an image
    Image(const char *pathfile);
    // constructor with a matrice
    Image(Mat matrice);
    // display the image with a window name
    void display_image(const char *name) const;
    // save the image to a path
    void save_image(const char *pathfile);
    // get the width of the image
    int get_width() const;
    // get the height of the image
    int get_height() const;
    // get the matrix of the image
    Mat get_matrice() const;
    // print the minimum and the maximum of pixel intensity
    void minmax() const;
    // cast all pixels values into floating numbers
    void int_to_float();
    // cast all pixels values into int numbers
    void float_to_int();
    // return the pixel [i,j] between 0 and 1
    float get_pixel(int i,int j) const;
    // set the pixel [i,j] with an intensity between 0 and 1
    void set_pixel(int i,int j,float intensity);
    // change the pixels of the image
    // width between w1 and w2, height between h1 and h2, intensity in [0,1]
    void change_pixels(int w1,int w2,int h1,int h2,float intensity);
    // symmetry transform of the image along the y axis
    Image symmetry_yaxis();
    // symmetry transform of the image along the x axis
    Image symmetry_xaxis();
    // symmetry transform of the image along the x and y diagonal axis y = x
    Mat symmetry_first_diagonal();
    // symmetry transform of the image along the x and y diagonal axis y = -x
    Mat symmetry_second_diagonal();
    // destructor
    ~Image();
    // return the barycenter of the image
    Point barycenter() const;
    // return the distance between the given point [i,j] and the barycenter of the image
    float bary_distance(int i,int j,Point bary);
    // inverse the intensity of the image
    void inverse();
    // apply the isotropic coefficient function
    void isotropic(int method);
    // apply the anisotropic coefficient function with an angle [0,360]
    Image anisotropic(int method,float angle);
};


#endif
