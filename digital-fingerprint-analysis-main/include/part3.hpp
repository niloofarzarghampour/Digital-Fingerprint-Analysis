#ifndef PART3_H
#define PART3_H

#include "../include/Eigen/Core"
#include "../include/Eigen/Dense"
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "starter1.hpp"

using namespace Eigen;

enum LinearSolverType{
    LinearSolver_SVD,
    LinearSolver_QR
};

/************************** Starter 3 **************************/
// 2D convolution
Mat Convolution(Mat InputImage,Mat kernel);
//realize convolution using DFT
Mat Conv2DFT(Mat& img, const Mat& kernel, int ddepth);
//use built in convolution
Mat filter2d(Mat InputImage, Mat kernel);

/************************** Main course 3 (simulation) **************************/
// get Gaussian kernel
// n is kernel size, sigma is the standard deviation
Mat get_gaussian_kernel(int n, double sigma);
// realize radial blur in one image
Mat radial_blur(const Image &img, Point bary, Mat kernel);
// This function uses the same idea with the last function
Mat gaussian_varying_blur(Mat ima, Point bary, Mat Input, int ksize, double sigma);
// add motion blur
Mat MotionBlur(Mat& img, int angle, int distance);
// add Gaussian blur
Mat gaussian_blur(Mat& input, int ksize, double sigmaX, double sigmaY);
// ideal low-pass filter
Mat ideal_lbrf_kernel(Mat &scr,float sigma);
Mat ideal_Low_Pass_Filter(Mat &src, float sigma);
// frequency filter
Mat freqfilt(Mat &scr,Mat &blur);

/************************** Main course 3 (restauration) **************************/
Mat linearsolver(const Image &img,MatrixXf& K, LinearSolverType type );

#endif