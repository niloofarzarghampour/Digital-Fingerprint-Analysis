#include "../include/starter1.hpp"

Image::Image(){
    image = Scalar(0);
    width = 0;
    height = 0;
}

Image::Image(int m,int n,float intensity){
    image = Mat::ones(m,n,CV_32FC1) * intensity ;
    width = n;
    height = m;
}

Image::Image(const Image &img){
    image = img.image.clone();
    width = image.size().width;
    height = image.size().height;
}

Image::Image(const char *pathfile){
    image = imread(pathfile,IMREAD_GRAYSCALE);
    if (image.empty()){
        cerr << "Image Not Found!!!" << endl;
        exit(1);
    }else{
        width = image.size().width;
        height = image.size().height;
    }
}

Image::Image(Mat matrice){
    image = matrice.clone();
    width = matrice.cols;
    height = matrice.rows;
}

void Image::display_image(const char *name) const{
    imshow(name, image);
    waitKey(0);
}

void Image::save_image(const char *pathfile){
    float_to_int();
    imwrite(pathfile,image);
}

int Image::get_width() const{
    return width;
}

int Image::get_height() const{
    return height;
}

Mat Image::get_matrice() const{
    Mat m = image.clone();
    return m;
}

void Image::minmax() const {
    double min=0, max=0;
    minMaxLoc(image,&min,&max,NULL,NULL);
    cout<<"The minimum is:"<<min<<endl;
    cout<<"The maximum is:"<<max<<endl;
}

void Image::change_pixels(int w1,int w2,int h1,int h2,float intensity){
    if (w1>w2){
        swap(w1,w2);
    }
    if (h1>h2){
        swap(h1,h2);
    }
    if (w1<0 or h1<0 or w2>width or h2>height){
        cout << "The pixel values to be modified need to be within the original image size" << endl;
    }else{
        for(int j=w1;j<w2;j++){
            for (int i=h1;i<=h2;i++){
                image.at<float>(i,j)= intensity ;
            }
        }
    }
}

float Image::get_pixel(int i,int j) const{
    return image.at<float>(i,j);
}

void Image::set_pixel(int i,int j,float intensity){
    image.at<float>(i,j) = intensity ;
}

void Image::int_to_float(){
    image.convertTo(image,CV_32FC1,1/255.0);
}

void Image::float_to_int(){
    image.convertTo(image,CV_8UC1,255);
}

Image Image::symmetry_yaxis(){
    Image symmetry;
    flip(image,symmetry.image,1);
    symmetry.width = width;
    symmetry.height = height;
    return symmetry;
}

Image Image::symmetry_xaxis(){
    Image symmetry;
    flip(image,symmetry.image,0);
    symmetry.width = width;
    symmetry.height = height;
    return symmetry;
}

Mat Image::symmetry_first_diagonal(){
    Mat symmetry;
    symmetry = Mat::zeros(width,height,CV_32FC1);
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++){
            symmetry.at<float>(i,j) = image.at<float>(height-1-j,width-1-i);
        }
    }
    return symmetry;
}

Mat Image::symmetry_second_diagonal(){
    Mat symmetry;
    symmetry = Mat::zeros(width,height,CV_32FC1);
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++){
            symmetry.at<float>(i,j) = image.at<float>(j,i);
        }
    }
    return symmetry;
}

Image::~Image(){
    image.release();
}

Point Image::barycenter() const{
    Point bary;
    float sum = 0;
    float weight = 0;
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            if(get_pixel(i,j)>=0) {
                weight = 1 - get_pixel(i,j);
                sum += weight;
                bary.x += weight * i;
                bary.y += weight * j;
            }
        }
    }
    if (sum == 0){
        bary.x = int(height/2);
        bary.y = int(width/2);
    }else{
        bary.x = int (bary.x/sum);
        bary.y = int (bary.y/sum);
    }
    return bary;
}

float Image::bary_distance(int i,int j,Point bary){
    float distance;
    distance = sqrt(pow(i-bary.x,2)+pow(j-bary.y,2));
    return distance;
}

void Image::inverse(){
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            set_pixel(i,j, 1-get_pixel(i,j));
        }
    }
}

