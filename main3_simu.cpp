#include "../include/part3.hpp"

// get Gaussian kernel
// n is kernel size, sigma is the standard deviation
Mat get_gaussian_kernel(int n, double sigma){
    Mat kernel(n, 1, CV_32F); // create a n*1 kernel array
    float* cf = (float*)kernel.data; // a pointer pointing to kernel
    // when sigma <= 0, use formula
    double Sigma = sigma > 0 ? sigma :((n-1)*0.5 - 1)*0.3 + 0.8;
    // Gaussian expression
    double scale2X = -0.5/(Sigma*Sigma);
    double sum = 0;

    int i;
    for(i = 0; i<n; i++){
        double x = i - (n-1)*0.5;
        double t = exp(scale2X*x*x); // Gaussian function
        cf[i] = (float)t;
        sum += cf[i]; //for normalization
    }
    sum = 1./sum; // normolization
    for( i = 0; i < n; i++ )
    {
        cf[i] = (float)(cf[i]*sum);
    }

    kernel = kernel*kernel.t(); //(n*1)*(1*n) = (n*n), get Gaussian kernel

    return kernel;

}

// realize radial blur in one image
// What we want to do here is to use different kernel in different area of the image.The kernel would change to identity kernel when it reaches barycenter. The idea is that first we blur the original image with one kernel, then we extract area from the original image and extract the same size area from the blurred image. After that, we replace the area from the original image by the area extracted from the blurred image. We keep repeating this process from the boundary to the barycenter. More importantly, the blur step should be done on the original image, not the image after replace and the area we extract has the same ratio.

Mat radial_blur(const Image &img,Point bary, Mat kernel){
    float ksize = 2.05;
    Mat blur;// mat used for doing blur step in the original image
    Mat extract;// mat used for extracting area from the original image
    Mat addblur;// mat used for extracting area from Mat blur
    Mat ima = img.get_matrice();
    int h = ima.rows;
    int w = ima.cols;
    int i = 0;
    int cycle = 20;// the number of while loop

    // in order to make the number of while loop equals to cycle
    int add = int(bary.y/(cycle-1));
    int times = 0;// used for changing the blur kernel

    while(i<bary.y){
        Mat Kernel = float(ksize-0.05*times)*kernel;
        times++;
        //cout<<Kernel<<endl;
        cout<<"coefficient: "<<float(ksize-0.05*times)<<endl;

        Mat tran = img.get_matrice();

        filter2D(tran, blur, -1, Kernel, Point(-1,-1), 0,4);

        float x = w - i*(w-bary.x)/bary.y;
        float y = bary.y+(bary.x-i*bary.x/bary.y)*(h-bary.y)/bary.x;

        // extract area from the original and blurred image
        extract = ima(Range(i,y), Range(i*bary.x/bary.y,x));
        addblur = blur(Range(i,y), Range(i*bary.x/bary.y,x));

        // replace the area from original image by the area from blurred image
        addWeighted(extract, 0, addblur, 1, 0., extract);

        i += add;

    }

    return ima;

}


// This function uses the same idea with the last function
Mat gaussian_varying_blur(Mat ima, Point bary, Mat Input, int ksize, double sigma){
    float size = 4;
    Mat blur(ima.rows, ima.cols, ima.type());// mat used for doing blur step in the original image
    Mat extract;// mat used for extracting area from the original image
    Mat addblur;// mat used for extracting area from Mat blur
    int h = ima.rows;
    int w = ima.cols;
    int i = 0;
    int index = 0;
    int cycle = 10;// the number of while loop

    // in order to make the number of while loop equals to cycle
    int add = int(bary.y/(cycle-1));
    int times = 1;

    while(i<bary.y){

        Mat tran = Input;

        Mat kernel = get_gaussian_kernel(ksize, sigma);
        sigma -= 1;

        float part1 = exp(1.6782992598876423);
        float part2 = exp(-0.1813496535867255*times);
        Mat Kernel = (part1*part2)* kernel;

        times++;
        float sum = 0;
        for( int i = 0; i< Kernel.rows; i++){
            for(int j = 0; j< Kernel.cols; j++){
                sum += Kernel.at<float>(i,j);
            }
        }
        filter2D(tran, blur, -1, Kernel, Point(-1,-1), 0,4);

        float x = w - i*(w-bary.x)/bary.y;
        float y = bary.y+(bary.x-i*bary.x/bary.y)*(h-bary.y)/bary.x;

        // extract area from the original and blurred image
        extract = ima(Range(i,y), Range(i*bary.x/bary.y,x));
        addblur = blur(Range(i,y), Range(i*bary.x/bary.y,x));

        // replace the area from original image by the area from blurred image
        addWeighted(extract, 0, addblur, 1, 0., extract);

        i += add;

    }

    return ima;

}



