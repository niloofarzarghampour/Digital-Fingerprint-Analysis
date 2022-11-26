#include "../include/starter1.hpp"
#include "../include/main1_rest.hpp"


void Image::isotropic(int method){
    Point bary = barycenter();
    inverse();
    float new_pixel,distance,coeff;
    float max1 = 0;
    for (int i=0;i<get_height();i++){
        for (int j=0;j<get_width();j++){
            distance = bary_distance(i,j,bary);
            if (method == 1){
                // c(r) = exp(-r^2 / 1000)
                coeff = exp(-pow(distance,2)/1000);
            }else if(method == 2){
                // c(r) = 1/(r^2/1000 + 1)
                coeff = 1 / (pow(distance,2)/1000+1);
            }else if(method == 3){
                // c(r) = 1/(1+exp^(0.45*r-30))
                coeff = 1/(1+exp(0.45*distance-30));
            }
            new_pixel = coeff * get_pixel(i,j);
            set_pixel(i,j,1-new_pixel);
        }
    }
}

Image Image::anisotropic(int method,float angle) {
    Image result(height,width,1);
    Ellipse e(*this);
    Mask ellipse(e);
    Point bary = barycenter();
    inverse();
    float new_pixel,distance,coeff,radian,a,b,x,y;
    for (int i=0;i<get_height();i++){
        for (int j=0;j<get_width();j++){
            radian = M_PI/180 * angle;
            a = e.get_a()/float(height);
            b = e.get_b()/float(width);
            x = (i - bary.x) * cos(radian) - (bary.y - j) *sin(radian)+bary.x;
            y = (i - bary.x) * sin(radian) + (bary.y - j) *cos(radian)+bary.y;
            distance = sqrt(pow((x-bary.x)/a,2)+pow((y-bary.y)/b,2));
            if (method == 1) {
                // c(r) = exp(-r^2 / 10000)
                coeff = exp(-pow(distance, 2) / 10000 );
            } else if (method == 2) {
                // c(r) = 1/(r^2/10000 + 1)
                coeff = 1 / (pow(distance, 2) / 10000 + 1);
            } else if (method == 3) {
                // c(r) = 1/(1+exp^(0.25*r-50))
                coeff = 1/(1+exp(0.25*distance-50));
            }
            new_pixel = coeff * get_pixel(i, j);
            result.set_pixel(i, j, 1 - new_pixel);
        }
    }
    return result;
}
