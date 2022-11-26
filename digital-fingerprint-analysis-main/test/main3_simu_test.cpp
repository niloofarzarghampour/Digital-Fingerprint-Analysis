#include "../include/part3.hpp"

Mat kernel2 = (Mat_<double>(3,3) << 0,0.2,0,
        0.2,0,0.2,
        0,0.2,0);

Mat kernel3 = (Mat_<double>(3,3) <<
                                 0,-1,0,
        -1,5,-1,
        0,-1,0);

Mat kernel4 = (Mat_<double>(5,5) << 0.04, 0.04, 0.04, 0.04, 0.04,
        0.04, 0.04, 0.04, 0.04, 0.04,
        0.04, 0.04, 0.04, 0.04, 0.04,
        0.04, 0.04, 0.04, 0.04, 0.04,
        0.04, 0.04, 0.04, 0.04, 0.04);

Mat kernel5 = Mat::ones(5, 5, CV_32F);

Mat kernel8 = Mat::ones(3, 3, CV_32F)/9;

Mat kernel9 = Mat::eye(3, 3, CV_32F);


int main(){
    // ***** get barycenter *****
    Image ima("picture/clean_finger.png");
    ima.int_to_float();
    Point barycenter = ima.barycenter();

    // ***** show original picture *****
    Mat src = imread("picture/clean_finger.png", IMREAD_GRAYSCALE);
    imshow("original", src);
    waitKey();

    // ***** discrete convolution operation *****
    //src.convertTo(src,CV_8UC1,255);
    Mat dst = Convolution(src, kernel4);
    imshow("convolution result", dst);
    imwrite("result/2Dcon_kernel9.png",dst);
    waitKey();

    // ***** built in convolution operation *****
    //src.convertTo(src,CV_32FC1,1/255.0);
    Mat dst2d = filter2d(src, kernel4);
    imshow("filter2D result", dst2d);
    imwrite("result/filter2D.png",dst2d);
    waitKey();

    // ***** use DFT to do convolution *****
    src.convertTo(src,CV_32FC1,1/255.0);
    Mat res;
    res = Conv2DFT(src, kernel4, -1);
    //normalize(res,res,1,0,NORM_MINMAX,-1);
    imshow("DFT convolution", res);
    imwrite("result/DFTcon.png",res*255);
    waitKey();

    // ***** varying blur in original image *****
    ima.float_to_int();
    Mat radial = radial_blur(ima.get_matrice(), barycenter, 0.05*kernel5);
    //Mat radial = ima.radial_blur(barycenter, kernel4);
    imshow("radial blur", radial);
    imwrite("result/radial_blur.png", radial);
    waitKey();

    // ***** motion blur in original image *****
    Mat motion;
    Mat radial_motion;
    Mat Input1 = imread("picture/clean_finger.png");
    Mat Input2 = imread("all_result/part3/float(ksize-0.05*times)*kernel8.png");
    // Mat Input = imread("../result/gaussian moving blur (9,0,0).png");
    motion = MotionBlur(Input1, 90, 10);
    radial_motion = MotionBlur(Input2, 90, 10);
    imshow("motion blur", motion);
    imwrite("result/motion(90,10).png",motion);
    waitKey();
    imshow("radial and motion blur", radial_motion);
    imwrite("result/motion(90,10)+radial(kernel8).png",radial_motion);
    waitKey();



    // ***** use ideal low pass filter in original image *****
    Mat input = imread("picture/clean_finger.png", IMREAD_GRAYSCALE);
    Mat ideal = ideal_Low_Pass_Filter(input, 50);
    ideal = ideal(Rect(0,0, input.cols, input.rows));
    imshow("ideal", ideal);
    imwrite("result/ideal filter result sigma50.png", ideal*255);
    waitKey();

    // ***** built in Gaussian blur operation *****
    Mat gauss = gaussian_blur(input, 1, 0, 0);
    imshow("Gaussian blur",gauss);
    imwrite("result/Gaussian_blur_15_2_15.png", gauss);
    waitKey();

    // ***** get Gausssian kernel *****
    Mat gaussian_kernel = get_gaussian_kernel(5,15);
    cout<<"Gaussian kernel of size 3 sigma 1 is: \n"<<gaussian_kernel<<endl;
    Mat test;
    filter2D(input, test, -1 , 10*gaussian_kernel, Point(-1, -1), 0, 4);
    imshow("test", test);
    imwrite("result/gaussian_10.png", test);
    waitKey();


    // ***** add gaussian varying blur *****
    Mat gaussian_blur = gaussian_varying_blur(test, barycenter, input, 5, 15);
    imshow("gaussing changing blur", gaussian_blur);
    imwrite("result/gaussian_varying.png", gaussian_blur);
    waitKey();

    // ***** realize gaussian varying blur and motion blur on original image *****
    Mat gaussian_motion_blur = imread("all_result/part3/exp(1.6782992598876423)*exp(-0.1813496535867255*times).png");
    gaussian_motion_blur = MotionBlur(gaussian_motion_blur, 90,5);
    imshow("gaussing changing blur + motion", gaussian_motion_blur);
    imwrite("result/gaussian_varying_motion.png", gaussian_motion_blur);
    waitKey();

    return 0;
}