// add motion blur
Mat MotionBlur(Mat& img,  int angle, int distance)
{
    // In image processing, downward y asis is positive direction.
    // So we need to rotate the angle 180 degree along square.
    angle = -1*angle % 360;
    double radian = ((double)angle + 180.0) / 180.0*3.1415926;
    // displacement along x asis and y asis
    int dx = (int)((double)distance* cos(radian) + 0.5);
    int dy = (int)((double)distance* sin(radian) + 0.5);

    cout<<"angle: "<<angle<<endl;
    cout<<"radian: "<<radian<<endl;
    cout<<"dx: "<<dx<<endl;
    cout<<"dy: "<<dy<<endl;

    int sign = 0;
    int height = img.rows;
    int width = img.cols;
    int chns = img.channels();

    // create the result mat
    Mat dst(height, width, img.type());
    int i, j, k;
    int i0=0, j0=0, p, sum, count;

    // This operation is like putting multiple same images in a specific direction then getting the average value. Also, we need to deal with the transparency degree of image in order to reach the most ideal blur effect.
    for (i = 0; i < height; i++)
    {
        unsigned char* dstData = (unsigned char*)dst.data + dst.step*i;
        for (j = 0; j < width; j++)
        {
            for (int k = 0; k < chns; k++)
            {
                sum = 0, count = 0;
                sign = (dx > 0) ? 1 : -1;
                for (p = 0; p < abs(dx); p++)
                {
                    i0 = i;
                    j0 = j+p * sign;
                    if (i0 >= 0 && i0 < height && j0 >= 0 && j0 < width)
                    {
                        count++;
                        sum += img.at<Vec3b>(i0, j0)[k];
                    }
                }
                sign = (dy > 0) ? 1 : -1;
                for (p = 0; p < abs(dy); p++)
                {
                    i0 = i + p * sign;
                    j0 = j;
                    if (i0 >= 0 && i0 < height && j0 >= 0 && j0 < width)
                    {
                        count++;
                        sum += img.at<Vec3b>(i0, j0)[k];
                    }
                }

                if (count == 0)
                {
                    dstData[j*chns + k] = img.at<Vec3b>(i0, j0)[k];
                }
                else
                {
                    dstData[j*chns + k] = saturate_cast<uchar>(sum / (double)count + 0.5);
                }

            }
        }
    }

    return dst;

}



// add Gaussian blur
Mat gaussian_blur(Mat& input, int ksize, double sigmaX, double sigmaY){
    Mat dst(input.rows, input.cols, input.type());
    GaussianBlur(input, dst, Size(ksize, ksize), sigmaX, sigmaY);

    return dst;
}


Mat ideal_Low_Pass_Filter(Mat &src, float sigma);
Mat ideal_lbrf_kernel(Mat &scr,float sigma);
Mat freqfilt(Mat &scr,Mat &blur);

//***************** ideal low-pass filter ***********************
Mat ideal_lbrf_kernel(Mat &scr,float sigma)
{
    // create the kernel of ideal low pass filter
    Mat ideal_low_pass(scr.size(),CV_32FC1);
    // cut off frequency
    // the smaller d0 is，the bigger the blur is
    float d0=sigma;
    for(int i=0;i<scr.rows ; i++ ){
        for(int j=0; j<scr.cols ; j++ ){
            double d = sqrt(pow((i - scr.rows/2),2) + pow((j - scr.cols/2),2));//分子,计算pow必须为float型
            if (d <= d0){
                ideal_low_pass.at<float>(i,j)=1;
            }
            else{
                ideal_low_pass.at<float>(i,j)=0;
            }
        }
    }
    string name = "low pass filter" ;
    imshow(name, ideal_low_pass);
    imwrite("result/Filter displayed as an image (sigma 50).png", ideal_low_pass*255);
    return ideal_low_pass;
}

