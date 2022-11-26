#include "../include/part3.hpp"

// 2D convolution
Mat Convolution(Mat InputImage,Mat kernel)
{
    // create the output image with the same size of the input image
    Mat OutputImage(InputImage.rows, InputImage.cols,CV_8UC1,Scalar(0));
    //calculate radius of kernel
    int sub_x = kernel.cols/2;
    int sub_y = kernel.rows/2;

    // create border with all white pixel based on the kernel size
    copyMakeBorder(InputImage, InputImage, sub_x, sub_x, sub_y, sub_y, BORDER_CONSTANT, Scalar(255,255,255));

    //go through the whole pic
    for (int image_y=0;image_y<InputImage.rows;image_y++)
    {
        for(int image_x=0;image_x<InputImage.cols;image_x++)
        {
            int pix_value = 0;
            for (int kernel_y = 0;kernel_y<kernel.rows;kernel_y++)
            {
                for(int kernel_x = 0;kernel_x<kernel.cols;kernel_x++)
                {
                    double  weihgt = kernel.at<double>(kernel_y,kernel_x);
                    int value =  (int)InputImage.at<uchar>(image_y+kernel_y,image_x+kernel_x);
                    pix_value +=weihgt*value;
                }
            }

            OutputImage.at<uchar>(image_y,image_x) = (uchar)pix_value;

        }
    }

    return OutputImage;
}



//realize convolution using DFT
Mat Conv2DFT(Mat& img, const Mat& kernel, int ddepth)
{
    // define result image with the same size of input image
    Mat convRes(img.rows, img.cols,CV_8UC1,Scalar(0));

    // extend the border with white pixels
    copyMakeBorder(img, img, kernel.cols/2, kernel.cols/2, kernel.rows/2, kernel.rows/2, BORDER_CONSTANT, Scalar(255,255,255));

    // calculate the size of DFT transform
    int dft_M = getOptimalDFTSize(img.rows + kernel.rows - 1);
    int dft_N = getOptimalDFTSize(img.cols + kernel.cols - 1);

    // allocate temporary buffers and initialize them with 0
    Mat imagePad(dft_M, dft_N, CV_32FC1, Scalar(0));
    Mat imagePadROI = imagePad(Rect(0, 0, img.cols, img.rows));
    img.convertTo(imagePadROI, CV_32FC1, 1, 0);

    Mat kernelPad(dft_M, dft_N, CV_32FC1, Scalar(0));
    Mat kernelPadROI = kernelPad(Rect(0, 0, kernel.cols, kernel.rows));
    kernel.convertTo(kernelPadROI, CV_32FC1, 1, 0);

    // DFT and use "nonzeroRows" hint for faster processing
    dft(imagePad, imagePad, 0, imagePad.rows);
    dft(kernelPad, kernelPad, 0, kernelPad.rows);

    // mutiply the spectrums
    // set the last parameter to false to compute convolution instead of correlation
    mulSpectrums(imagePad, kernelPad, imagePad, DFT_COMPLEX_OUTPUT, false);
    //IDFT
    idft(imagePad, imagePad, DFT_SCALE | DFT_REAL_OUTPUT, imagePad.rows);

    // output the result
    Rect r;
    r = Rect((kernel.cols + 0.5) / 2, (kernel.rows + 0.5) / 2, convRes.cols, convRes.rows);
    imagePad(r).convertTo(convRes, ddepth, 1, 0);

    return convRes;
}




//use built in convolution
Mat filter2d(Mat InputImage, Mat kernel){
    Mat OutputImage;

    filter2D(InputImage, OutputImage, -1 , kernel, Point(-1, -1), 0, 4);
    return OutputImage;
}