Mat ideal_Low_Pass_Filter(Mat &src, float sigma)
{
    // expand the input image to the ideal size, padding the border with 0
    int M = getOptimalDFTSize(src.rows);
    int N = getOptimalDFTSize(src.cols);
    Mat padded;                 //expand the image to speed up dft
    copyMakeBorder(src, padded, 0, M - src.rows, 0, N - src.cols, BORDER_CONSTANT, Scalar::all(0));
    padded.convertTo(padded,CV_32FC1);

    // ideal kernel
    Mat ideal_kernel=ideal_lbrf_kernel(padded,sigma);

    float sum = 0;
    for(int i = 0; i<ideal_kernel.rows; i++){
        for(int j = 0; j<ideal_kernel.cols; j++){
            sum += ideal_kernel.at<float>(i,j);
        }
    }
    ideal_kernel /= sum;

    Mat result = freqfilt(padded,ideal_kernel);

    return result;
}

//***************** frequency filter *******************
Mat freqfilt(Mat &scr,Mat &blur)
{
    //***********************DFT*******************
    // allocate storage space to the result of DFT (real part and imaginary part)
    // merge planes array to a complexI array with channels
    Mat plane[]={scr, Mat::zeros(scr.size() , CV_32FC1)};
    Mat complexIm;
    merge(plane,2,complexIm);

    // DFT and store the result in itself
    dft(complexIm,complexIm);


    //*************** centralization ********************
    split(complexIm,plane);//split arrays

    // If there exits odd rows or cols, cut the frequency.
    plane[0] = plane[0](Rect(0, 0, plane[0].cols & -2, plane[0].rows & -2));
    //coordinate is expressed by cx and cy
    int cx=plane[0].cols/2;
    int cy=plane[0].rows/2;
    // reorder quadrant of Fourier image, makeing the original center located in the center of image
    Mat part1_r(plane[0],Rect(0,0,cx,cy));
    Mat part2_r(plane[0],Rect(cx,0,cx,cy));
    Mat part3_r(plane[0],Rect(0,cy,cx,cy));
    Mat part4_r(plane[0],Rect(cx,cy,cx,cy));

    //switch positions for real part
    Mat temp;
    part1_r.copyTo(temp);
    part4_r.copyTo(part1_r);
    temp.copyTo(part4_r);

    part2_r.copyTo(temp);
    part3_r.copyTo(part2_r);
    temp.copyTo(part3_r);

    Mat part1_i(plane[1],Rect(0,0,cx,cy));
    Mat part2_i(plane[1],Rect(cx,0,cx,cy));
    Mat part3_i(plane[1],Rect(0,cy,cx,cy));
    Mat part4_i(plane[1],Rect(cx,cy,cx,cy));

    //switch positions for imaginary part
    part1_i.copyTo(temp);
    part4_i.copyTo(part1_i);
    temp.copyTo(part4_i);

    part2_i.copyTo(temp);
    part3_i.copyTo(part2_i);
    temp.copyTo(part3_i);

    //***************** product of filter function and DFT result ****************
    Mat blur_r,blur_i,BLUR;
    //filtering (real part mutiply with corresponding values of filtering)
    multiply(plane[0], blur, blur_r);
    //filtering (imaginary part mutiply with corresponding values of filtering)
    multiply(plane[1], blur,blur_i);
    Mat plane1[]={blur_r, blur_i};
    merge(plane1,2,BLUR);//merge real part and imaginary part

    //************* get frequency spectrum of original image *************************
    //get magnitude image, 0 channel is for real part, 1 is for imaginary part
    magnitude(plane[0],plane[1],plane[0]);
    //logarithmic scale
    plane[0]+=Scalar::all(1);
    log(plane[0],plane[0]);    // float [0，1]
    normalize(plane[0],plane[0],1,0,NORM_MINMAX);  //normalization for show
    imshow("spectrum of original",plane[0]);
    imwrite("result/spectrum.png", plane[0]*255);

    idft( BLUR, BLUR);
    split(BLUR,plane);
    magnitude(plane[0],plane[1],plane[0]);
    normalize(plane[0],plane[0],1,0,NORM_MINMAX);
    return plane[0];
}


